#include "bitcoin_tree_list.h"

BitcoinTree* initBitcoinTree(int startAmount, char* walletId, int bitcoinId) {
    BitcoinTree* bitcoinTree = (BitcoinTree*)malloc(sizeof(BitcoinTree));
    bitcoinTree->bitcoinId = bitcoinId;
    bitcoinTree->rootNode = initBitcoinTreeNode(walletId, startAmount, NULL);
    bitcoinTree->size = 0;
    // bitcoinTree->nextBitcoinTree = NULL;
    // bitcoinTree->prevBitcoinTree = NULL;

    return bitcoinTree;
}

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction) {  // transaction is NULL for remainingNodes
    BitcoinTreeNode* bitcoinTreeNode = (BitcoinTreeNode*)malloc(sizeof(BitcoinTreeNode));
    bitcoinTreeNode->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    strcpy(bitcoinTreeNode->walletId, walletId);

    bitcoinTreeNode->transaction = transaction;
    bitcoinTreeNode->amount = amount;
    bitcoinTreeNode->receiverNode = NULL;
    bitcoinTreeNode->remainingNode = NULL;

    return bitcoinTreeNode;
}

// BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount) {
//     BitcoinTreeNode* bitcoinTreeNode = malloc(sizeof(BitcoinTreeNode));
//     bitcoinTreeNode->walletId = NULL;
//     bitcoinTreeNode->amount = startAmount;
//     bitcoinTreeNode->receiverNode = NULL;
//     bitcoinTreeNode->remainingNode = NULL;

//     return bitcoinTreeNode;
// }

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode) {
    if ((*bitcoinTreeNode) == NULL)
        return;

    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->receiverNode);
    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->remainingNode);

    if ((*bitcoinTreeNode)->walletId != NULL) {
        free((*bitcoinTreeNode)->walletId);
        (*bitcoinTreeNode)->walletId = NULL;
    }
    if ((*bitcoinTreeNode)->transaction != NULL) {
        free((*bitcoinTreeNode)->transaction);
        (*bitcoinTreeNode)->transaction = NULL;
    }
    (*bitcoinTreeNode)->receiverNode = NULL;
    (*bitcoinTreeNode)->remainingNode = NULL;
    free(*bitcoinTreeNode);
    (*bitcoinTreeNode) = NULL;

    return;
}

void freeBitcoinTree(BitcoinTree** bitcoinTree) {
    if ((*bitcoinTree) == NULL)
        return;

    freeBitcoinTreeNodeRec(&(*bitcoinTree)->rootNode);
    // here maybe make the pointers next and prev = NULL ???????????????????????????????????????? see below
    // (*bitcoinTree)->nextBitcoinTree = NULL;
    // (*bitcoinTree)->prevBitcoinTree = NULL;
    free(*bitcoinTree);
    (*bitcoinTree) = NULL;

    return;
}

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTree(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    if (nextBitcoinTreeNode == NULL || (strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 && nextBitcoinTreeNode->amount == amount))
        return nextBitcoinTreeNode;

    return findBitcoinTreeNodeOfBitcoinTree(walletId, amount, nextBitcoinTreeNode->receiverNode);
    return findBitcoinTreeNodeOfBitcoinTree(walletId, amount, nextBitcoinTreeNode->remainingNode);
}

BitcoinTreeNode* findBitcoinTreeNodeForLog(char* walletId, int bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    if (nextBitcoinTreeNode == NULL || (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 && nextBitcoinTreeNode->amount >= bitcoinAmount))
        return nextBitcoinTreeNode;

    return findBitcoinTreeNodeOfBitcoinTree(walletId, bitcoinAmount, nextBitcoinTreeNode->receiverNode);
    return findBitcoinTreeNodeOfBitcoinTree(walletId, bitcoinAmount, nextBitcoinTreeNode->remainingNode);
}

