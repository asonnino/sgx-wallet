/*
 *
 *
 */
// sgx includes
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
// standard libs
#include <stdio.h>

/* global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;


/***************************************************
 *
 * Defines
 *
 ***************************************************/
#define _APP_NAME_ "SGX-WALLET"
#define _VERSION_ "0.0.1"


/***************************************************
 *
 * OCALL implementation - debug print implementation
 *
 ***************************************************/
void ocall_debug_print(const char* str) {
    printf("[DEBUG] %s\n", str);
}


/***************************************************
 *
 * Helpers
 *
 ***************************************************/
void info_print(const char* str) {printf("[INFO] %s\n", str);}
void warning_print(const char* str) {printf("[WARNING] %s\n", str);}
void error_print(const char* str) {printf("[ERROR] %s\n", str);}


/***************************************************
 *
 * main
 *
 ***************************************************/
int main(int argc, char const *argv[]) {
    ////////////////////////////////////////////////
    // print welcome message
    ////////////////////////////////////////////////
    printf("\n-----------------------------------\n\n");
    printf("%s v%s\n", _APP_NAME_, _VERSION_);
    printf("Password wallet based on Intel SGX for linux.\n\n");
    printf("-----------------------------------\n\n");


    ////////////////////////////////////////////////
    // initialise enclave
    ////////////////////////////////////////////////
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        error_print("Fail to initialize enclave.");
        return 1;
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
        error_print("Fail test ECALL.");
        return 1; 
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
        error_print("Sealing failed.");
        return 1;
    }

    int unsealed;
    status = unseal(
        global_eid, &ecall_status,
        (sgx_sealed_data_t*) sealed_data, sealed_size,
        (uint8_t*) &unsealed, sizeof(unsealed)
    );

    if (status != SGX_SUCCESS || ecall_status != SGX_SUCCESS) {
        error_print("Unsealing failed.");
        return 1;
    }

    sprintf(string_buffer, "Seal round trip success! Receive back: %d", unsealed);
    info_print(string_buffer);


    ////////////////////////////////////////////////
    // read input arguments 
    ////////////////////////////////////////////////
    //read user input and opt for adding new password 
    // or display (cat) password content


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
