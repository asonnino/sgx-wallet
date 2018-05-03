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
 * along with SGX-WALLET.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <cstring>

#include "utils.h"
#include "app.h"
#include "wallet.h"
#include "enclave.h"


/**
 * @brief      Prints an info message. 
 *
 */
void info_print(const char* str) {
    printf("[INFO] %s\n", str);
}


/**
 * @brief      Prints a warning message.
 *
 */
void warning_print(const char* str) {
    printf("[WARNING] %s\n", str);
}


/**
 * @brief      Prints an error message. 
 *
 */
void error_print(const char* str) {
    printf("[ERROR] %s\n", str);
}


/**
 * @brief      Prints the wallet's content.
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
 * @brief      Prints an error message correspondig to the
 *             error code.
 *
 */
int is_error(int error_code) {
    char err_message[100];

    // check error case
    switch(error_code) {
        case RET_SUCCESS:
            return 0;

        case ERR_PASSWORD_OUT_OF_RANGE:
            sprintf(err_message, "Password should be at least 8 characters long and at most %d.", MAX_ITEM_SIZE);
            break;

        case ERR_WALLET_ALREADY_EXISTS:
            sprintf(err_message, "Wallet already exists: delete file '%s' first.", WALLET_FILE);
            break;

        case ERR_CANNOT_SAVE_WALLET:
            strcpy(err_message, "Coud not save wallet.");
            break;

        case ERR_CANNOT_LOAD_WALLET:
            strcpy(err_message, "Coud not load wallet.");
            break;

        case ERR_WRONG_MASTER_PASSWORD:
            strcpy(err_message, "Wrong master password."); 
            break;

        case ERR_WALLET_FULL:
            sprintf(err_message, "Wallet full (maximum number of item: %d).", MAX_ITEMS);
            break;

        case ERR_ITEM_DOES_NOT_EXIST: 
            strcpy(err_message, "Item does not exist."); 
            break;

        case ERR_ITEM_TOO_LONG:
            sprintf(err_message, "Item too longth (maximum size: %d).", MAX_ITEM_SIZE); 
            break;

        case ERR_FAIL_SEAL:
            sprintf(err_message, "Fail to seal wallet."); 
            break;

        case ERR_FAIL_UNSEAL:
            sprintf(err_message, "Fail to unseal wallet."); 
            break;

        default:
            sprintf(err_message, "Unknown error."); 
    }

    // print error message
    error_print(err_message);
    return 1;
}


/**
 * @brief      Prints help & usage. 
 *
 */
void show_help() {
	const char* command = "[-h Show this screen] [-v Show version] [-t Run tests] " \
		"[-n master-password] [-p master-password -c new-master-password]" \
		"[-p master-password -a -x items_title -y items_username -z toitems_password]" \
		"[-p master-password -r items_index]";
	printf("\nusage: %s %s\n\n", APP_NAME, command);
    printf("more information: https://github.com/asonnino/sgx-wallet\n\n");
}


/**
 * @brief      Prints the build version. 
 *
 */
void show_version() {
	printf("v%s\n", VERSION);
}



