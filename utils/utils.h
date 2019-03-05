#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE
#define __USE_XOPEN
#include <time.h>

#include "../wallet_list/wallet_list.h"
#include "../hashtable/hashtable.h"

#define MAX_WALLET_ID_SIZE 50
#define MAX_TRANSACTION_ID_SIZE 50
#define MAX_DATETIME_SIZE 15
#define MAX_FILE_LINE_SIZE 100
#define MAX_INPUT_SIZE 100
#define MAX_STRING_INT_SIZE 12    // including end of string

// Bitcoin

// typedef struct BitcoinTreeNode {
//     char* walletId;
//     int amount;
//     struct BitcoinTreeNode *receiverNode, *remainingNode;  // remainingNode may be NULL if remaining amount is 0
// } BitcoinTreeNode;

// typedef struct BitcoinTree {
//     BitcoinTreeNode* rootNode;
//     int bitcoinId;
//     unsigned int size;
//     struct BitcoinTree* nextBitcoinTree;
// } BitcoinTree;

// typedef struct BitcoinList {
//     BitcoinTree *firstBitcoinTree;
//     unsigned int size;
// } BitcoinList;

////////////

// typedef struct Wallet {
//     char* walletId;
//     BitcoinList* bitcoinList;
//     struct Wallet* nextWallet;
// } Wallet;

// typedef struct WalletList {
//     Wallet* firstWallet;
//     unsigned int size;
// } WalletList;

typedef struct HashTable HashTable;
typedef struct WalletList WalletList;

void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes);

void handleBitcoinBalancesFile(char* fileName, WalletList** walletList, BitcoinList** bitcoinList, int bitcoinValue);

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, int withTransactionId);

void handleTransactionsFile(char* fileName, HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            int senderHashTableSize, int receiverHashTableSize, int bucketSize);

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList);

time_t datetimeStringToTimeStamp(char* datetimeS);

#endif