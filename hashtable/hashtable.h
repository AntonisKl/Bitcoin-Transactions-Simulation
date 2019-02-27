#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../utils/utils.h"
#include "../bitcoin_tree_list/bitcoin_tree_list.h"

typedef struct Transaction {
    char *senderWalletId, *receiverWalletId, *datetimeS;
    BitcoinList* bitcoinList;
    time_t timestamp;  // millis or sec??
    int amount;
    Transaction* nextTransaction;
} Transaction;

typedef struct TransactionList {
    Transaction* firstTransaction, *lastTransaction;
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
    unsigned int bucketListArraySize, bucketSize;
} HashTable;

// Transaction
Transaction* initTransaction(char* senderWalletId, char* receiverWalletId, char* datetimeS, BitcoinList* bitcoinList);

void freeTransaction(Transaction** transaction);

// Transaction** initTransactionArray(unsigned int size);

// void freeTransactionArray(Transaction*** transactions, unsigned int size);
// end

// Bucket
Bucket* initBucket(unsigned int maxTransactionsNum);

void freeBucketRec(HashTable* hashTable, Bucket** bucket);

Transaction* addTransactionToBucketList(HashTable* hashTable, unsigned int listIndex, Transaction* transaction);

BucketList** initBucketListArray(unsigned int size);

BucketList* initBucketList();

void freeBucketList(HashTable* hashTable, BucketList** bucketList);

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int maxTransactionsNum);
// end

// Hash Table
HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize);

unsigned int hashFunction(HashTable* hashTable, char* walletId);

Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction);
// end

#endif