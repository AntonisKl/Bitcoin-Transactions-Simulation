#include "hashtable.h"

//////////////////////////////////////////////////////////////////////////////// START OF TRANSACTION ///////////////////////////////////////////////////////////////

Transaction* initTransaction(char* senderWalletId, char* receiverWalletId, char* datetimeS) {
    Transaction* transaction = malloc(sizeof(Transaction));

    transaction->senderWalletId = malloc(MAX_WALLET_ID_SIZE);
    strcpy(transaction->senderWalletId, senderWalletId);

    transaction->receiverWalletId = malloc(MAX_WALLET_ID_SIZE);
    strcpy(transaction->receiverWalletId, receiverWalletId);

    transaction->datetimeS = malloc(MAX_DATETIME_SIZE);
    strcpy(transaction->datetimeS, datetimeS);

    struct tm timeStruct;
    time_t timestamp;
    if (strptime(datetimeS, "%d-%m-%Y %H:%M", &timeStruct) != NULL) {
        timestamp = mktime(&timeStruct);
    } else {
        perror("time convert error");
    }

    transaction->timestamp = timestamp;

    return transaction;
}

void freeTransaction(Transaction** transaction) {
    free((*transaction)->senderWalletId);
    (*transaction)->senderWalletId = NULL;
    free((*transaction)->receiverWalletId);
    (*transaction)->receiverWalletId = NULL;
    free((*transaction)->datetimeS);
    (*transaction)->datetimeS = NULL;
    free((*transaction));
    (*transaction) = NULL;

    return;
}

Transaction** initTransactionArray(unsigned int size) {
    Transaction** transactions = (Transaction**)malloc(size * sizeof(Transaction*));
    for (int i = 0; i < size; i++) {
        transactions[i] = NULL;
    }

    return transactions;
}

void freeTransactionArray(Transaction*** transactions, unsigned int size) {
    for (int i = 0; i < size; i++) {
        if ((*transactions[i]) != NULL)
            freeTransaction(transactions[i]);
    }

    free((*transactions));
    (*transactions) = NULL;
    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF TRANSACTION ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF BUCKET ///////////////////////////////////////////////////////////////

Bucket* initBucket(unsigned int maxTransactionsNum) {
    Bucket* bucket = (Bucket*)malloc(sizeof(Bucket));
    bucket->transactions = initTransactionArray(maxTransactionsNum);
    bucket->nextIndex = 0;
    // bucket->name = (char*)malloc(PATH_MAX);
    // strcpy(bucket->name, name);
    bucket->nextBucket = NULL;
    // bucket->prevNode = NULL;

    return bucket;
}

void freeBucketRec(HashTable* hashTable, Bucket** bucket) {
    if ((*bucket) == NULL)
        return;

    freeBucketRec(hashTable, &((*bucket)->nextBucket));

    freeTransactionArray((*bucket)->transactions, hashTable->bucketSize);

    // free((*bucket)->name);
    // (*bucket)->name = NULL;
    free(*bucket);
    (*bucket) = NULL;
    return;
}

Transaction* addTransactionToBucketList(HashTable* hashTable, unsigned int listIndex, Transaction* transaction) {
    Bucket* lastBucket = hashTable->bucketLists[listIndex]->lastBucket;

    if (lastBucket->nextIndex >= hashTable->bucketSize) {
        lastBucket = addBucketToEndOfBucketList(hashTable->bucketLists[listIndex], hashTable->bucketSize);  // maybe add transaction with index of bucketList
    }

    lastBucket->transactions[lastBucket->nextIndex] = initTransaction(transaction->senderWalletId, transaction->receiverWalletId, transaction->datetimeS);
    lastBucket->nextIndex++;

    return lastBucket->transactions[lastBucket->nextIndex - 1];
}

//////////////////////////////////////////////////////////////////////////////// END OF BUCKET ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF BUCKET LIST ///////////////////////////////////////////////////////////////

BucketList** initBucketListArray(unsigned int size) {
    BucketList** bucketLists = (BucketList**)malloc(size * sizeof(BucketList*));
    for (int i = 0; i < size; i++) {
        bucketLists[i] = NULL;
    }

    return bucketLists;
}

BucketList* initBucketList(char* walletId) {
    BucketList* bucketList = (BucketList*)malloc(sizeof(BucketList));
    bucketList->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    strcpy(bucketList->walletId, walletId);

    bucketList->size = 0;

    bucketList->firstBucket = NULL;
    bucketList->lastBucket = NULL;

    return bucketList;
}

void freeBucketList(HashTable* hashTable, BucketList** bucketList) {
    if (bucketList == NULL)
        return;
    // Bucket* curBucket = (*bucketList)->firstBucket;

    freeBucketRec(hashTable, (*bucketList)->firstBucket);

    // while (curBucket != NULL) {
    //     // free(curBucket->name);
    //     // curBucket->name = NULL;

    //     if (curBucket->nextBucket != NULL) {
    //         curBucket = curBucket->nextBucket;
    //         freeBucket()
    //         // free(curBucket->prevNode);

    //         // curBucket->prevNode = NULL;
    //     } else {
    //         free(curBucket);
    //         curBucket = NULL;
    //     }
    // }

    free((*bucketList)->walletId);
    (*bucketList)->walletId = NULL;

    free(*bucketList);
    (*bucketList) = NULL;

    return;
}

// Bucket* findBucketInBucketList(BucketList* bucketList, char* name) {
//     if (bucketList == NULL)
//         return NULL;

//     Bucket* curBucket = bucketList->firstBucket;

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

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int maxTransactionsNum) {
    // if (bucketList->size == 0) {
    //     bucketList->firstBucket = initBucket(name);

    //     bucketList->size++;
    //     printf("Inserted |%s| to BucketList\n\n", name);
    //     return bucketList->firstBucket;
    // } else {
    //     Bucket* curBucket = bucketList->firstBucket;

    //     if (strcmp(name, curBucket->name) < 0) {
    //         // insert at start
    //         Bucket* bucketToInsert = initBucket(name);
    //         bucketToInsert->nextBucket = curBucket;

    //         curBucket->prevNode = bucketToInsert;
    //         bucketList->firstBucket = bucketToInsert;
    //         bucketList->size++;
    //         printf("Inserted |%s| to BucketList\n\n", name);
    //         return bucketList->firstBucket;
    //     }
    //     while (curBucket != NULL) {
    //         if (curBucket->nextBucket != NULL) {
    //             if (strcmp(name, curBucket->nextBucket->name) < 0) {
    //                 Bucket* bucketToInsert = initBucket(name);
    //                 bucketToInsert->prevNode = curBucket;
    //                 bucketToInsert->nextBucket = curBucket->nextBucket;

    //                 curBucket->nextBucket->prevNode = bucketToInsert;
    //                 curBucket->nextBucket = bucketToInsert;
    //                 bucketList->size++;
    //                 printf("Inserted |%s| to BucketList\n\n", name);
    //                 return curBucket->nextBucket;
    //             }
    //         } else {
    //             // insert at the end
    //             curBucket->nextBucket = initBucket(name);
    //             curBucket->nextBucket->prevNode = curBucket;

    //             bucketList->size++;
    //             printf("Inserted |%s| to BucketList\n\n", name);
    //             return curBucket->nextBucket;
    //         }

    //         curBucket = curBucket->nextBucket;
    //     }
    // }

    if (bucketList->size == 0) {
        bucketList->firstBucket = initBucket(maxTransactionsNum);
        bucketList->lastBucket = bucketList->firstBucket;
        bucketList->size++;
        return bucketList->firstBucket;
    } else {
        bucketList->lastBucket->nextBucket = initBucket(maxTransactionsNum);
        bucketList->lastBucket = bucketList->lastBucket->nextBucket;
        bucketList->size++;
        return bucketList->lastBucket;
    }

    return NULL;  // not normal behavior
}

