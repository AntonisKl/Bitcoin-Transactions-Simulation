#include "hashtable.h"

//////////////////////////////////////////////////////////////////////////////// START OF TRANSACTION ///////////////////////////////////////////////////////////////

Transaction* initTransaction(char* transactionId, int amount, char* senderWalletId, char* receiverWalletId, char* datetimeS, BitcoinList* bitcoinList) {  // bitcoinList will be already initialized
    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    printf("\n\n\nINIT TRANSACTION WITH ID: %s\n\n\n\n", transactionId);
    transaction->amount = amount;

    transaction->transactionId = (char*)malloc(MAX_TRANSACTION_ID_SIZE);
    strcpy(transaction->transactionId, transactionId);

    transaction->senderWalletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    strcpy(transaction->senderWalletId, senderWalletId);

    transaction->receiverWalletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    strcpy(transaction->receiverWalletId, receiverWalletId);

    transaction->datetimeS = (char*)malloc(MAX_DATETIME_SIZE);
    strcpy(transaction->datetimeS, datetimeS);

    transaction->timestamp = datetimeStringToTimeStamp(datetimeS);

    // transaction->bitcoinList = initBitcoinList();
    transaction->bitcoinList = bitcoinList;

    transaction->nextTransaction = NULL;

    return transaction;
}

void freeTransaction(Transaction** transaction) {
    if ((*transaction) == NULL)
        return;

    printf("\n\n\nFREE TRANSACTION WITH ID: %s\n\n\n\n", (*transaction)->transactionId);

    free((*transaction)->transactionId);
    (*transaction)->transactionId = NULL;
    free((*transaction)->senderWalletId);
    (*transaction)->senderWalletId = NULL;
    free((*transaction)->receiverWalletId);
    (*transaction)->receiverWalletId = NULL;
    free((*transaction)->datetimeS);
    (*transaction)->datetimeS = NULL;
    freeBitcoinList(&(*transaction)->bitcoinList, 0);
    (*transaction)->bitcoinList = NULL;

    free((*transaction));
    (*transaction) = NULL;

    printf("freed Transaction!!\n");
    return;
}

void freeTransactionRec(Transaction** transaction) {
    if ((*transaction) == NULL)
        return;

    freeTransactionRec(&((*transaction)->nextTransaction));

    // free((*bucket)->name);
    // (*bucket)->name = NULL;
    freeTransaction(transaction);
    return;
}

// Transaction** initTransactionArray(unsigned int size) {
//     Transaction** transactions = (Transaction**)malloc(size * sizeof(Transaction*));
//     for (int i = 0; i < size; i++) {
//         transactions[i] = NULL;
//     }

//     return transactions;
// }

// void freeTransactionArray(Transaction*** transactions, unsigned int size) {
//     for (int i = 0; i < size; i++) {
//         if ((*transactions[i]) != NULL)
//             freeTransaction(transactions[i]);
//     }

//     free((*transactions));
//     (*transactions) = NULL;
//     return;
// }

//////////////////////////////////////////////////////////////////////////////// END OF TRANSACTION ///////////////////////////////////////////////////////////////

