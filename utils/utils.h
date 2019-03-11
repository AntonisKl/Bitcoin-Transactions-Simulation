#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

typedef struct HashTable HashTable;
typedef struct WalletList WalletList;

void printError(char* s);
char stringIsNumber(char* s);

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

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, char withTransactionId, time_t* lastTransactionTimestamp);

void handleTransactionsFile(char* fileName, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            time_t* lastTransactionTimestamp);

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp);

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList);

#endif