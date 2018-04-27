/*
 * Copyright 2018 Alberto Sonnino
 * 
 * This file is part of SGX-WALLET.
 * 
 * SGX-WALLET is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SGX-WALLET is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

// sgx includes
#include "enclave_u.h"
#include "sgx_urts.h"
// standard libs
#include <cstring>
#include <fstream>
// custom libs
#include "app.h"
#include "utils.h"
#include "ocalls.h"
#include "debug.h"
#include "wallet.h"
#include "enclave.h"

using namespace std;


/***************************************************
 * OCALLs implementation
 ***************************************************/

/*
 *
 */
void ocall_debug_print(const char* str) {
    printf("[DEBUG] %s\n", str);
}

/*
 *
 */
int ocall_save_wallet(const uint8_t* sealed_data, const size_t sealed_size) {
    ofstream file(WALLET_FILE, ios::out | ios::binary);
    if (file.fail()) {return 1;}
    file.write((const char*) sealed_data, sealed_size);
    file.close();
    return 0;
}

/*
 *
 */
int ocall_load_wallet(uint8_t* sealed_data, const size_t sealed_size) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 1;}
    file.read((char*) sealed_data, sealed_size);
    file.close();
    return 0;
}

/*
 *
 */
int ocall_is_wallet(void) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 0;} // failure means no wallet 
    file.close();
    return 1;
}


/***************************************************
 * OCALLs debug implementation
 ***************************************************/
int ocall_save_tmp(const wallet_t* data, size_t len) {
    ofstream file(WALLET_FILE, ios::out | ios::binary);
    if (file.fail()) {return 1;}
    file.write((const char*) data, len);
    file.close();
    return 0;
}
int ocall_load_tmp(wallet_t* data, size_t len) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 1;}
    file.read((char*) data, len);
    file.close();
    return 0;
}



/***************************************************
 * main
 ***************************************************/
int main(int argc, char const *argv[]) {
    // declare enclave & return variables
    sgx_enclave_id_t eid = 0;
    sgx_launch_token_t token = {0};
    int updated, ret;
    sgx_status_t ecall_status, enclave_status;


    ////////////////////////////////////////////////
    // initialise enclave
    ////////////////////////////////////////////////
    enclave_status = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
    if(enclave_status != SGX_SUCCESS) {
        error_print("Fail to initialize enclave."); 
        return -1;
    }
    info_print("Enclave successfully initilised.");




    ////////////////////////////////////////////////
    // test enclave
    ////////////////////////////////////////////////
    /*
    int ptr; // return value
    int ret; // return value
    sgx_status_t status; // sgx return status
    sgx_status_t ecall_status; // ecall return status
    char string_buffer[50]; // string buffer for format printing

    // test simple ECALL

    status = generate_random_number(eid, &ptr);
    if (status != SGX_SUCCESS) {
        error_print("Fail test ECALL."); return 1; 
    }
    sprintf(string_buffer, "Random number generated: %d", ptr);
    info_print(string_buffer);


    // seal / unseal the random number
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(ptr);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    status = seal(
        eid, &ecall_status, 
        (uint8_t*) &ptr, sizeof(ptr), 
        (sgx_sealed_data_t*) sealed_data, sealed_size
    );

    if (status != SGX_SUCCESS || ecall_status != SGX_SUCCESS) {
        error_print("Sealing failed."); return 1;
    }

    int unsealed;
    status = unseal(
        eid, &ecall_status,
        (sgx_sealed_data_t*) sealed_data, sealed_size,
        (uint8_t*) &unsealed, sizeof(unsealed)
    );

    if (status != SGX_SUCCESS || ecall_status != SGX_SUCCESS) {
        error_print("Unsealing failed."); return 1;
    }

    free(sealed_data);

    sprintf(string_buffer, "Seal round trip success! Receive back: %d", unsealed);
    info_print(string_buffer);
    */


    ////////////////////////////////////////////////
    // read input arguments 
    ////////////////////////////////////////////////
    //read user input and opt for adding new password 
    const char master_password[MAX_ITEM_SIZE] = "This is the master-password";
    const char new_master_password[MAX_ITEM_SIZE] = "This is the new master-password";
    const char title[MAX_ITEM_SIZE] = "New Item Title";
    const char username[MAX_ITEM_SIZE] = "asonnino";
    const char password[MAX_ITEM_SIZE] = "test1234";
    
    
    ////////////////////////////////////////////////
    // enclave tests
    ////////////////////////////////////////////////
    // create wallet
    ecall_status = ecall_create_wallet(eid, &ret, master_password);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to create new wallet.");
        return -1;
    }
    info_print("Wallet successfully created.");


    // add item
    item_t new_item;
    strcpy(new_item.title, title); 
    strcpy(new_item.username, username); 
    strcpy(new_item.password, password);
    ecall_status = ecall_add_item(eid, &ret, master_password, &new_item, sizeof(item_t));
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to add new item to wallet.");
        return -1;
    }
    info_print("Item successfully added to the wallet.");


    // test change password
    ecall_status = ecall_change_master_password(eid, &ret, master_password, new_master_password);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail change master-password.");
        return -1;
    }
    info_print("Master-password successfully changed.");


    // add a second item
    ecall_status = ecall_add_item(eid, &ret, new_master_password, &new_item, sizeof(item_t));
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to add new item to wallet.");
        return -1;
    }
    info_print("Item successfully added to the wallet.");


    // remove second item
    const int index = 1;
    ecall_status = ecall_remove_item(eid, &ret, new_master_password, index);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to remove item.");
        return -1;
    }
    info_print("Item successfully removed from the wallet.");


    // show wallet
    wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
    ecall_status = ecall_show_wallet(eid, &ret, new_master_password, wallet, sizeof(wallet_t));
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to retrieve wallet.");
        return -1;
    }
    info_print("Wallet successfully retrieved.");
    print_wallet(wallet);
    free(wallet);



    ////////////////////////////////////////////////
    // destroy enclave
    ////////////////////////////////////////////////
    enclave_status = sgx_destroy_enclave(eid);
    if(enclave_status != SGX_SUCCESS) {
        error_print("Fail to destroy enclave."); 
        return -1;
    }


    ////////////////////////////////////////////////
    // exit success
    ////////////////////////////////////////////////
    info_print("Program exit success.");
    return 0;
}
