#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../bitcoin_tree_list/bitcoin_tree_list.h"
#include "../utils/utils.h"

typedef struct BitcoinList BitcoinList;  // forward declaration for compilation

typedef struct Transaction {
    char *transactionId, *senderWalletId, *receiverWalletId, *datetimeS;  // datetimeS: valid datetime string as it was read
    time_t timestamp;                                                     // unix timestamp in seconds
    int amount;                                                           // dollars
} Transaction;

typedef struct TransactionListNode {
    Transaction* transaction;
    struct TransactionListNode* nextTransactionListNode;
} TransactionListNode;

typedef struct TransactionList {
    TransactionListNode *firstTransactionListNode, *lastTransactionListNode;
    unsigned int size;
    char* walletId;  // walletId: walletId of wallet that is involved in the transaction list
} TransactionList;

typedef struct Bucket {
    TransactionList** transactionLists;  // transactionLists: array of transaction lists
    struct Bucket* nextBucket;           // no deletions so prevBucket is not needed
    unsigned int nextIndex;              // nextIndex: index of transactionLists array in which we will insert the next transaction list that will be created
} Bucket;

typedef struct BucketList {
    Bucket *firstBucket, *lastBucket;
    unsigned int size;
} BucketList;

typedef struct HashTable {
    BucketList** bucketLists;                      // array of bucket lists
    unsigned int bucketListArraySize, bucketSize;  // bucketListArraySize: size of bucket lists array
                                                   // bucketSize: number of transaction lists that fit inside a bucket
} HashTable;

typedef enum HashTableType {
    SENDER,
    RECEIVER
} HashTableType;

// Transaction and Transaction List

Transaction* initTransaction(char* transactionId, int amount, char* senderWalletId, char* receiverWalletId, char* datetimeS);

void freeTransaction(Transaction** transaction);

TransactionListNode* initTransactionListNode(Transaction* transaction);

void freeTransactionListNodeRec(TransactionListNode** transactionListNode, char shouldFreeTransactions);

TransactionList* initTransactionList(char* walletId);

TransactionList** initTransactionListArray(unsigned int size);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeTransactionList(TransactionList** transactionList, char shoudFreeTransactions);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeTransactionListArray(TransactionList*** transactionLists, unsigned int size, char shoudFreeTransactions);

// adds transaction by pointer to end of transactionList
TransactionListNode* addTransactionListNodeToEndOfTransactionList(TransactionList* transactionList, Transaction* transaction);  // add by pointer

// adds transaction by pointer to transactionList by keeping the ascending sorting order of the transactions by timestamp
TransactionListNode* addTransactionListNodeToTransactionListSorted(TransactionList* transactionList, Transaction* transaction);

// prints transactions of transactionList that have date > date1 && date < date2 && time > time1 && time < time2 (if some values are NULL they are ignored)
// the printed transactions are in ascending sorting order by timestamp
// also, it prints the sum of the amount of dollars that are involved in the transactions of the transactionList
void printTransactionsFromTransactionList(TransactionList* transactionList, char* time1, char* date1, char* time2, char* date2, char findEarnings, time_t timestamp1, time_t timestamp2);

// prints transactions of transactionList in ascending sorting order by timestamp
void printTransactionsOfTransactionListSimple(TransactionList* transactionList);

// End

// Bucket and Bucket List

Bucket* initBucket(unsigned int bucketSize);

// free each bucket of a bucket list recursively
// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeBucketRec(HashTable* hashTable, Bucket** bucket, char shoudFreeTransactions);

BucketList** initBucketListArray(unsigned int size);

BucketList* initBucketList();

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeBucketList(HashTable* hashTable, BucketList** bucketList, char shoudFreeTransactions);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeBucketListArray(HashTable* hashTable, BucketList*** bucketLists, unsigned int size, char shoudFreeTransactions);

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int bucketSize);

// End

// Hash Table

HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize);

void freeHashTable(HashTable** hashTable, char shoudFreeTransactions);

// djb2 hash function that generates a number in [0, hash table's size - 1] according to walletId
unsigned int hashFunction(HashTable* hashTable, char* walletId);

// returns receiver's or sender's wallet id according to type
char* getWalletIdByHashTableType(Transaction* transaction, HashTableType type);

// inserts transaction to hash table according to the result of the hash function
// if hashTableType == RECEIVER: receiver's wallet id is hashed
// if hashTableType == SENDER: sender's wallet id is hashed
Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction, HashTableType hashTableType);

// searchs serially for a transaction in hash table
// returns NULL if no transaction was found
TransactionListNode* findTransactionListNodeInHashTable(HashTable* hashTable, char* transactionId);

// searchs for a transaction list with wallet id walletId in hash table
// returns NULL if no transaction list was found
TransactionList* findTransactionListInHashTable(HashTable* hashTable, char* walletId);

// End

#endif