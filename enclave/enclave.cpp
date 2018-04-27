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

#include "enclave_t.h"
#include "string.h"
#include "debug.h"
#include "enclave.h"
#include "wallet.h"


/**
 *
 *
 */
int ecall_create_wallet(const char* master_password) {

	//
	// OVERVIEW: 
	//	1. check password policy
	//	2. [ocall] abort if wallet already exist
	//	3. create wallet 
	//	4. seal wallet
	//	5. [ocall] save wallet
	//	6. free enclave memory
	//
	//
	sgx_status_t ocall_status;
	int ocall_ret;

	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("CREATING NEW WALLET...");
	#endif


	// 1. check passaword policy
	if (strlen(master_password) < 8 || strlen(master_password)+1 > MAX_ITEM_SIZE) {
		return ERR_PASSWORD_OUT_OF_RANGE;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Password policy successfully checked.");
	#endif


	// 2. abort if wallet already exist
	ocall_status = ocall_is_wallet(&ocall_ret);
	if (ocall_ret != 0) {
		return ERR_WALLET_ALREADY_EXISTS;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] No pre-existing wallets.");
	#endif


	// 3. create new wallet
	wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
	wallet->size = 0;
	strncpy(wallet->master_password, master_password, strlen(master_password)+1); 
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] New wallet successfully created.");
	#endif


	// 4. seal wallet
	// TODO
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[TODO] Seal wallet.");
	#endif


	// 5. save wallet
	ocall_status = ocall_save_tmp(&ocall_ret, wallet, sizeof(wallet_t)); 
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_SAVE_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] New wallet successfully saved.");
	#endif


	// 6. free encalve memory
	free(wallet);
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("WALLET SUCCESSFULLY CREATED.");
	#endif
	return RET_SUCCESS;
}


/**
 *
 *
 */