void getTotalBitcoinBalanceOfWalletId(char* walletId, int* balance, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    printf("before\n");
    printf("nextBitcoinTreeNode->walletId: %s\n", nextBitcoinTreeNode->walletId);
    printf("after\n");
    if (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0) {
        (*balance) += nextBitcoinTreeNode->amount;
    }

    getTotalBitcoinBalanceOfWalletId(walletId, balance, nextBitcoinTreeNode->receiverNode);
    getTotalBitcoinBalanceOfWalletId(walletId, balance, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsNumOfBitcoin(int* transactionsNum, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    if (nextBitcoinTreeNode->transaction != NULL) {
        (*transactionsNum)++;
    }

    getTransactionsNumOfBitcoin(transactionsNum, nextBitcoinTreeNode->receiverNode);
    getTransactionsNumOfBitcoin(transactionsNum, nextBitcoinTreeNode->remainingNode);
}

void handleLog(char* walletId, Transaction* transaction, int* bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    printf("handleLog: 0\n");
    if (nextBitcoinTreeNode == NULL || (*bitcoinAmount) <= 0)
        return;

    printf("handleLog: 1\n");

    int receiverBitcoinAmount = 0, remainingBitcoinAmount = 0;
    if (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 /* && nextBitcoinTreeNode->amount >= bitcoinAmount*/) {
        // BitcoinTreeNode* foundBitcoinTreeNode = findBitcoinTreeNodeForLog(transaction->receiverWalletId, transaction->amount, bitcoinTree->rootNode);

        // if (foundBitcoinTreeNode == NULL) {
        //     printf("Suitable node not found or insufficient bitcoin amount\n");
        //     return 1;
        // }

        // if (foundBitcoinTreeNode->amount < amount) {
        //     printf("Insufficient bitcoin amount\n");
        //     return 1;
        // }
        printf("handleLog: 2\n");
        receiverBitcoinAmount = (nextBitcoinTreeNode->amount - (*bitcoinAmount)) >= 0 ? (*bitcoinAmount) : nextBitcoinTreeNode->amount;
        remainingBitcoinAmount = nextBitcoinTreeNode->amount - receiverBitcoinAmount;

        nextBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId,
                                                                receiverBitcoinAmount,
                                                                transaction);
        printf("handleLog: 3\n");

        if (remainingBitcoinAmount > 0) {  // maybe remove this condition
            nextBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId,
                                                                     remainingBitcoinAmount,
                                                                     NULL);
        }

        printf("handleLog: 3.5\n");
    }
    printf("handleLog: 4\n");

    (*bitcoinAmount) -= receiverBitcoinAmount;
    printf("bitcoinAmount = %d\n", *bitcoinAmount);

    handleLog(walletId, transaction, bitcoinAmount, nextBitcoinTreeNode->receiverNode);
    handleLog(walletId, transaction, bitcoinAmount, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsOfBitcoinTree(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    if (nextBitcoinTreeNode->transaction != NULL) {
        addTransactionToTransactionListSorted(foundTransactionList, nextBitcoinTreeNode->transaction);
    }

    getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->receiverNode);
    getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->remainingNode);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BitcoinList* initBitcoinList() {
    BitcoinList* bitcoinList = (BitcoinList*)malloc(sizeof(BitcoinList));
    bitcoinList->size = 0;
    bitcoinList->firstNode = NULL;

    return bitcoinList;
}

BitcoinListNode* initBitcoinListNode(BitcoinTree* bitcoinTree) {
    BitcoinListNode* bitcoinListNode = (BitcoinListNode*)malloc(sizeof(BitcoinListNode));
    bitcoinListNode->bitcoinTree = bitcoinTree;
    bitcoinListNode->nextNode = NULL;
    bitcoinListNode->prevNode = NULL;

    return bitcoinListNode;
}

void freeBitcoinListNode(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees) {
    if ((*bitcoinListNode) == NULL)
        return;

    if (shouldFreeBitcoinTrees == 1) {
        freeBitcoinTree(&(*bitcoinListNode)->bitcoinTree);
    }
    (*bitcoinListNode)->nextNode = NULL;
    (*bitcoinListNode)->prevNode = NULL;
    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;

    return;
}

// void freeBitcoinListNodeNoTree(BitcoinListNode** bitcoinListNode) {
//     if ((*bitcoinListNode) == NULL)
//         return;

//     // freeBitcoinTree(&(*bitcoinListNode)->bitcoinTree);
//     (*bitcoinListNode)->nextNode = NULL;
//     (*bitcoinListNode)->prevNode = NULL;
//     free(*bitcoinListNode);
//     (*bitcoinListNode) = NULL;

//     return;
// }

// BitcoinTree* initBitcoinTree(char* name) {
//     BitcoinTree* bitcoinTree = (BitcoinTree*)malloc(sizeof(BitcoinTree));
//     bitcoinTree->name = (char*)malloc(PATH_MAX);
//     strcpy(bitcoinTree->name, name);
//     bitcoinTree->nextNode = NULL;
//     bitcoinTree->prevNode = NULL;

//     return bitcoinTree;
// }

// void freeBitcoinTree(BitcoinTree** bitcoinTree) {
//     if ((*bitcoinTree) == NULL)
//         return;

//     free((*bitcoinTree)->name);
//     (*bitcoinTree)->name = NULL;
//     free(*bitcoinTree);
//     (*bitcoinTree) = NULL;
//     return;
// }

// void freeBitcoinList(BitcoinList** bitcoinList) {
//     BitcoinTree* curBitcoinTree = (*bitcoinList)->firstBitcoinTree;

//     while (curBitcoinTree != NULL) {
//         free(curBitcoinTree->name);
//         curBitcoinTree->name = NULL;

//         if (curBitcoinTree->nextNode != NULL) {
//             curBitcoinTree = curBitcoinTree->nextNode;
//             free(curBitcoinTree->prevNode);

//             curBitcoinTree->prevNode = NULL;
//         } else {
//             free(curBitcoinTree);
//             curBitcoinTree = NULL;
//         }
//     }
//     free(*bitcoinList);
//     (*bitcoinList) = NULL;
// }

void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees) {
    if ((*bitcoinListNode) == NULL)
        return;

    freeBitcoinListNodeRec(&((*bitcoinListNode)->nextNode), shouldFreeBitcoinTrees);

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeBitcoinListNode(bitcoinListNode, shouldFreeBitcoinTrees);

    // free((*bucket)->name);
    // (*bucket)->name = NULL;
    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;
    return;
}

