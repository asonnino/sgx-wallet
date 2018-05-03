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
#include "enclave_t.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"

#include "wallet.h"
#include "sealing.h"


/**
 * @brief      Seals the wallet given into the sgx_sealed_data_t structure
 *             given.
 *
 */
sgx_status_t seal_wallet(const wallet_t* wallet, sgx_sealed_data_t* sealed_data, size_t sealed_size) {
    return sgx_seal_data(0, NULL, sizeof(wallet_t), (uint8_t*)wallet, sealed_size, sealed_data);
}


/**
 * @brief      Unseal the sealed_data given into c-string
 *
 */
sgx_status_t unseal_wallet(const sgx_sealed_data_t* sealed_data, wallet_t* plaintext, uint32_t plaintext_size) {
    return sgx_unseal_data(sealed_data, NULL, NULL, (uint8_t*)plaintext, &plaintext_size);
}

