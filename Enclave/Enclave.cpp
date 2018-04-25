// ============================================================================
// enclave.cpp
// 
//
// ============================================================================
#include "Enclave_t.h"
#include "string.h"


/**
 *
 */
int generate_random_number() {
    ocall_debug_print("Processing random number generation...");
    return 42;
}


/**
 *
 *
 */
int create_wallet(const char* master_password, sgx_sealed_data_t* sealed_data, size_t sealed_size) {

	//
	// TODO: 
	// 	1. create wallet
	//	2. seal wallet
	//	3. return sealed wallet
	//
	//

	return 0;
}


/**
 *
 *
 */
int show_wallet(const char* master_password, const sgx_sealed_data_t* sealed_data, const size_t sealed_size,
	wallet_t* wallet, size_t wallet_size) {

	//
	// TODO: 
	// 	1. unseal wallet
	//	2. verify master-password
	//	3. return wallet
	//
	//

	return 0;
}


/**
 *
 *
 */
int change_wallet_password(const char* old_password, const char* new_password, sgx_sealed_data_t* sealed_data, 
	size_t sealed_size) {

	//
	// TODO: 
	// 	1. unseal wallet
	//	2. verify old password
	//	3. update password
	//	4. seal wallet
	//  5. return sealed wallet
	//
	//

	return 0;
}


/**
 *
 *
 */
int add_item(const char* master_password, sgx_sealed_data_t* sealed_data, size_t sealed_size,
	const item_t* item, const size_t item_size) {

	//
	// TODO: 
	// 	1. unseal wallet
	//	2. verify master-password
	//	3. add item to the wallet
	//	4. seal wallet
	//	5. return new sealed wallet
	//
	//

	return 0;
}


/**
 *
 *
 */
int remove_item(const char* master_password, sgx_sealed_data_t* sealed_data, size_t sealed_size, const int index) {

	//
	// TODO: 
	// 	1. unseal wallet
	//	2. verify master-password
	//	3. remove item to the wallet
	//	4. seal wallet
	//	5. return new sealed wallet
	//
	//

	return 0;
}

