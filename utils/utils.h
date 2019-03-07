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
#define MAX_DATETIME_SIZE 17
#define MAX_BIG_DATETIME_SIZE 27
#define MAX_FILE_LINE_SIZE 100
#define MAX_INPUT_SIZE 500
#define MAX_STRING_INT_SIZE 12    // including end of string
#define MAX_TIME_SIZE 6
#define MAX_DATE_SIZE 11

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

void removeSpaces(char* str);

void timeStringToHourMinute(char* timeS, int* hour, int* minute);

char validateTimeS(char* timeS);

char compareTimesS(char* time1, char* time2);  // considers time string valid

void dateStringToDayMonthYear(char* timeS, int* day, int* month, int* year);

char validateDateS(char* dateS);

char compareDatesS(char* date1, char* date2);

time_t datetimeStringToTimeStamp(char* datetimeS);
void timestampToDatetimeString(time_t timestamp, char (*datetimeS)[MAX_DATETIME_SIZE]);

void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes);

void handleBitcoinBalancesFile(char* fileName, WalletList** walletList, BitcoinList** bitcoinList, int bitcoinValue);

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, int withTransactionId, time_t* lastTransactionTimestamp);

void handleTransactionsFile(char* fileName, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            time_t* lastTransactionTimestamp);

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp);

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList);

#endif