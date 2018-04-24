/*
 *
 *
 */
#include "Enclave_t.h"
#include "string.h"






/***************************************************
 *
 * Defines
 *
 ***************************************************/
#define ERR_TITLE_TOO_LONG 1
#define ERR_USERNAME_TOO_LONG 2
#define ERR_PASSWORD_TOO_LONG 3
#define ERR_SEAL_FAILED 4




/*
 *
 */
int generate_random_number() {
    ocall_debug_print("Processing random number generation...");
    return 42;
}


/*
 *
 */
int add_item(const char* title, const char* username, const char* password) {




	// receive sealed data -- unseal -- check if master_pass -- return unsealed




	/*
	// check title size
	const int title_len = strlen(title)+1;
	if (title_len > MAX_BUF_LEN) {return ERR_TITLE_TOO_LONG;}

	// check username size
	const int username_len = strlen(username)+1;
	if (username_len > MAX_BUF_LEN) {return ERR_USERNAME_TOO_LONG;}

	// check password size
	const int password_len = strlen(password)+1;
	if (password_len > MAX_BUF_LEN) {return ERR_PASSWORD_TOO_LONG;}

	// first create a data struct for the new item
	Item new_item;
	strncpy(new_item.title, title, title_len); 
	strncpy(new_item.username, username, username_len); 
	strncpy(new_item.password, password, password_len); 

	// seal data
	size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(new_item);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    sgx_status_t status = seal( 
        (uint8_t*) &new_item, sizeof(new_item), 
        (sgx_sealed_data_t*) sealed_data, sealed_size
    );
    if (status != SGX_SUCCESS) {return ERR_SEAL_FAILED;}

    ocall_save(sealed_data, sealed_size);



	ocall_debug_print("Username and password copied into the enclave.");
	//char str[20];
	//snprintf(str, 20, "%d", sizeof(new_item));
	//ocall_debug_print(str);
	return 0;
	*/
}


