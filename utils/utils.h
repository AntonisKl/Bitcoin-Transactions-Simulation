#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE
#define __USE_XOPEN
#include <time.h>

#define MAX_WALLET_ID_SIZE 50
#define MAX_DATETIME_SIZE 15

// Bitcoin

typedef struct BitcoinTreeNode {
    char* walletId;
    int amount;
    struct BitcoinTreeNode *receiverNode, *remainingNode;  // remainingNode may be NULL if remaining amount is 0
} BitcoinTreeNode;

typedef struct BitcoinTree {
    BitcoinTreeNode* rootNode;
    int bitcoinId;
    unsigned int size;
    struct BitcoinTree* nextBitcoinTree;
} BitcoinTree;

typedef struct BitcoinList {
    BitcoinTree *firstBitcoinTree;
    unsigned int size;
} BitcoinList;

////////////

typedef struct Wallet {
    char* walletId;
    BitcoinList* bitcoinList;
    struct Wallet* nextWallet;
} Wallet;

typedef struct WalletList {
    Wallet* firstWallet;
    unsigned int size;
} WalletList;

#endif