TransactionList* initTransactionList(char* walletId) {
    TransactionList* transactionList = (TransactionList*)malloc(sizeof(TransactionList));
    if (walletId != NULL) {
        transactionList->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
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
    // Bucket* curBucket = (*transactionList)->firstTransaction;

    if (shoudFreeTransactions == 1) {
        freeTransactionRec(&(*transactionList)->firstTransaction);
    }
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

    free((*transactionList)->walletId);
    (*transactionList)->walletId = NULL;

    free(*transactionList);
    (*transactionList) = NULL;

    return;
}

// void freeTransactionListNoTransactions(TransactionList** transactionList) {
//     if ((*transactionList) == NULL)
//         return;
//     // Bucket* curBucket = (*transactionList)->firstTransaction;

//     // freeTransactionRec(&(*transactionList)->firstTransaction);

//     // while (curBucket != NULL) {
//     //     // free(curBucket->name);
//     //     // curBucket->name = NULL;

//     //     if (curBucket->nextBucket != NULL) {
//     //         curBucket = curBucket->nextBucket;
//     //         freeBucket()
//     //         // free(curBucket->prevNode);

//     //         // curBucket->prevNode = NULL;
//     //     } else {
//     //         free(curBucket);
//     //         curBucket = NULL;
//     //     }
//     // }

//     free((*transactionList)->walletId);
//     (*transactionList)->walletId = NULL;

//     free(*transactionList);
//     (*transactionList) = NULL;

//     return;
// }

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

Transaction* addTransactionToEndOfTransactionList(TransactionList* transactionList, Transaction* transaction) {  // add by pointer
    if (transactionList->size == 0) {
        transactionList->firstTransaction = transaction;
        transactionList->lastTransaction = transactionList->firstTransaction;
        transactionList->size++;
        printf("added transaction with id: %s\n", transaction->transactionId);
        return transactionList->firstTransaction;
    } else {
        transactionList->lastTransaction->nextTransaction = transaction;
        transactionList->lastTransaction = transactionList->lastTransaction->nextTransaction;
        transactionList->size++;
        printf("added transaction with id: %s\n", transaction->transactionId);
        return transactionList->lastTransaction;
    }

    return NULL;  // not normal behavior
}

Transaction* addTransactionToTransactionListSorted(TransactionList* transactionList, Transaction* transaction) {
    if (transactionList->size == 0) {
        transactionList->firstTransaction = transaction;
        transactionList->lastTransaction = transactionList->firstTransaction;

        transactionList->size++;
        printf("Inserted |%ld| to TransactionList\n\n", transaction->timestamp);
        return transactionList->firstTransaction;
    } else {
        Transaction* curTransaction = transactionList->firstTransaction;

        if (transaction->timestamp < curTransaction->timestamp) {
            // insert at start
            Transaction* transactionToInsert = transaction;
            transactionToInsert->nextTransaction = curTransaction;

            // curTransaction->prevTransaction = transactionToInsert;
            transactionList->firstTransaction = transactionToInsert;
            transactionList->size++;
            printf("Inserted |%ld| to TransactionList\n\n", transaction->timestamp);
            return transactionList->firstTransaction;
        }
        while (curTransaction != NULL) {
            if (curTransaction->nextTransaction != NULL) {
                if (transaction->timestamp < curTransaction->nextTransaction->timestamp) {
                    Transaction* transactionToInsert = transaction;
                    // transactionToInsert->prevTransaction = curTransaction;
                    transactionToInsert->nextTransaction = curTransaction->nextTransaction;

                    // curTransaction->nextTransaction->prevTransaction = transactionToInsert;
                    curTransaction->nextTransaction = transactionToInsert;
                    transactionList->size++;
                    printf("Inserted |%ld| to TransactionList\n\n", transaction->timestamp);
                    return curTransaction->nextTransaction;
                }
            } else {
                // insert at the end
                curTransaction->nextTransaction = transaction;
                // curTransaction->nextTransaction->prevTransaction = curTransaction;
                transactionList->lastTransaction = curTransaction->nextTransaction;

                transactionList->size++;
                printf("Inserted |%ld| to TransactionList\n\n", transaction->timestamp);
                return curTransaction->nextTransaction;
            }

            curTransaction = curTransaction->nextTransaction;
        }
    }

    return NULL;  // not normal behavior
}

//////////////////////////////////////////////////////////////////////////////// START OF BUCKET ///////////////////////////////////////////////////////////////

Bucket* initBucket(unsigned int bucketSize) {
    Bucket* bucket = (Bucket*)malloc(sizeof(Bucket));
    bucket->transactionLists = initTransactionListArray(bucketSize);
    // bucket->name = (char*)malloc(PATH_MAX);
    // strcpy(bucket->name, name);
    bucket->nextIndex = 0;
    bucket->nextBucket = NULL;
    // bucket->prevNode = NULL;

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

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);

    // free((*bucket)->name);
    // (*bucket)->name = NULL;

    freeBucket(bucket, hashTable->bucketSize, shoudFreeTransactions);
    // free(*bucket);
    // (*bucket) = NULL;
    return;
}

// Transaction* addTransactionToHashTable(HashTable* hashTable, unsigned int listIndex, Transaction* transaction) {
//     TransactionList* foundTransactionList = findTransactionListInBucketList(hashTable, /*walletid*/, listIndex);

//     if (foundTransactionList == NULL) {
//         Bucket* lastBucket = hashTable->bucketLists[listIndex]->lastBucket;
//         lastBucket->transactionLists[lastBucket->nextIndex] = initTransactionList(walletId);
//         foundTransactionList = lastBucket->transactionLists[lastBucket->nextIndex];

