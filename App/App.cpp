/*
 *
 *
 */
// sgx includes
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
#include "sgx_capable.h"
#include "sgx_uae_service.h"
// standard libs
#include <iostream>
#include <stdio.h>
#include <fstream>
// custom libs
#include "Wallet.h"
#include "enclave.h"

using namespace std;



/***************************************************
 *
 * Defines
 *
 ***************************************************/
#define APP_NAME "SGX-WALLET"
#define VERSION "0.0.1"
#define ENCLAVE_FILE "enclave.signed.so"
#define ENCLAVE_TOKEN "enclave.token"
#define WALLET_FILE "wallet.seal"


/***************************************************
 *
 * OCALL implementation - debug print implementation
 *
 ***************************************************/
void ocall_debug_print(const char* str) {
    printf("[DEBUG] %s\n", str);
}
int ocall_save_wallet(const uint8_t* sealed_data, const size_t sealed_size) {
    ofstream file(WALLET_FILE, ios::out | ios::binary);
    if (file.fail()) {return 1;}
    file.write((const char*) sealed_data, sealed_size);
    file.close();
    return 0;
}
int ocall_load_wallet(uint8_t* sealed_data, const size_t sealed_size) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 1;}
    file.read((char*) sealed_data, sealed_size);
    file.close();
    return 0;
}
int ocall_is_wallet(void) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 0;} // failure means no wallet 
    file.close();
    return 1;
}
// tmp
int ocall_save(const wallet_t* data, size_t len) {
    ofstream file(WALLET_FILE, ios::out | ios::binary);
    if (file.fail()) {return 1;}
    file.write((const char*) data, len);
    file.close();
    return 0;
}
int ocall_load(wallet_t* data, size_t len) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {return 1;}
    file.read((char*) data, len);
    file.close();
    return 0;
}



/***************************************************
 *
 * Helper functions
 *
 ***************************************************/

/**
 *
 *
 */
void info_print(const char* str) {
    printf("[INFO] %s\n", str);
}

/**
 *
 *
 */
void warning_print(const char* str) {
    printf("[WARNING] %s\n", str);
}

/**
 *
 *
 */
void error_print(const char* str) {
    printf("[ERROR] %s\n", str);
}

/**
 *
 *
 */
void print_wallet(const wallet_t* wallet) {
    printf("\n-----------------------------------------\n\n");
    printf("%s v%s\n", APP_NAME, VERSION);
    printf("Simple password wallet based on Intel SGX.\n\n");
    printf("Number of items: %lu\n\n", wallet->size);
    for (int i = 0; i < wallet->size; ++i) {
        printf("#%d -- %s\n", i, wallet->items[i].title);
        printf("[username:] %s\n", wallet->items[i].username);
        printf("[password:] %s\n", wallet->items[i].password);
        printf("\n");
    }
    printf("\n------------------------------------------\n\n");
}

/**
 *
 *
 */
int is_error(int ret) {
    char err_message[100];

    switch(ret) {
        case RET_SUCCESS:
            return 0;

        case ERR_PASSWORD_OUT_OF_RANGE:
            sprintf(err_message, "Password should be at least 8 characters long and at most %d.", MAX_ITEM_SIZE);
            error_print(err_message); 
            return 1;

        case ERR_WALLET_ALREADY_EXISTS:
            sprintf(err_message, "Wallet already exists: delete file '%s' first.", WALLET_FILE);
            error_print(err_message); 
            return 1;

        case ERR_CANNOT_SAVE_WALLET:
            error_print("Coud not save wallet."); 
            return 1;

        case ERR_CANNOT_LOAD_WALLET:
            error_print("Coud not load wallet.");
            return 1;

        case ERR_WRONG_MASTER_PASSWORD:
            error_print("Wrong master password."); 
            return 1;

        case ERR_WALLET_FULL:
            sprintf(err_message, "Wallet full (maximum number of item: %d).", MAX_ITEMS);
            error_print(err_message);
            return 1;

        case ERR_ITEM_DOES_NOT_EXIST: 
            error_print("Item does not exist."); 
            return 1;

        default:
            error_print("Unknown error."); 
            return 1;
    }
}


/*
void save(wallet_t* data, size_t len) {
    ofstream file(WALLET_FILE, ios::out | ios::binary);
    if (file.fail()) {error_print("Couldn't save wallet."); return;}
    file.write((char*) data, len);
    file.close();
}
void load(wallet_t* data, size_t len) {
    ifstream file(WALLET_FILE, ios::in | ios::binary);
    if (file.fail()) {error_print("Couldn't load wallet."); return;}
    file.read((char*) data, len);
    file.close();
}
*/