// void freeBitcoinListNodeRecNoTree(BitcoinListNode** bitcoinListNode) {
//     if ((*bitcoinListNode) == NULL)
//         return;

//     freeBitcoinListNodeRecNoTree(&((*bitcoinListNode)->nextNode));

//     // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
//     freeBitcoinListNodeNoTree(bitcoinListNode);

//     // free((*bucket)->name);
//     // (*bucket)->name = NULL;
//     free(*bitcoinListNode);
//     (*bitcoinListNode) = NULL;
//     return;
// }

void freeBitcoinList(BitcoinList** bitcoinList, char shouldFreeBitcoinTrees) {
    if ((*bitcoinList) == NULL)
        return;
    // Bucket* curBucket = (*bitcoinList)->firstBucket;

    freeBitcoinListNodeRec(&(*bitcoinList)->firstNode, shouldFreeBitcoinTrees);

    free(*bitcoinList);
    (*bitcoinList) = NULL;

    return;
}

// void freeBitcoinListNoTrees(BitcoinList** bitcoinList) {
//     if ((*bitcoinList) == NULL)
//         return;
//     // Bucket* curBucket = (*bitcoinList)->firstBucket;

//     freeBitcoinListNodeRecNoTree(&(*bitcoinList)->firstNode);

//     free(*bitcoinList);
//     (*bitcoinList) = NULL;

//     return;
// }

BitcoinListNode* findBitcoinListNodeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return NULL;

    BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

    while (curBitcoinListNode != NULL) {
        // printf("bitcoin id: %d\n", curBitcoinListNode->bitcoinTree->bitcoinId);
        if (curBitcoinListNode->bitcoinTree->bitcoinId == bitcoinId) {
            return curBitcoinListNode;
        } else if (bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curBitcoinListNode = curBitcoinListNode->nextNode;
    }
    return NULL;
}

