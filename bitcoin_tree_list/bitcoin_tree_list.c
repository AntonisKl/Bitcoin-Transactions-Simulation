#include "bitcoin_tree_list.h"

//////////////////////////////////////////////////////////////////////////////// START OF BITCOIN TREE ///////////////////////////////////////////////////////////////

BitcoinTree* initBitcoinTree(int startAmount, char* walletId, int bitcoinId) {
    BitcoinTree* bitcoinTree = (BitcoinTree*)malloc(sizeof(BitcoinTree));
    bitcoinTree->bitcoinId = bitcoinId;
    bitcoinTree->rootNode = initBitcoinTreeNode(walletId, startAmount, NULL);
    bitcoinTree->size = 0;

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

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode, char shouldFreeTransactions) {
    if ((*bitcoinTreeNode) == NULL)
        return;

    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->receiverNode, shouldFreeTransactions);
    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->remainingNode, shouldFreeTransactions);

    if ((*bitcoinTreeNode)->walletId != NULL) {
        free((*bitcoinTreeNode)->walletId);
        (*bitcoinTreeNode)->walletId = NULL;
    }

    if (shouldFreeTransactions == 1 && (*bitcoinTreeNode)->transaction != NULL) {
        free((*bitcoinTreeNode)->transaction);
        (*bitcoinTreeNode)->transaction = NULL;
    }
    (*bitcoinTreeNode)->receiverNode = NULL;
    (*bitcoinTreeNode)->remainingNode = NULL;
    free(*bitcoinTreeNode);
    (*bitcoinTreeNode) = NULL;

    return;
}

void freeBitcoinTree(BitcoinTree** bitcoinTree, char shouldFreeTransactions) {
    if ((*bitcoinTree) == NULL)
        return;

    freeBitcoinTreeNodeRec(&(*bitcoinTree)->rootNode, shouldFreeTransactions);
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

    if (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0) {
        (*balance) += nextBitcoinTreeNode->amount;
    }

    getTotalBitcoinBalanceOfWalletId(walletId, balance, nextBitcoinTreeNode->receiverNode);
    getTotalBitcoinBalanceOfWalletId(walletId, balance, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsNumOfBitcoin(int* transactionsNum, TransactionList* foundTransactionsList, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    Transaction* curTransaction = nextBitcoinTreeNode->transaction;

    if (curTransaction != NULL && addTransactionToTransactionListSorted(foundTransactionsList,
                                                                        initTransaction(curTransaction->transactionId,
                                                                                        curTransaction->amount,
                                                                                        curTransaction->senderWalletId,
                                                                                        curTransaction->receiverWalletId,
                                                                                        curTransaction->datetimeS)) != NULL) {
        (*transactionsNum)++;
    }

    getTransactionsNumOfBitcoin(transactionsNum, foundTransactionsList, nextBitcoinTreeNode->receiverNode);
    getTransactionsNumOfBitcoin(transactionsNum, foundTransactionsList, nextBitcoinTreeNode->remainingNode);
}

void handleLog(char* walletId, Transaction* transaction, int* bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    //printf("handleLog: 0\n");
    if (nextBitcoinTreeNode == NULL || (*bitcoinAmount) <= 0)
        return;

    //printf("handleLog: 1\n");

    int receiverBitcoinAmount = 0, remainingBitcoinAmount = 0;
    if (nextBitcoinTreeNode->amount > 0 && nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 /* && nextBitcoinTreeNode->amount >= bitcoinAmount*/) {
        receiverBitcoinAmount = (nextBitcoinTreeNode->amount - (*bitcoinAmount)) >= 0 ? (*bitcoinAmount) : nextBitcoinTreeNode->amount;
        remainingBitcoinAmount = nextBitcoinTreeNode->amount - receiverBitcoinAmount;

        nextBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId,
                                                                receiverBitcoinAmount,
                                                                transaction);
        //printf("handleLog: 3\n");
        nextBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId,
                                                                 remainingBitcoinAmount,
                                                                 NULL);

        //printf("handleLog: 3.5\n");
    }
    //printf("handleLog: 4\n");

    (*bitcoinAmount) -= receiverBitcoinAmount;
    //printf("bitcoinAmount = %d\n", *bitcoinAmount);

    handleLog(walletId, transaction, bitcoinAmount, nextBitcoinTreeNode->receiverNode);
    handleLog(walletId, transaction, bitcoinAmount, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsOfBitcoinTree(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    if (nextBitcoinTreeNode->transaction != NULL) {
        Transaction* curTransaction = nextBitcoinTreeNode->transaction;
        addTransactionToTransactionListSorted(foundTransactionList,
                                              initTransaction(curTransaction->transactionId,
                                                              curTransaction->amount,
                                                              curTransaction->senderWalletId,
                                                              curTransaction->receiverWalletId,
                                                              curTransaction->datetimeS));
    }

    getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->receiverNode);
    getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->remainingNode);

    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF BITCOIN TREE ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF BITCOIN LIST ///////////////////////////////////////////////////////////////

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

void freeBitcoinListNode(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees, char shouldFreeTransactions) {
    if ((*bitcoinListNode) == NULL)
        return;

    if (shouldFreeBitcoinTrees == 1) {
        freeBitcoinTree(&(*bitcoinListNode)->bitcoinTree, shouldFreeTransactions);
    }
    (*bitcoinListNode)->nextNode = NULL;
    (*bitcoinListNode)->prevNode = NULL;
    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;

    return;
}

void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees, char shouldFreeTransactions) {
    if ((*bitcoinListNode) == NULL)
        return;

    freeBitcoinListNodeRec(&((*bitcoinListNode)->nextNode), shouldFreeBitcoinTrees, shouldFreeTransactions);

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeBitcoinListNode(bitcoinListNode, shouldFreeBitcoinTrees, shouldFreeTransactions);

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

void freeBitcoinList(BitcoinList** bitcoinList, char shouldFreeBitcoinTrees, char shouldFreeTransactions) {
    if ((*bitcoinList) == NULL)
        return;
    // Bucket* curBucket = (*bitcoinList)->firstBucket;

    freeBitcoinListNodeRec(&(*bitcoinList)->firstNode, shouldFreeBitcoinTrees, shouldFreeTransactions);

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
        printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d to BitcoinList\n" ANSI_COLOR_RESET, bitcoinId);
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
            printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d to BitcoinList\n" ANSI_COLOR_RESET, bitcoinId);
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
                    printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d to BitcoinList\n" ANSI_COLOR_RESET, bitcoinId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d to BitcoinList\n" ANSI_COLOR_RESET, bitcoinId);
                return curBitcoinListNode->nextNode;
            }

            curBitcoinListNode = curBitcoinListNode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

