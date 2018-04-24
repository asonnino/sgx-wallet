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

using namespace std;

/* global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;


/***************************************************
 *
 * Defines
 *
 ***************************************************/
#define APP_NAME "SGX-WALLET"
#define VERSION "0.0.1"
#define ENCLAVE_FILE "enclave.signed.so"
#define ENCLAVE_TOKEN "enclave.token"
#define WALLET_FILE "wallet.bin"
#define MAX_BUF_LEN 100


/***************************************************
 *
 * OCALL implementation - debug print implementation
 *
 ***************************************************/
void ocall_debug_print(const char* str) {
    printf("[DEBUG] %s\n", str);
}
void ocall_save(const uint8_t* sealed_data, size_t sealed_size) {
    ofstream file;
    file.open(WALLET_FILE, ios::out | ios::binary);
    file.write((const char*) sealed_data, sealed_size);
    file.close();
}



/***************************************************
 *
 * Helpers
 *
 ***************************************************/
void info_print(const char* str) {printf("[INFO] %s\n", str);}
void warning_print(const char* str) {printf("[WARNING] %s\n", str);}
void error_print(const char* str) {printf("[ERROR] %s\n", str);}



void save(wallet_t* data, size_t len) {
    ofstream file;
    file.open(WALLET_FILE, ios::out | ios::binary);
    file.write((char*) data, len);
    file.close();
}
void load(wallet_t* data, size_t len) {
    ifstream file;
    file.open(WALLET_FILE, ios::out | ios::binary);
    file.read((char*) data, len);
    file.close();
}

/***************************************************
 *
 * main
 *
 ***************************************************/
int main(int argc, char const *argv[]) {
    ////////////////////////////////////////////////
    // print welcome message
    ////////////////////////////////////////////////
    printf("\n-----------------------------------------\n\n");
    printf("%s v%s\n", APP_NAME, VERSION);
    printf("Simple password wallet based on Intel SGX.\n\n");
    printf("------------------------------------------\n\n");


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
    sgx_status_t status; // sgx return status
    sgx_status_t ecall_status; // ecall return status
    char string_buffer[50]; // string buffer for format printing

    // test simple ECALL
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


    ////////////////////////////////////////////////
    // read input arguments 
    ////////////////////////////////////////////////
    //read user input and opt for adding new password 
    // or display (cat) password content
    const char title[MAX_BUF_LEN] = "title item";
    const char username[MAX_BUF_LEN] = "asonnino2";
    const char password[MAX_BUF_LEN] = "test1234";
    const char master_password[MAX_BUF_LEN] = "HELLO!";
    /*
    status = add_item(global_eid, &ptr, title, username, password);
    if (status != SGX_SUCCESS) {
        error_print("Fail to add item."); return 1; 
    }
    switch (ptr) {
        case 0: info_print("Item successfully added to the wallet."); break;
        case 1: error_print("Item title too long."); return 1;
        case 2: error_print("Username too long."); return 1;
        case 3: error_print("Password too long."); return 1;
        default: error_print("Fail to add item."); return 1;
    }
    */
    // check title size
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

    // first create a data struct for the new item
    item_t new_item;
    strncpy(new_item.title, title, title_len); 
    strncpy(new_item.username, username, username_len); 
    strncpy(new_item.password, password, password_len); 

    wallet_t new_wallet;
    new_wallet.size = 0;
    strncpy(new_wallet.master_password, master_password, strlen(master_password)+1); 


    // add item
    new_wallet.items[0] = new_item;
    ++new_wallet.size;

    // seal data here

    // save
    save(&new_wallet, sizeof(new_wallet));


    wallet_t read_wallet;
    load(&read_wallet, sizeof(read_wallet));

    // unseal data here
    
    info_print(read_wallet.items[read_wallet.size-1].username);
    info_print(read_wallet.master_password);


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