// int deleteBucketFromBucketList(BucketList* bucketList, char* name) {
//     if (bucketList == NULL)
//         return -1;
//     Bucket* bucketToDelete = findBucketInBucketList(bucketList, name);

//     if (bucketToDelete == NULL) {
//         printf("Name node with name %s not found in names' list\n", name);
//         return -1;
//     }

//     if (strcmp(bucketToDelete->name, bucketList->firstBucket->name) == 0) {
//         bucketList->firstBucket = bucketToDelete->nextBucket;
//     }
//     if (bucketToDelete->prevNode != NULL) {
//         bucketToDelete->prevNode->nextBucket = bucketToDelete->nextBucket;
//     }
//     if (bucketToDelete->nextBucket != NULL) {
//         bucketToDelete->nextBucket->prevNode = bucketToDelete->prevNode;
//     }

//     freeBucket(&bucketToDelete);
//     bucketList->size--;

//     return 0;
// }

//////////////////////////////////////////////////////////////////////////////// END OF BUCKET LIST ///////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////// START OF HASH TABLE ///////////////////////////////////////////////////////////////

HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->bucketLists = initBucketListArray(bucketListArraySize);
    hashTable->bucketListArraySize = bucketListArraySize;
    hashTable->bucketSize = bucketSize;

    return hashTable;
}

unsigned int hashFunction(HashTable* hashTable, char* walletId) {
    // djb2 hash function
    unsigned long hash = 5381;
    int c;

    while (c = *walletId++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % hashTable->bucketListArraySize;
}

Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction) {
    unsigned int index = hashFunction(hashTable, transaction->senderWalletId);  // implies that the sender is the right value to hash for this hash table

    if (hashTable->bucketLists[index] == NULL) {
        hashTable->bucketLists[index] = initBucketList(transaction->senderWalletId);
    }

    return addTransactionToBucketList(hashTable, index, transaction);
}

//////////////////////////////////////////////////////////////////////////////// END OF HASH TABLE ///////////////////////////////////////////////////////////////