BitcoinListNode* addBitcoinListNodeToBitcoinListByPointer(BitcoinList* bitcoinList, BitcoinListNode* bitcoinListNode, char* receiverWalletId) {
    if (findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinListNode->bitcoinTree->bitcoinId) != NULL) {
        return NULL;
    }

    if (bitcoinList->size == 0) {
        bitcoinList->firstNode = initBitcoinListNode(bitcoinListNode->bitcoinTree);

        bitcoinList->size++;
        printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d by pointer to BitcoinList of %s\n" ANSI_COLOR_RESET, bitcoinListNode->bitcoinTree->bitcoinId, receiverWalletId);
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
            printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d by pointer to BitcoinList of %s\n" ANSI_COLOR_RESET, bitcoinListNode->bitcoinTree->bitcoinId, receiverWalletId);
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
                    printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d by pointer to BitcoinList of %s\n" ANSI_COLOR_RESET, bitcoinListNode->bitcoinTree->bitcoinId, receiverWalletId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(bitcoinListNode->bitcoinTree);
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf(ANSI_COLOR_BLUE "Inserted bitcoin with id %d by pointer to BitcoinList of %s\n" ANSI_COLOR_RESET, bitcoinListNode->bitcoinTree->bitcoinId, receiverWalletId);
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

    freeBitcoinListNode(&bitcoinListNodeToDelete, 0, 0);
    bitcoinList->size--;

    return 0;
}

//////////////////////////////////////////////////////////////////////////////// END OF BITCOIN LIST ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF HIGH LEVEL FUNCTIONS ///////////////////////////////////////////////////////////////

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int* amount) {
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
    TransactionList* foundTransactionsList = initTransactionList(NULL);

    getTransactionsNumOfBitcoin(&transactionsNum, foundTransactionsList, foundBitcoinListNode->bitcoinTree->rootNode);

    freeTransactionList(&foundTransactionsList, 1);

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
//////////////////////////////////////////////////////////////////////////////// END OF HIGH LEVEL FUNCTIONS ///////////////////////////////////////////////////////////////