/***************************************************
 *
 * main
 *
 ***************************************************/
int main(int argc, char const *argv[]) {
    sgx_enclave_id_t global_eid = 0;

    ////////////////////////////////////////////////
    // initialise enclave
    ////////////////////////////////////////////////
    if (initialize_enclave(&global_eid, ENCLAVE_TOKEN, ENCLAVE_FILE) < 0) {
        error_print("Fail to initialize enclave."); return 1;
    }
    info_print("Enclave successfully initilised.");



    ////////////////////////////////////////////////
    // test enclave
    ////////////////////////////////////////////////
    int ptr; // return value
    int ret; // return value
    sgx_status_t status; // sgx return status
    sgx_status_t ecall_status; // ecall return status
    char string_buffer[50]; // string buffer for format printing

    // test simple ECALL
    /*
    status = generate_random_number(global_eid, &ptr);
    if (status != SGX_SUCCESS) {
        error_print("Fail test ECALL."); return 1; 
    }
    sprintf(string_buffer, "Random number generated: %d", ptr);
    info_print(string_buffer);


    // seal / unseal the random number
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(ptr);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    status = seal(
        global_eid, &ecall_status, 
        (uint8_t*) &ptr, sizeof(ptr), 
        (sgx_sealed_data_t*) sealed_data, sealed_size
    );

    if (status != SGX_SUCCESS || ecall_status != SGX_SUCCESS) {
        error_print("Sealing failed."); return 1;
    }

    int unsealed;
    status = unseal(
        global_eid, &ecall_status,
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
    const char master_password[MAX_BUF_LEN] = "HELLO!HELLO!HELLO!";
    const char new_master_password[MAX_BUF_LEN] = "NEW PASSword here";
    const char title[MAX_BUF_LEN] = "title item";
    const char username[MAX_BUF_LEN] = "asonnino2";
    const char password[MAX_BUF_LEN] = "test1234";
    
    // check title size
    warning_print("Input checks should happen in the encalve.");
    const int title_len = strlen(title)+1;
    if (title_len > MAX_BUF_LEN) {
        error_print("Item title too long."); return 1;
    }

    // check username size
    const int username_len = strlen(username)+1;
    if (username_len > MAX_BUF_LEN) {
        error_print("Username too long."); return 1;
    }

    // check password size
    const int password_len = strlen(password)+1;
    if (password_len > MAX_BUF_LEN) {
        error_print("Password too long."); return 1;
    }


    ////////////////////////////////////////////////
    // enclave tests
    ////////////////////////////////////////////////
    // create wallet
    ecall_status = ecall_create_wallet(global_eid, &ret, master_password);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to create new wallet.");
        return -1;
    }
    info_print("Wallet successfully created.");


    // add item
    item_t new_item;
    strncpy(new_item.title, title, title_len); 
    strncpy(new_item.username, username, username_len); 
    strncpy(new_item.password, password, password_len);
    ecall_status = ecall_add_item(global_eid, &ret, master_password, &new_item, sizeof(item_t));
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to add new item to wallet.");
        return -1;
    }
    info_print("Item successfully added to the wallet.");


    // test change password
    ecall_status = ecall_change_master_password(global_eid, &ret, master_password, new_master_password);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail change master-password.");
        return -1;
    }
    info_print("Master-password successfully changed.");


    // add a second item
    ecall_status = ecall_add_item(global_eid, &ret, new_master_password, &new_item, sizeof(item_t));
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to add new item to wallet.");
        return -1;
    }
    info_print("Item successfully added to the wallet.");


    // remove second item
    const int index = 1;
    ecall_status = ecall_remove_item(global_eid, &ret, new_master_password, index);
    if (ecall_status != SGX_SUCCESS || is_error(ret)) {
        error_print("Fail to remove item.");
        return -1;
    }
    info_print("Item successfully removed from the wallet.");


    // show wallet
    wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
    ecall_status = ecall_show_wallet(global_eid, &ret, new_master_password, wallet, sizeof(wallet_t));
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
    // TODO
    //info_print("Enclave successfully destroyed.");
    warning_print("Not implemented: Enclave should be detroyed here.");


    ////////////////////////////////////////////////
    // exit success
    ////////////////////////////////////////////////
    info_print("Program exit success.");
    return 0;
}
