#ifndef WALLET_LIST_H
#define WALLET_LIST_H

#include "../bitcoin_tree_list/bitcoin_tree_list.h"
#include "../utils/utils.h"

typedef struct BitcoinList BitcoinList;  // forward declaration to fix compiling issues
typedef struct Transaction Transaction;  // forward declaration to fix compiling issues

typedef struct Wallet {
    char* walletId;
    int balance;               // represents dollars
    BitcoinList* bitcoinList;  // bitcoin list nodes of bitcoin trees of bitcoins owned by this user
    struct Wallet* nextWallet;
} Wallet;

// WalletList is sorted by walletId in ascending order
typedef struct WalletList {
    Wallet* firstWallet;
    unsigned int size;
} WalletList;

// Wallet

Wallet* initWallet(char* walletId, unsigned int balance, BitcoinList* bitcoinList);

void freeWallet(Wallet** wallet);

void freeWalletRec(Wallet** wallet);

// End

// Wallet List

WalletList* initWalletList();

void freeWalletList(WalletList** walletList);

// find wallet in wallet list by taking advantage of sorting order
Wallet* findWalletInWalletList(WalletList* walletList, char* walletId);

// add wallet to wallet list by maintaining the sorting order
Wallet* addWalletToWalletList(WalletList* walletList, char* walletId, unsigned int balance, BitcoinList* bitcoinList);

// End

// High level functions

// tries to make a transaction by transfering amount of dollars and copying/transfering bitcoin trees from sender wallet to receiver wallet
// and updates the bitcoin trees of the bitcoins that are used in the transaction
// returns 0 if all went ok
// returns -1 if transaction could not be made
int handleWalletToWalletTransfer(Wallet* senderWallet, Wallet* receiverWallet, Transaction* transaction);

// finds wallet with id walletId and returns its balance
int getCurrentBalanceOfWallet(WalletList* walletList, char* walletId);

// End

#endif