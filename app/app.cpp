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
#include "enclave_u.h"
#include "sgx_urts.h"

#include <cstring>
#include <fstream>
#include <getopt.h>

#include "app.h"
#include "utils.h"
#include "ocalls.h"
#include "debug.h"
#include "wallet.h"
#include "enclave.h"
#include "test.h"

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
/*
 *
 */
int main(int argc, char** argv) {
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
    // read input arguments 
    ////////////////////////////////////////////////
    const char* options = "hp:nc:sa:t";
    char err_message[100];
    int opt;
    int h_flag=0, n_flag=0, s_flag=0, t_flag=0;
    char *p_value, *c_value, *a_value;
    
    // read user input
    while ((opt = getopt (argc, argv, options)) != -1) {
        switch (opt) {
            case 'h': // help
                h_flag = 1;
                break;

            case 'p': // master-password
                p_value = optarg;
                break;

            case 'n': // create new wallet
                n_flag = 1;
                break;

            case 'c': // change master-password
                c_value = optarg;
                break;

            case 's': // show wallet
                s_flag = 1;
                break;

            case 'a': // add item
                a_value = optarg;
                break;

            case 't': // run tests
                t_flag = 1;
                break;

            case '?':
                if (optopt == 'p' || optopt == 'c' || optopt == 'a') {
                    sprintf(err_message, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint(optopt)) {
                    sprintf(err_message, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    sprintf(err_message, "Unknown option character `\\x%x'.\n",optopt);
                }
                error_print(err_message);
                break;

            default:
                error_print("Unknown option.");
        }
    }

    // perform actions
    if (h_flag) {
        display_help();
    }
    else if(t_flag) {
        info_print("Running tests...");
        if (test(eid) != 0) {error_print("One or more tests failed.");}
        else {info_print("All tests successfully passed.");}
    }
    else if(n_flag && p_value != NULL) {
        // TODO
        warning_print("[TODO] call create wallet.");
    }
    else if(s_flag && p_value != NULL) {
        // TODO
        warning_print("[TODO] call show wallet.");
    }
    else if (p_value != NULL && c_value != NULL) {
        // TODO
        warning_print("[TODO] call change master-password.");
    }
    else if (p_value != NULL && a_value != NULL) {
        // TODO
        warning_print("[TODO] call add item.");
    }


    ////////////////////////////////////////////////
    // destroy enclave
    ////////////////////////////////////////////////
    enclave_status = sgx_destroy_enclave(eid);
    if(enclave_status != SGX_SUCCESS) {
        error_print("Fail to destroy enclave."); 
        return -1;
    }
    info_print("Enclave successfully destroyed.");


    ////////////////////////////////////////////////
    // exit success
    ////////////////////////////////////////////////
    info_print("Program exit success.");
    return 0;
}
