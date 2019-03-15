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
typedef struct BitcoinList BitcoinList;

// prints the string s in red color
void printError(char* s);

// raises SIGINT and exits
void raiseIntAndExit(int num);

// returns 0 if string is not a number
// returns 1 if it is a number
char stringIsNumber(char* s);

// removes all space characters from str
void removeSpaces(char* str);

// frees a string and makes it = NULL
void freeString(char** s);

// converts a time string to hours and minutes (considers that timeS is a valid time string)
void timeStringToHourMinute(char* timeS, int* hour, int* minute);

// returns 0 if timeS is a valid time
// returns -1 if timeS is an invalid time
char validateTimeS(char* timeS);

// returns -1 if time1 is before time2
// returns 1 if time1 is after time2
// returns 0 if time1 is equal with time2
char compareTimesS(char* time1, char* time2);  // considers time string valid

// converts a date string to days, months and years (considers that dateS is a valid date string)
void dateStringToDayMonthYear(char* dateS, int* day, int* month, int* year);

// returns 0 if dateS is a valid date
// returns -1 if dateS is an invalid date
char validateDateS(char* dateS);

// returns -1 if date1 is before date2
// returns 1 if date1 is after date2
// returns 0 if date1 is equal with date2
char compareDatesS(char* date1, char* date2);

// converts a datetime string to unix timestamp
time_t datetimeStringToTimeStamp(char* datetimeS);

// converts a unix timestamp to datetime string
void timestampToDatetimeString(time_t timestamp, char (*datetimeS)[MAX_DATETIME_SIZE]);

// handles execution arguments that are given as input
void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes);

// reads a file that contains bitcoin balances, initializes and fills walletList and bitcoinList with valid data
// bitcoinList: the main list that contains all bitcoins
void handleBitcoinBalancesFile(char* fileName, WalletList* walletList, BitcoinList* bitcoinList, int bitcoinValue);

// validates a transaction string, makes the transaction if possible and updates senderHashTable and receiverHashTable appropriately
// transactionS: the string of a single transaction
// withTransactionId: - is 0 if transactionS does not contain a transaction id (e.g. when ./requestTransaction is given as command)
//                    - is 1 if transactionS contains a transaction id
void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, char withTransactionId, time_t* lastTransactionTimestamp);

// reads a file that contains transactions and fills senderHashTable and receiverHashTable with valid data
// lastTransactionTimestamp: the timestamp of the last succesful transaction. This is updated every time that a transaction is succesfully made
void handleTransactionsFile(char* fileName, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            time_t* lastTransactionTimestamp, char withTransactionId);

// handles user input until the program is exited with ./exit or the process is killed/interrupted
// bitcoinList: the main list that contains all bitcoins
// lastTransactionTimestamp: the timestamp of the last succesful transaction. This is updated every time that a transaction is succesfully made
void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp);

// frees the allocated memory and avoids double free of transactions and bitcoin trees
void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList);

#endif
