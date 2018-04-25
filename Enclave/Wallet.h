// ============================================================================
// wallet.h
// 
//
// ============================================================================
#ifndef WALLET_H_
#define WALLET_H_


/***************************************************
 * Defines
 ***************************************************/
#define MAX_BUF_LEN 100
#define MAX_ITEMS 100


/***************************************************
 * Struct
 ***************************************************/
// item
struct Item {
	char  title[MAX_BUF_LEN];
	char  username[MAX_BUF_LEN];
	char  password[MAX_BUF_LEN];
};
typedef struct Item item_t;

// wallet
struct Wallet {
	item_t items[MAX_ITEMS];
	size_t size;
	char master_password[MAX_BUF_LEN];
};
typedef struct Wallet wallet_t;



#endif // WALLET_H_