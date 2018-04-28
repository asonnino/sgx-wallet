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
#ifndef UTIL_H_
#define UTIL_H_

#include "wallet.h"


/**
 *
 */
void info_print(const char* str);

/**
 *
 */
void warning_print(const char* str);

/**
 *
 */
void error_print(const char* str);

/**
 *
 */
void print_wallet(const wallet_t* wallet);

/**
 *
 */
int is_error(int ret);

/**
 *
 */
void show_help();

/**
 *
 */
void show_version();


#endif // UTIL_H_


