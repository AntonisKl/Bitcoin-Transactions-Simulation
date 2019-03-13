#include "hashtable.h"

//////////////////////////////////////////////////////////////////////////////// START OF TRANSACTION ///////////////////////////////////////////////////////////////

Transaction* initTransaction(char* transactionId, int amount, char* senderWalletId, char* receiverWalletId, char* datetimeS) {  // bitcoinList will be already initialized
    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    transaction->amount = amount;

    transaction->transactionId = (char*)malloc(strlen(transactionId) + 1);
    strcpy(transaction->transactionId, transactionId);

    transaction->senderWalletId = (char*)malloc(strlen(senderWalletId) + 1);
    strcpy(transaction->senderWalletId, senderWalletId);

    transaction->receiverWalletId = (char*)malloc(strlen(receiverWalletId) + 1);
    strcpy(transaction->receiverWalletId, receiverWalletId);

    transaction->datetimeS = (char*)malloc(strlen(datetimeS) + 1);
    strcpy(transaction->datetimeS, datetimeS);

    transaction->timestamp = datetimeStringToTimeStamp(datetimeS);

    transaction->nextTransaction = NULL;

    return transaction;
}

void freeTransaction(Transaction** transaction) {
    if ((*transaction) == NULL)
        return;

    free((*transaction)->transactionId);
    (*transaction)->transactionId = NULL;
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

void freeTransactionRec(Transaction** transaction) {
    if ((*transaction) == NULL)
        return;

    freeTransactionRec(&((*transaction)->nextTransaction));

    freeTransaction(transaction);
    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF TRANSACTION ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF TRANSACTION LIST ///////////////////////////////////////////////////////////////

TransactionList* initTransactionList(char* walletId) {
    TransactionList* transactionList = (TransactionList*)malloc(sizeof(TransactionList));
    if (walletId != NULL) {
        transactionList->walletId = (char*)malloc(strlen(walletId) + 1);
        strcpy(transactionList->walletId, walletId);
    } else {
        transactionList->walletId = NULL;
    }
    transactionList->size = 0;

    transactionList->firstTransaction = NULL;
    transactionList->lastTransaction = NULL;

    return transactionList;
}

TransactionList** initTransactionListArray(unsigned int size) {
    TransactionList** transactionLists = (TransactionList**)malloc(size * sizeof(TransactionList*));

    for (int i = 0; i < size; i++) {
        transactionLists[i] = NULL;
    }

    return transactionLists;
}

void freeTransactionList(TransactionList** transactionList, char shoudFreeTransactions) {
    if ((*transactionList) == NULL)
        return;

    if (shoudFreeTransactions == 1) {
        freeTransactionRec(&(*transactionList)->firstTransaction);
    }

    free((*transactionList)->walletId);
    (*transactionList)->walletId = NULL;

    free(*transactionList);
    (*transactionList) = NULL;

    return;
}

void freeTransactionListArray(TransactionList*** transactionLists, unsigned int size, char shoudFreeTransactions) {
    if ((*transactionLists) == NULL)
        return;

    for (int i = 0; i < size; i++) {
        if ((*transactionLists)[i] != NULL) {
            freeTransactionList(&(*transactionLists)[i], shoudFreeTransactions);
        }
    }

    free(*transactionLists);
    (*transactionLists) = NULL;

    return;
}

Transaction* addTransactionToEndOfTransactionList(TransactionList* transactionList, Transaction* transaction) {  // add by pointer
    if (transactionList->size == 0) {
        transactionList->firstTransaction = transaction;
        transactionList->lastTransaction = transactionList->firstTransaction;
        transactionList->size++;
        return transactionList->firstTransaction;
    } else {
        transactionList->lastTransaction->nextTransaction = transaction;
        transactionList->lastTransaction = transactionList->lastTransaction->nextTransaction;
        transactionList->size++;
        return transactionList->lastTransaction;
    }

    return NULL;  // not normal behavior
}

Transaction* addTransactionToTransactionListSorted(TransactionList* transactionList, Transaction* transaction) {
    if (transactionList->size == 0) {
        transactionList->firstTransaction = transaction;
        transactionList->lastTransaction = transactionList->firstTransaction;

        transactionList->size++;
        printf("Inserted transaction with id %s to sorted TransactionList\n", transaction->transactionId);
        return transactionList->firstTransaction;
    } else {
        Transaction* curTransaction = transactionList->firstTransaction;

        if (transaction->timestamp < curTransaction->timestamp) {
            // insert at start
            Transaction* transactionToInsert = transaction;
            transactionToInsert->nextTransaction = curTransaction;

            transactionList->firstTransaction = transactionToInsert;
            transactionList->size++;
            printf("Inserted transaction with id %s to sorted TransactionList\n", transaction->transactionId);
            return transactionList->firstTransaction;
        }
        while (curTransaction != NULL) {
            if (curTransaction->nextTransaction != NULL) {
                if (transaction->timestamp < curTransaction->nextTransaction->timestamp) {
                    Transaction* transactionToInsert = transaction;
                    transactionToInsert->nextTransaction = curTransaction->nextTransaction;

                    curTransaction->nextTransaction = transactionToInsert;
                    transactionList->size++;
                    printf("Inserted transaction with id %s to sorted TransactionList\n", transaction->transactionId);
                    return curTransaction->nextTransaction;
                } else if (transaction->timestamp == curTransaction->nextTransaction->timestamp &&
                           strcmp(transaction->transactionId, curTransaction->nextTransaction->transactionId) == 0) {  // we don't want duplicates
                    freeTransaction(&transaction);
                    return NULL;
                }
            } else {
                // insert at the end
                curTransaction->nextTransaction = transaction;
                transactionList->lastTransaction = curTransaction->nextTransaction;

                transactionList->size++;
                printf("Inserted transaction with id %s to sorted TransactionList\n", transaction->transactionId);
                return curTransaction->nextTransaction;
            }

            curTransaction = curTransaction->nextTransaction;
        }
    }

    return NULL;  // not normal behavior
}

void printTransactionsFromTransactionList(TransactionList* transactionList, char* time1, char* date1, char* time2, char* date2, char findEarnings) {
    // create a temporary transaction list to print transactions in ascending sorting order by timestamp
    TransactionList* tempTransactionList = initTransactionList(NULL);
    int amountSum = 0;

    Transaction* curTransaction = transactionList->firstTransaction;
    while (curTransaction != NULL) {
        char *timeS, *dateS, datetimeSCopy[MAX_DATETIME_SIZE];
        strcpy(datetimeSCopy, curTransaction->datetimeS);
        dateS = strtok(datetimeSCopy, " ");
        timeS = strtok(NULL, " ");
        if ((time1 == NULL && date1 == NULL && time2 == NULL && date2 == NULL) ||
            ((time1 != NULL && date1 != NULL && time2 != NULL && date2 != NULL) && compareDatesS(dateS, date1) >= 0 && compareDatesS(dateS, date2) <= 0 && compareTimesS(timeS, time1) >= 0 && compareTimesS(timeS, time2) <= 0) ||
            ((time1 != NULL && date1 == NULL && time2 != NULL && date2 == NULL) && compareTimesS(timeS, time1) >= 0 && compareTimesS(timeS, time2) <= 0) ||
            ((time1 == NULL && date1 != NULL && time2 == NULL && date2 != NULL) && compareDatesS(dateS, date1) >= 0 && compareDatesS(dateS, date2) <= 0)) {
            addTransactionToTransactionListSorted(tempTransactionList, initTransaction(curTransaction->transactionId, curTransaction->amount, curTransaction->senderWalletId, curTransaction->receiverWalletId, curTransaction->datetimeS));
            amountSum += curTransaction->amount;
        }
        curTransaction = curTransaction->nextTransaction;
    }

    if (tempTransactionList->size == 0) {
        printf("No transactions found\n");
        freeTransactionList(&tempTransactionList, 1);
        return;
    }

    printf(ANSI_COLOR_YELLOW "\nResults:" ANSI_COLOR_RESET "\nAmount of dollars %s: %d\n", findEarnings == 1 ? "earned" : "paid", amountSum);
    curTransaction = tempTransactionList->firstTransaction;
    while (curTransaction != NULL) {
        printf("%s %s %s %d %s\n", curTransaction->transactionId, curTransaction->senderWalletId, curTransaction->receiverWalletId, curTransaction->amount, curTransaction->datetimeS);
        curTransaction = curTransaction->nextTransaction;
    }

    freeTransactionList(&tempTransactionList, 1);

    return;
}

void printTransactionsOfTransactionListSimple(TransactionList* transactionList) {
    Transaction* curTransaction = transactionList->firstTransaction;
    printf(ANSI_COLOR_YELLOW "\nResults:\n" ANSI_COLOR_RESET);
    while (curTransaction != NULL) {
        printf("%s %s %s %d %s\n", curTransaction->transactionId, curTransaction->senderWalletId, curTransaction->receiverWalletId, curTransaction->amount, curTransaction->datetimeS);
        curTransaction = curTransaction->nextTransaction;
    }

    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF TRANSACTION LIST ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF BUCKET ///////////////////////////////////////////////////////////////

Bucket* initBucket(unsigned int bucketSize) {
    Bucket* bucket = (Bucket*)malloc(sizeof(Bucket));
    bucket->transactionLists = initTransactionListArray(bucketSize);
    bucket->nextIndex = 0;
    bucket->nextBucket = NULL;

    return bucket;
}

void freeBucket(Bucket** bucket, unsigned int bucketSize, char shoudFreeTransactions) {
    if ((*bucket) == NULL)
        return;

    freeTransactionListArray(&(*bucket)->transactionLists, bucketSize, shoudFreeTransactions);

    free(*bucket);
    (*bucket) = NULL;

    return;
}

void freeBucketRec(HashTable* hashTable, Bucket** bucket, char shoudFreeTransactions) {
    if ((*bucket) == NULL)
        return;

    freeBucketRec(hashTable, &((*bucket)->nextBucket), shoudFreeTransactions);

    freeBucket(bucket, hashTable->bucketSize, shoudFreeTransactions);
    return;
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

BucketList* initBucketList() {
    BucketList* bucketList = (BucketList*)malloc(sizeof(BucketList));

    bucketList->size = 0;

    bucketList->firstBucket = NULL;
    bucketList->lastBucket = NULL;

    return bucketList;
}

void freeBucketList(HashTable* hashTable, BucketList** bucketList, char shoudFreeTransactions) {
    if ((*bucketList) == NULL)
        return;

    freeBucketRec(hashTable, &(*bucketList)->firstBucket, shoudFreeTransactions);

    free(*bucketList);
    (*bucketList) = NULL;

    return;
}

void freeBucketListArray(HashTable* hashTable, BucketList*** bucketLists, unsigned int size, char shoudFreeTransactions) {
    if ((*bucketLists) == NULL)
        return;

    for (int i = 0; i < size; i++) {
        if ((*bucketLists)[i] != NULL) {
            freeBucketList(hashTable, &(*bucketLists)[i], shoudFreeTransactions);
        }
    }

    free(*bucketLists);
    (*bucketLists) = NULL;

    return;
}

TransactionList* findTransactionListInBucketList(HashTable* hashTable, char* walletId, unsigned int listIndex) {
    BucketList* bucketList = hashTable->bucketLists[listIndex];
    if (bucketList == NULL) {
        printf("Invalid hash table index\n");
        return NULL;
    }

    Bucket* curBucket = bucketList->firstBucket;

    while (curBucket != NULL) {
        for (int i = 0; i < hashTable->bucketSize; i++) {
            if (curBucket->transactionLists[i] != NULL && strcmp(curBucket->transactionLists[i]->walletId, walletId) == 0) {
                return curBucket->transactionLists[i];
            }
        }
        curBucket = curBucket->nextBucket;
    }

    // not found
    return NULL;
}

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int bucketSize) {
    if (bucketList->size == 0) {
        bucketList->firstBucket = initBucket(bucketSize);
        bucketList->lastBucket = bucketList->firstBucket;
        bucketList->size++;
        return bucketList->firstBucket;
    } else {
        bucketList->lastBucket->nextBucket = initBucket(bucketSize);
        bucketList->lastBucket = bucketList->lastBucket->nextBucket;

        bucketList->size++;
        return bucketList->lastBucket;
    }

    return NULL;  // not normal behavior
}

//////////////////////////////////////////////////////////////////////////////// END OF BUCKET LIST ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF HASH TABLE ///////////////////////////////////////////////////////////////

HashTable* initHashTable(unsigned int bucketListArraySize, unsigned int bucketSize) {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->bucketLists = initBucketListArray(bucketListArraySize);
    hashTable->bucketListArraySize = bucketListArraySize;
    hashTable->bucketSize = bucketSize;

    return hashTable;
}

void freeHashTable(HashTable** hashTable, char shoudFreeTransactions) {
    if ((*hashTable) == NULL)
        return;

    freeBucketListArray((*hashTable), &(*hashTable)->bucketLists, (*hashTable)->bucketListArraySize, shoudFreeTransactions);

    free(*hashTable);
    (*hashTable) = NULL;
}

// djb2 hash function
unsigned int hashFunction(HashTable* hashTable, char* walletId) {
    unsigned long hash = 5381;
    int c;

    while ((c = *(walletId++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % hashTable->bucketListArraySize;
}

char* getWalletIdByHashTableType(Transaction* transaction, HashTableType type) {
    return type == SENDER ? transaction->senderWalletId : transaction->receiverWalletId;
}

Transaction* insertTransactionToHashTable(HashTable* hashTable, Transaction* transaction, HashTableType hashTableType) {
    // get index by hashing sender's or receiver's wallet id
    unsigned int index = hashFunction(hashTable, getWalletIdByHashTableType(transaction, hashTableType));  // implies that the sender is the right value to hash for this hash table

    TransactionList* foundTransactionList = NULL;
    if (hashTable->bucketLists[index] == NULL) {
        // create new bucket list and bucket
        hashTable->bucketLists[index] = initBucketList(transaction->senderWalletId);
        addBucketToEndOfBucketList(hashTable->bucketLists[index], hashTable->bucketSize);
    } else {
        // search for existing transaction list
        foundTransactionList = findTransactionListInBucketList(hashTable, getWalletIdByHashTableType(transaction, hashTableType), index);
    }

    if (foundTransactionList == NULL) {
        Bucket* lastBucket = hashTable->bucketLists[index]->lastBucket;

        if (lastBucket->nextIndex >= hashTable->bucketSize) {  // bucket is full
            // add new bucket to bucket list
            lastBucket = addBucketToEndOfBucketList(hashTable->bucketLists[index], hashTable->bucketSize);
        }

        // create new transaction list to last bucket of bucket list
        lastBucket->transactionLists[lastBucket->nextIndex] = initTransactionList(getWalletIdByHashTableType(transaction, hashTableType));
        foundTransactionList = lastBucket->transactionLists[lastBucket->nextIndex];

        lastBucket->nextIndex++;
    }

    // add transaction to the current transaction list
    Transaction* insertedTransaction = addTransactionToEndOfTransactionList(foundTransactionList, transaction);
    printf(ANSI_COLOR_CYAN "Inserted transaction with id %s to %s HashTable\n" ANSI_COLOR_RESET, transaction->transactionId, hashTableType == RECEIVER ? "receiver" : "sender");

    return insertedTransaction;
}

Transaction* findTransactionInHashTable(HashTable* hashTable, char* transactionId) {
    for (int i = 0; i < hashTable->bucketListArraySize; i++) {
        BucketList* curBucketList = hashTable->bucketLists[i];
        if (curBucketList == NULL)
            continue;

        Bucket* curBucket = curBucketList->firstBucket;
        while (curBucket != NULL) {
            for (int j = 0; j < curBucket->nextIndex; j++) {
                TransactionList* curTransactionList = curBucket->transactionLists[j];

                Transaction* curTransaction = curTransactionList->firstTransaction;
                while (curTransaction != NULL) {
                    if (strcmp(curTransaction->transactionId, transactionId) == 0) {
                        return curTransaction;
                    }
                    curTransaction = curTransaction->nextTransaction;
                }
            }
            curBucket = curBucket->nextBucket;
        }
    }

    return NULL;
}

TransactionList* findTransactionListInHashTable(HashTable* hashTable, char* walletId) {
    int index = hashFunction(hashTable, walletId);
    return findTransactionListInBucketList(hashTable, walletId, index);
}

//////////////////////////////////////////////////////////////////////////////// END OF HASH TABLE ///////////////////////////////////////////////////////////////