BitcoinListNode* addBitcoinListNodeToBitcoinList(BitcoinList* bitcoinList, char* walletId, int bitcoinId, int startAmount) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode != NULL) {
        return foundBitcoinListNode;
    }

    if (bitcoinList->size == 0) {
        bitcoinList->firstNode = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));

        bitcoinList->size++;
        printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
        return bitcoinList->firstNode;
    } else {
        BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

        if (bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {
            // insert at start
            BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));
            bitcoinListNodeToInsert->nextNode = curBitcoinListNode;

            curBitcoinListNode->prevNode = bitcoinListNodeToInsert;
            bitcoinList->firstNode = bitcoinListNodeToInsert;
            bitcoinList->size++;
            printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
            return bitcoinList->firstNode;
        }
        while (curBitcoinListNode != NULL) {
            if (curBitcoinListNode->nextNode != NULL) {
                if (bitcoinId < curBitcoinListNode->nextNode->bitcoinTree->bitcoinId) {
                    BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));
                    bitcoinListNodeToInsert->prevNode = curBitcoinListNode;
                    bitcoinListNodeToInsert->nextNode = curBitcoinListNode->nextNode;

                    curBitcoinListNode->nextNode->prevNode = bitcoinListNodeToInsert;
                    curBitcoinListNode->nextNode = bitcoinListNodeToInsert;
                    bitcoinList->size++;
                    printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
                return curBitcoinListNode->nextNode;
            }

            curBitcoinListNode = curBitcoinListNode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

BitcoinListNode* addBitcoinListNodeToBitcoinListByPointer(BitcoinList* bitcoinList, BitcoinListNode* bitcoinListNode) {
    if (findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinListNode->bitcoinTree->bitcoinId) != NULL) {
        return NULL;
    }

    if (bitcoinList->size == 0) {
        bitcoinList->firstNode = initBitcoinListNode(bitcoinListNode->bitcoinTree);

        bitcoinList->size++;
        printf("Inserted |%d| to BitcoinList\n\n", bitcoinListNode->bitcoinTree->bitcoinId);
        return bitcoinList->firstNode;
    } else {
        BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

        if (bitcoinListNode->bitcoinTree->bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {
            // insert at start
            BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(bitcoinListNode->bitcoinTree);
            bitcoinListNodeToInsert->nextNode = curBitcoinListNode;

            curBitcoinListNode->prevNode = bitcoinListNodeToInsert;
            bitcoinList->firstNode = bitcoinListNodeToInsert;
            bitcoinList->size++;
            printf("Inserted |%d| to BitcoinList\n\n", bitcoinListNode->bitcoinTree->bitcoinId);
            return bitcoinList->firstNode;
        }
        while (curBitcoinListNode != NULL) {
            if (curBitcoinListNode->nextNode != NULL) {
                if (bitcoinListNode->bitcoinTree->bitcoinId < curBitcoinListNode->nextNode->bitcoinTree->bitcoinId) {
                    BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(bitcoinListNode->bitcoinTree);
                    bitcoinListNodeToInsert->prevNode = curBitcoinListNode;
                    bitcoinListNodeToInsert->nextNode = curBitcoinListNode->nextNode;

                    curBitcoinListNode->nextNode->prevNode = bitcoinListNodeToInsert;
                    curBitcoinListNode->nextNode = bitcoinListNodeToInsert;
                    bitcoinList->size++;
                    printf("Inserted |%d| to BitcoinList\n\n", bitcoinListNode->bitcoinTree->bitcoinId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(bitcoinListNode->bitcoinTree);
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf("Inserted |%d| to BitcoinList\n\n", bitcoinListNode->bitcoinTree->bitcoinId);
                return curBitcoinListNode->nextNode;
            }

            curBitcoinListNode = curBitcoinListNode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

int deleteBitcoinListNodeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return -1;

    BitcoinListNode* bitcoinListNodeToDelete = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);

    if (bitcoinListNodeToDelete == NULL) {
        printf("Bitcoin list node with id %d not found in names' list\n", bitcoinId);
        return -1;
    }

    if (bitcoinListNodeToDelete->bitcoinTree->bitcoinId == bitcoinList->firstNode->bitcoinTree->bitcoinId) {
        bitcoinList->firstNode = bitcoinListNodeToDelete->nextNode;
    }
    if (bitcoinListNodeToDelete->prevNode != NULL) {
        bitcoinListNodeToDelete->prevNode->nextNode = bitcoinListNodeToDelete->nextNode;
    }
    if (bitcoinListNodeToDelete->nextNode != NULL) {
        bitcoinListNodeToDelete->nextNode->prevNode = bitcoinListNodeToDelete->prevNode;
    }

    freeBitcoinListNode(&bitcoinListNodeToDelete, 0);
    bitcoinList->size--;

    return 0;
}

///////////////////////////////////////////////

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int* amount) {
    // BitcoinTreeNode* foundBitcoinTreeNode = findBitcoinTreeNodeForLog(transaction->receiverWalletId, transaction->amount, bitcoinTree->rootNode);

    // if (foundBitcoinTreeNode == NULL) {
    //     printf("Suitable node not found or insufficient bitcoin amount\n");
    //     return 1;
    // }

    // // if (foundBitcoinTreeNode->amount < amount) {
    // //     printf("Insufficient bitcoin amount\n");
    // //     return 1;
    // // }

    // foundBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId, transaction->amount, transaction);

    // if (foundBitcoinTreeNode->amount > transaction->amount) {
    //     foundBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId, foundBitcoinTreeNode->amount - transaction->amount, NULL);
    // }
    printf("addLogToBitcoinTree\n");

    handleLog(transaction->senderWalletId, transaction, amount, bitcoinTree->rootNode);

    return 0;
}

TransactionList* findTransactionsInBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return NULL;
    }

    TransactionList* foundTransactionList = initTransactionList(NULL);
    getTransactionsOfBitcoinTree(foundTransactionList, foundBitcoinListNode->bitcoinTree->rootNode);

    return foundTransactionList;
    // transaction list must not be empty here most of th

    // Bitcoin bitcoinListNode->bitcoinTree
}

