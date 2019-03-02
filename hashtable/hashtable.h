#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../utils/utils.h"
#include "../bitcoin_tree_list/bitcoin_tree_list.h"

typedef struct BitcoinList BitcoinList; // forward declaration for compilation

typedef struct Transaction {
    char *transactionId, *senderWalletId, *receiverWalletId, *datetimeS;
    BitcoinList* bitcoinList; // bitcoins involved
    time_t timestamp;  // millis or sec??
    int amount;
    struct Transaction* nextTransaction;
} Transaction;

typedef struct TransactionList {
    Transaction *firstTransaction, *lastTransaction;
    unsigned int size;
    char* walletId;
} TransactionList;

typedef struct Bucket {
    TransactionList** transactionLists;
    // Transaction** transactions;                   // array of fixed size
    struct Bucket* nextBucket /*, *prevBucket*/;  // no deletions so prevBucket is not needed
    unsigned int nextIndex;
} Bucket;

typedef struct BucketList {
    Bucket *firstBucket, *lastBucket;
    unsigned int size;
} BucketList;

typedef struct HashTable {
    BucketList** bucketLists;  // array of fixed size according to number of walletIds
    char* walletId;
    unsigned int bucketListArraySize, bucketSize; // bucketSize: number of transaction lists that fit inside a bucket
} HashTable;

typedef enum HashTableType {
    SENDER, RECEIVER
} HashTableType;

// Transaction
Transaction* initTransaction(char* transactionId, char* senderWalletId, char* receiverWalletId, char* datetimeS, BitcoinList* bitcoinList);

void freeTransaction(Transaction** transaction);

TransactionList* initTransactionList(char* walletId);

TransactionList** initTransactionListArray(unsigned int size);

void freeTransactionList(TransactionList** transactionList);

void freeTransactionListArray(TransactionList*** transactionLists, unsigned int size);

// Bucket* findBucketInTransactionList(TransactionList* transactionList, char* name) {
//     if (transactionList == NULL)
//         return NULL;

//     Bucket* curBucket = transactionList->firstTransaction;

//     while (curBucket != NULL) {
//         if (strcmp(curBucket->name, name) == 0) {
//             return curBucket;
//         } else if (strcmp(name, curBucket->name) < 0) {  // no need for searching further since the list is sorted
//             return NULL;
//         }
//         curBucket = curBucket->nextBucket;
//     }
//     return NULL;
// }

Transaction* addTransactionToEndOfTransactionList(TransactionList* transactionList, Transaction* transaction);

// Transaction** initTransactionArray(unsigned int size);

// void freeTransactionArray(Transaction*** transactions, unsigned int size);
// end

// Bucket
Bucket* initBucket(unsigned int bucketSize);

void freeBucketRec(HashTable* hashTable, Bucket** bucket);

Transaction* addTransactionToBucketList(HashTable* hashTable, unsigned int listIndex, Transaction* transaction);

BucketList** initBucketListArray(unsigned int size);

BucketList* initBucketList();

void freeBucketList(HashTable* hashTable, BucketList** bucketList);

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int bucketSize);
// end

// Hash Table
HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize);

unsigned int hashFunction(HashTable* hashTable, char* walletId);

char* getWalletIdByHashTableType(Transaction* transaction, HashTableType type);

Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction, HashTableType hashTableType);
// end

#endif