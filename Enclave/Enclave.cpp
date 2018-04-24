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
#define MAX_BUF_LEN 100




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
	// check title size
	const int title_len = strlen(title)+1;
	if (title_len > MAX_BUF_LEN) {return 1;}

	// check username size
	const int username_len = strlen(username)+1;
	if (username_len > MAX_BUF_LEN) {return 2;}

	// check password size
	const int password_len = strlen(password)+1;
	if (password_len > MAX_BUF_LEN) {return 3;}

	// seal data
	// first create a data strcut

	ocall_debug_print("Username and password copied into the enclave.");
	return 0;
}