//         lastBucket->nextIndex++;
//     }

//     addTransactionToEndOfTransactionList(foundTransactionList, transaction);

// }

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
    // bucketList->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    // strcpy(bucketList->walletId, walletId);

    bucketList->size = 0;

    bucketList->firstBucket = NULL;
    bucketList->lastBucket = NULL;

    return bucketList;
}

void freeBucketList(HashTable* hashTable, BucketList** bucketList, char shoudFreeTransactions) {
    if ((*bucketList) == NULL)
        return;
    // Bucket* curBucket = (*bucketList)->firstBucket;

    freeBucketRec(hashTable, &(*bucketList)->firstBucket, shoudFreeTransactions);

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

    // free((*bucketList)->walletId);
    // (*bucketList)->walletId = NULL;

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

Bucket* addBucketToEndOfBucketList(BucketList* bucketList, unsigned int bucketSize) {
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

void freeHashTable(HashTable** hashTable, char shoudFreeTransactions) {
    if ((*hashTable) == NULL)
        return;

    freeBucketListArray((*hashTable), &(*hashTable)->bucketLists, (*hashTable)->bucketListArraySize, shoudFreeTransactions);

    free(*hashTable);
    (*hashTable) = NULL;
}

unsigned int hashFunction(HashTable* hashTable, char* walletId) {
    // djb2 hash function
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
    // FIRST SEE IF TRANSACTION IS ALREADY MADE !!!!!!!!!!!!!!!!!!!!!!!!

    unsigned int index = hashFunction(hashTable, getWalletIdByHashTableType(transaction, hashTableType));  // implies that the sender is the right value to hash for this hash table
    printf("index: %d WALLET ID: %s\n", index, getWalletIdByHashTableType(transaction, hashTableType));

    TransactionList* foundTransactionList = NULL;
    if (hashTable->bucketLists[index] == NULL) {
        hashTable->bucketLists[index] = initBucketList(transaction->senderWalletId);
        printf("init bucket list at index: %d\n\n\n\n", index);
        addBucketToEndOfBucketList(hashTable->bucketLists[index], hashTable->bucketSize);
    } else {
        foundTransactionList = findTransactionListInBucketList(hashTable, getWalletIdByHashTableType(transaction, hashTableType), index);
    }

    if (foundTransactionList == NULL) {
        Bucket* lastBucket = hashTable->bucketLists[index]->lastBucket;
        // if (lastBucket == NULL) {
        //     hashTable->bucketLists[index]->lastBucket = initBucket(hashTable->bucketSize);
        // }

        if (lastBucket->nextIndex >= hashTable->bucketSize) {  // bucket is full
            lastBucket = addBucketToEndOfBucketList(hashTable->bucketLists[index], hashTable->bucketSize);
        }

        lastBucket->transactionLists[lastBucket->nextIndex] = initTransactionList(getWalletIdByHashTableType(transaction, hashTableType));
        foundTransactionList = lastBucket->transactionLists[lastBucket->nextIndex];

        lastBucket->nextIndex++;
    }

    return addTransactionToEndOfTransactionList(foundTransactionList, transaction);

    // return addTransactionToBucketList(hashTable, index, transaction);
}

Transaction* findTransactionInHashTable(HashTable* hashTable, char* transactionId) {
    for (int i = 0; i < hashTable->bucketListArraySize; i++) {
        printf("i=%d\n", i);
        BucketList* curBucketList = hashTable->bucketLists[i];
        if (curBucketList == NULL) {
            printf("continue\n");
            continue;
        }
        Bucket* curBucket = curBucketList->firstBucket;
        while (curBucket != NULL) {
            for (int j = 0; j < curBucket->nextIndex; j++) {
                printf("j=%d, index=%d\n", j, curBucket->nextIndex);

                TransactionList* curTransactionList = curBucket->transactionLists[j];

                Transaction* curTransaction = curTransactionList->firstTransaction;
                while (curTransaction != NULL) {
                    printf("while, curid=%s\n", curTransaction->transactionId);
                    if (strcmp(curTransaction->transactionId, transactionId) == 0) {
                        return curTransaction;
                    }
                    printf("hey\n");
                    curTransaction = curTransaction->nextTransaction;
                    printf("hey1.5\n");
                }
                printf("hey1\n");
            }
            curBucket = curBucket->nextBucket;
        }
    }
    printf("eeee\n");

    return NULL;
}

Transaction* printTransactionsInHashTable(HashTable* hashTable) {
    for (int i = 0; i < hashTable->bucketListArraySize; i++) {
        printf("i=%d\n", i);
        BucketList* curBucketList = hashTable->bucketLists[i];
        if (curBucketList == NULL) {
            continue;
        }
        Bucket* curBucket = curBucketList->firstBucket;
        while (curBucket != NULL) {
            for (int j = 0; j < curBucket->nextIndex; j++) {
                TransactionList* curTransactionList = curBucket->transactionLists[j];

                Transaction* curTransaction = curTransactionList->firstTransaction;
                while (curTransaction != NULL) {
                    printf("transaction id: %s, bucket list array index: %d\n", curTransaction->transactionId, i);
                    curTransaction = curTransaction->nextTransaction;
                }
                printf("hey1\n");
            }
            curBucket = curBucket->nextBucket;
        }
    }
    printf("eeee\n");

    return NULL;
}

TransactionList* findTransactionListInHashTable(HashTable* hashTable, char* walletId) {
    int index = hashFunction(hashTable, walletId);
    printf("inside index: %d\n", index);
    return findTransactionListInBucketList(hashTable, walletId, index);
}

// int getAmountSumFromTransactionList(TransactionList* transactionList, char* time1, char* date1, char* time2, char* date2) {
//     int amountSum = 0;
//     Transaction* curTransaction = transactionList->firstTransaction;
//     while (curTransaction != NULL) {
//         char *timeS, *dateS, datetimeSCopy[MAX_DATETIME_SIZE];
//         strcpy(datetimeSCopy, curTransaction->datetimeS);
//         dateS = strtok(datetimeSCopy, " ");
//         timeS = strtok(NULL, " ");
//         if ((time1 == NULL && date1 == NULL && time2 == NULL && date2 == NULL) ||
//             ((time1 != NULL && date1 != NULL && time2 != NULL && date2 != NULL) && compareDatesS(dateS, date1) > 0 && compareDatesS(dateS, date2) < 0 && compareTimesS(timeS, time1) > 0 && compareTimesS(timeS, time2) < 0) ||
//             ((time1 != NULL && date1 == NULL && time2 != NULL && date2 == NULL) && compareTimesS(timeS, time1) > 0 && compareTimesS(timeS, time2) < 0) ||
//             ((time1 == NULL && date1 != NULL && time2 == NULL && date2 != NULL) && compareDatesS(dateS, date1) > 0 && compareDatesS(dateS, date2) < 0)) {
//             amountSum += curTransaction->amount;
//         }
//         curTransaction = curTransaction->nextTransaction;
//     }
// }

void printTransactionsFromTransactionList(TransactionList* transactionList, char* time1, char* date1, char* time2, char* date2, char findEarnings) {
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
            addTransactionToTransactionListSorted(tempTransactionList, curTransaction);
            amountSum += curTransaction->amount;
        }
        curTransaction = curTransaction->nextTransaction;
    }

    if (tempTransactionList->size == 0) {
        printf("No transactions found\n");
        freeTransactionList(&tempTransactionList, 0);
        return;
    }

    printf("Total amount of dollars %s: %d\n", findEarnings == 1 ? "earned" : "paid", amountSum);
    curTransaction = tempTransactionList->firstTransaction;
    while (curTransaction != NULL) {
        printf("%s %s %s %d %s\n", curTransaction->transactionId, curTransaction->senderWalletId, curTransaction->receiverWalletId, curTransaction->amount, curTransaction->datetimeS);
        curTransaction = curTransaction->nextTransaction;
    }
    freeTransactionList(&tempTransactionList, 0);

    return;
    // freeTransactionList(tempTransactionList);
}

void printTransactionsOfTransactionListSimple(TransactionList* transactionList) {
    Transaction* curTransaction = transactionList->firstTransaction;
    while (curTransaction != NULL) {
        printf("%s %s %s %d %s\n", curTransaction->transactionId, curTransaction->senderWalletId, curTransaction->receiverWalletId, curTransaction->amount, curTransaction->datetimeS);
        curTransaction = curTransaction->nextTransaction;
    }

    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF HASH TABLE ///////////////////////////////////////////////////////////////