int ecall_show_wallet(const char* master_password, wallet_t* wallet, size_t wallet_size) {

	//
	// OVERVIEW: 
	//	1. [ocall] load wallet
	// 	2. unseal wallet
	//	3. verify master-password
	//	4. return wallet to app
	//	5. free encalve memory
	//
	//
	sgx_status_t ocall_status;
	int ocall_ret;

	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("RETURNING WALLET TO APP...");
	#endif


	// 1. load wallet
	wallet_t* loaded_wallet = (wallet_t*)malloc(sizeof(wallet_t));
	ocall_status = ocall_load_tmp(&ocall_ret, loaded_wallet, sizeof(wallet_t));
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(loaded_wallet);
		return ERR_CANNOT_LOAD_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Wallet successfully loaded.");
	#endif


	// 2. unseal loaded wallet
	// TODO
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[TODO] Unseal wallet.");
	#endif


	// 3. verify master-password
	if (strcmp(loaded_wallet->master_password, master_password) != 0) {
		free(loaded_wallet);
		return ERR_WRONG_MASTER_PASSWORD;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Master-password successfully verified.");
	#endif


	// 4. return wallet to app
	(* wallet) = *loaded_wallet;
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Wallet successfully saved to buffer.");
	#endif


	// 5. free encalve memory
	free(loaded_wallet);
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("WALLET SUCCESSFULLY RETURNED TO APP.");
	#endif
	return RET_SUCCESS;
}


/**
 *
 *
 */
int ecall_change_master_password(const char* old_password, const char* new_password) {

	//
	// OVERVIEW: 
	//	1. check password policy
	//	2. [ocall] load wallet
	// 	3. unseal wallet
	//	4. verify old password
	//	5. update password
	//	6. seal wallet
	// 	7. [ocall] save sealed wallet
	//	8. free encalve memory
	//
	//
	sgx_status_t ocall_status;
	int ocall_ret;

	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("CHANGING MASTER PASSWORD...");
	#endif


	// 1. check passaword policy
	if (strlen(new_password) < 8 || strlen(new_password)+1 > MAX_ITEM_SIZE) {
		return ERR_PASSWORD_OUT_OF_RANGE;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Password policy successfully checked.");
	#endif


	// 2. load wallet
	wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
	ocall_status = ocall_load_tmp(&ocall_ret, wallet, sizeof(wallet_t));
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_LOAD_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Wallet successfully loaded.");
	#endif


	// 3. unseal wallet
	// TODO
	// seal-unseal as .h (instead of EDL -- no need to interface with app)
	ocall_debug_print("[TODO] Unseal wallet.");


	// 4. verify master-password
	if (strcmp(wallet->master_password, old_password) != 0) {
		free(wallet);
		return ERR_WRONG_MASTER_PASSWORD;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Master-password successfully verified.");
	#endif


	// 5. update password
	strncpy(wallet->master_password, new_password, strlen(new_password)+1); 
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Successfully updated master-password.");
	#endif


	// 6. seal wallet
	// TODO
	ocall_debug_print("[TODO] Seal wallet.");


	// 7. save wallet
	ocall_status = ocall_save_tmp(&ocall_ret, wallet, sizeof(wallet_t)); 
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_SAVE_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Wallet successfully saved.");
	#endif


	// 6. free encalve memory
	free(wallet);
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("MASTER PASSWORD SUCCESSFULLY CHANGED.");
	#endif
	return RET_SUCCESS;
}


/**
 *
 *
 */
int ecall_add_item(const char* master_password, const item_t* item, const size_t item_size) {

	//
	// OVERVIEW: 
	//	1. [ocall] load wallet
	//	2. unseal wallet
	//	3. verify master-password
	//	4. check input length
	//	5. add item to the wallet
	//	6. seal wallet
	//	7. [ocall] save sealed wallet
	//	8. free encalve memory
	//
	//
	sgx_status_t ocall_status;
	int ocall_ret;

	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("ADDING ITEM TO THE WALLET...");
	#endif


	// 1. load wallet
	wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
	ocall_status = ocall_load_tmp(&ocall_ret, wallet, sizeof(wallet_t));
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_LOAD_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Wallet successfully loaded.");
	#endif


	// 2. unseal wallet
	// TODO
	ocall_debug_print("[TODO] Unseal wallet.");


	// 3. verify master-password
	if (strcmp(wallet->master_password, master_password) != 0) {
		free(wallet);
		return ERR_WRONG_MASTER_PASSWORD;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Master-password successfully verified.");
	#endif


	// 4. check input length
	if (strlen(item->title)+1 > MAX_ITEM_SIZE ||
		strlen(item->username)+1 > MAX_ITEM_SIZE ||
		strlen(item->password)+1 > MAX_ITEM_SIZE
	) {
		free(wallet);
		return ERR_ITEM_TOO_LONG; 
    }
    #ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Item successfully verified.");
	#endif


	// 5. add item to the wallet
	size_t wallet_size = wallet->size;
	if (wallet_size >= MAX_ITEMS) {
		free(wallet);
		return ERR_WALLET_FULL;
	}
	wallet->items[wallet_size] = *item;
	++wallet->size;
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Item successfully added.");
	#endif


	// 6. seal wallet
	// TODO
	ocall_debug_print("[TODO] Seal wallet.");


	// 7. save wallet
	ocall_status = ocall_save_tmp(&ocall_ret, wallet, sizeof(wallet_t)); 
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_SAVE_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Wallet successfully saved.");
	#endif


	// 8. free encalve memory
	free(wallet);
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("ITEM SUCCESSFULLY ADDED TO THE WALLET.");
	#endif
	return RET_SUCCESS;
}


/**
 *
 *
 */
int ecall_remove_item(const char* master_password, const int index) {

	//
	// OVERVIEW: 
	//	1. check index bounds
	//	2. [ocall] load wallet
	//	3. unseal wallet
	//	4. verify master-password
	//	5. remove item from the wallet
	//	6. seal wallet
	//	7. [ocall] save sealed wallet
	//	8. free encalve memory
	//
	//
	sgx_status_t ocall_status;
	int ocall_ret;

	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("REMOVING ITEM FROM THE WALLET...");
	#endif


	// 1. check index bounds
	if (index < 0 || index >= MAX_ITEMS) {
		return ERR_ITEM_DOES_NOT_EXIST;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Successfully checked index bounds.");
	#endif


	// 2. load wallet
	wallet_t* wallet = (wallet_t*)malloc(sizeof(wallet_t));
	ocall_status = ocall_load_tmp(&ocall_ret, wallet, sizeof(wallet_t));
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_LOAD_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Wallet successfully loaded.");
	#endif


	// 3. unseal wallet
	// TODO
	ocall_debug_print("[TODO] Unseal wallet.");


	// 4. verify master-password
	if (strcmp(wallet->master_password, master_password) != 0) {
		free(wallet);
		return ERR_WRONG_MASTER_PASSWORD;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[ok] Master-password successfully verified.");
	#endif


	// 5. remove item from the wallet
	size_t wallet_size = wallet->size;
	if (index >= wallet_size) {
		free(wallet);
		return ERR_ITEM_DOES_NOT_EXIST;
	}
	for (int i = index; i < wallet_size-1; ++i) {
		wallet->items[i] = wallet->items[i+1];
	}
	--wallet->size;
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Item successfully removed.");
	#endif


	// 6. seal wallet
	// TODO
	ocall_debug_print("[TODO] Seal wallet.");


	// 7. save wallet
	ocall_status = ocall_save_tmp(&ocall_ret, wallet, sizeof(wallet_t)); 
	if (ocall_ret != 0 || ocall_status != SGX_SUCCESS) {
		free(wallet);
		return ERR_CANNOT_SAVE_WALLET;
	}
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("[OK] Wallet successfully saved.");
	#endif


	// 8. free encalve memory
	free(wallet);
	#ifdef ENCLAVE_DEBUG
		ocall_debug_print("ITEM SUCCESSFULLY REMOVED FROM THE WALLET.");
	#endif
	return RET_SUCCESS;
}