int getUnspentAmountOfBitcoin(BitcoinList* bitcoinList, int bitcoinId) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return -1;
    }

    BitcoinTreeNode* curBitcoinTreeNode = foundBitcoinListNode->bitcoinTree->rootNode;
    while (curBitcoinTreeNode->remainingNode != NULL) {
        curBitcoinTreeNode = curBitcoinTreeNode->remainingNode;
    }

    return curBitcoinTreeNode->amount;
}

int getUnspentAmountOfBitcoinByTree(BitcoinTree* bitcoinTree) {
    BitcoinTreeNode* curBitcoinTreeNode = bitcoinTree->rootNode;
    while (curBitcoinTreeNode->remainingNode != NULL) {
        curBitcoinTreeNode = curBitcoinTreeNode->remainingNode;
    }

    return curBitcoinTreeNode->amount;
}

int getCurrentBitcoinBalanceOfWalletId(BitcoinTree* bitcoinTree, char* walletId) {
    int balance = 0;
    getTotalBitcoinBalanceOfWalletId(walletId, &balance, bitcoinTree->rootNode);

    return balance;
}

int getTransactionsNumOfBitcoinById(BitcoinList* bitcoinList, int bitcoinId) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return -1;
    }

    int transactionsNum = 0;

    getTransactionsNumOfBitcoin(&transactionsNum, foundBitcoinListNode->bitcoinTree->rootNode);

    return transactionsNum;
}

void getTransactionsOfBitcoin(BitcoinList* bitcoinList, int bitcoinId, TransactionList* foundTransactionList) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return;
    }

    getTransactionsOfBitcoinTree(foundTransactionList, foundBitcoinListNode->bitcoinTree->rootNode);

    return;
}