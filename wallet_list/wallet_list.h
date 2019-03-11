#ifndef WALLET_LIST_H
#define WALLET_LIST_H

#include "../utils/utils.h"
#include "../bitcoin_tree_list/bitcoin_tree_list.h"

typedef struct BitcoinList BitcoinList;
typedef struct Transaction Transaction;

typedef struct Wallet {
    char* walletId;
    int balance; // represents dollars
    BitcoinList* bitcoinList; // bitcoins owned by this user
    struct Wallet* nextWallet;
} Wallet;

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

Wallet* findWalletInWalletList(WalletList* walletList, char* walletId);

Wallet* addWalletToWalletList(WalletList* walletList, char* walletId, unsigned int balance, BitcoinList* bitcoinList);

// End

// High level functions

int handleWalletToWalletTransfer(Wallet* senderWallet, Wallet* receiverWallet, Transaction* transaction);

int getCurrentBalanceOfWallet(WalletList* walletList, char* walletId);

// End

#endif