#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../utils/utils.h"

typedef struct Transaction {
    char *senderWalletId, receiverWalletId, datetimeS;
    time_t timestamp;  // millis or sec??
    int amount;
} Transaction;

typedef struct Bucket {
    Transaction** transactions;                   // array of fixed size
    struct Bucket* nextBucket /*, *prevBucket*/;  // no deletions so prevBucket is not needed
    unsigned int nextIndex;
} Bucket;

typedef struct BucketList {
    Bucket *firstBucket, *lastBucket;
    char* walletId;
    unsigned int size;
} BucketList;

typedef struct HashTable {
    BucketList** bucketLists;  // array of fixed size according to number of walletIds
    char* walletId;
    unsigned int bucketListArraySize, bucketSize;
} HashTable;

// Transaction
Transaction* initTransaction(char* senderWalletId, char* receiverWalletId, char* datetimeS);

void freeTransaction(Transaction** transaction);

Transaction** initTransactionArray(unsigned int size);

void freeTransactionArray(Transaction*** transactions, unsigned int size);
// end

// Bucket
Bucket* initBucket(unsigned int maxTransactionsNum);

void freeBucketRec(HashTable* hashTable, Bucket** bucket);

Transaction* addTransactionToBucketList(HashTable* hashTable, unsigned int listIndex, Transaction* transaction);

BucketList** initBucketListArray(unsigned int size);

BucketList* initBucketList(char* walletId);

void freeBucketList(HashTable* hashTable, BucketList** bucketList);

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int maxTransactionsNum);
// end

// Hash Table
HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize);

unsigned int hashFunction(HashTable* hashTable, char* walletId);

Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction);
// end

#endif