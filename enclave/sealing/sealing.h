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
#ifndef SEALING_H_
#define SEALING_H_

#include "sgx_trts.h"
#include "sgx_tseal.h"

#include "wallet.h"

/**
 *
 */
sgx_status_t seal(const uint8_t* plaintext, const size_t plaintext_len, sgx_sealed_data_t* sealed_data, size_t sealed_size);
sgx_status_t seal_wallet(const wallet_t* plaintext, sgx_sealed_data_t* sealed_data, size_t sealed_size);

/**
 *
 */
sgx_status_t unseal(const sgx_sealed_data_t* sealed_data, const size_t sealed_size, uint8_t* plaintext, uint32_t plaintext_len);
sgx_status_t unseal_wallet(const sgx_sealed_data_t* sealed_data, wallet_t* plaintext, uint32_t plaintext_size);

#endif // SEALING_H_


