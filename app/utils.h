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
#ifndef UTIL_H_
#define UTIL_H_

#include "wallet.h"


/**
 * @brief      Prints an info message. 
 *
 * @param[in]  str    The message to print
 *
 * @return     -
 */
void info_print(const char* str);


/**
 * @brief      Prints a warning message. 
 *
 * @param[in]  str    The message to print
 *
 * @return     -
 */
void warning_print(const char* str);


/**
 * @brief      Prints an error message. 
 *
 * @param[in]  str    The message to print
 *
 * @return     -
 */
void error_print(const char* str);


/**
 * @brief      Prints the wallet's content.
 *
 * @param[in]  wallet    The wallet to print out
 *
 * @return     -
 */
void print_wallet(const wallet_t* wallet);


/**
 * @brief      Prints an error message correspondig to the
 *			   error code.
 *
 * @param[in]  ret    The error code
 *
 * @return     -
 */
int is_error(int error_code);


/**
 * @brief      Prints help & usage. 
 *
 * @param      -
 *
 * @return     -
 */
void show_help();


/**
 * @brief      Prints the build version. 
 *
 * @param      -
 *
 * @return     -
 */
void show_version();


#endif // UTIL_H_


