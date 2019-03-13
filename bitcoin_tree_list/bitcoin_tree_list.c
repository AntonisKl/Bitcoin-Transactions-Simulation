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

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTreeRec(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    if (nextBitcoinTreeNode == NULL || (strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 && nextBitcoinTreeNode->amount == amount))
        return nextBitcoinTreeNode;

    return findBitcoinTreeNodeOfBitcoinTreeRec(walletId, amount, nextBitcoinTreeNode->receiverNode);
    return findBitcoinTreeNodeOfBitcoinTreeRec(walletId, amount, nextBitcoinTreeNode->remainingNode);
}

BitcoinTreeNode* findBitcoinTreeNodeForLogRec(char* walletId, int bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    if (nextBitcoinTreeNode == NULL || (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0 && nextBitcoinTreeNode->amount >= bitcoinAmount))
        return nextBitcoinTreeNode;

    return findBitcoinTreeNodeForLogRec(walletId, bitcoinAmount, nextBitcoinTreeNode->receiverNode);
    return findBitcoinTreeNodeForLogRec(walletId, bitcoinAmount, nextBitcoinTreeNode->remainingNode);
}

void getCurrentBitcoinBalanceOfWalletIdRec(char* walletId, int* balance, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    if (nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0) {
        (*balance) += nextBitcoinTreeNode->amount;
    }

    getCurrentBitcoinBalanceOfWalletIdRec(walletId, balance, nextBitcoinTreeNode->receiverNode);
    getCurrentBitcoinBalanceOfWalletIdRec(walletId, balance, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsNumOfBitcoinRec(int* transactionsNum, TransactionList* foundTransactionsList, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    Transaction* curTransaction = nextBitcoinTreeNode->transaction;

    if (curTransaction != NULL && addTransactionToTransactionListSorted(foundTransactionsList,
                                                                        initTransaction(curTransaction->transactionId,
                                                                                        curTransaction->amount,
                                                                                        curTransaction->senderWalletId,
                                                                                        curTransaction->receiverWalletId,
                                                                                        curTransaction->datetimeS)) != NULL) {
        // only if there is a transaction in the curent node and this transaction is not the same with a transaction that we have already encountered
        // increment the number of found transactions
        (*transactionsNum)++;
    }

    getTransactionsNumOfBitcoinRec(transactionsNum, foundTransactionsList, nextBitcoinTreeNode->receiverNode);
    getTransactionsNumOfBitcoinRec(transactionsNum, foundTransactionsList, nextBitcoinTreeNode->remainingNode);
}

void handleLogRec(char* walletId, Transaction* transaction, int* amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/) {
    if (nextBitcoinTreeNode == NULL || (*amount) <= 0)  // (*bitcoinAmount) <= 0 means that transaction has been compeleted
        return;

    int receiverAmount = 0, remainingAmount = 0;
    if (nextBitcoinTreeNode->amount > 0 && nextBitcoinTreeNode->receiverNode == NULL && nextBitcoinTreeNode->remainingNode == NULL && strcmp(nextBitcoinTreeNode->walletId, walletId) == 0) {
        receiverAmount = (nextBitcoinTreeNode->amount - (*amount)) >= 0 ? (*amount) : nextBitcoinTreeNode->amount;  // calculate receiver amount of dollars
        remainingAmount = nextBitcoinTreeNode->amount - receiverAmount;                                             // calculate remaining amount of dollars

        nextBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId,
                                                                receiverAmount,
                                                                transaction);
        nextBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId,
                                                                 remainingAmount,
                                                                 NULL);
    }

    (*amount) -= receiverAmount;  // update remaining amount of dollars for transation to be completed

    handleLogRec(walletId, transaction, amount, nextBitcoinTreeNode->receiverNode);
    handleLogRec(walletId, transaction, amount, nextBitcoinTreeNode->remainingNode);

    return;
}

void getTransactionsOfBitcoinTreeRec(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode) {
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

    getTransactionsOfBitcoinTreeRec(foundTransactionList, nextBitcoinTreeNode->receiverNode);
    getTransactionsOfBitcoinTreeRec(foundTransactionList, nextBitcoinTreeNode->remainingNode);

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

    freeBitcoinListNode(bitcoinListNode, shouldFreeBitcoinTrees, shouldFreeTransactions);

    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;
    return;
}

void freeBitcoinList(BitcoinList** bitcoinList, char shouldFreeBitcoinTrees, char shouldFreeTransactions) {
    if ((*bitcoinList) == NULL)
        return;

    freeBitcoinListNodeRec(&(*bitcoinList)->firstNode, shouldFreeBitcoinTrees, shouldFreeTransactions);

    free(*bitcoinList);
    (*bitcoinList) = NULL;

    return;
}

BitcoinListNode* findBitcoinListNodeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return NULL;

    BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

    while (curBitcoinListNode != NULL) {
        if (curBitcoinListNode->bitcoinTree->bitcoinId == bitcoinId) {
            return curBitcoinListNode;
        } else if (bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {  // no need for searching further since the list is sorted by bitcoinId
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
        printf("Inserted bitcoin with id %d to BitcoinList\n", bitcoinId);
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
            printf("Inserted bitcoin with id %d to BitcoinList\n", bitcoinId);
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
                    printf("Inserted bitcoin with id %d to BitcoinList\n", bitcoinId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(initBitcoinTree(startAmount, walletId, bitcoinId));
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf("Inserted bitcoin with id %d to BitcoinList\n", bitcoinId);
                return curBitcoinListNode->nextNode;
            }

            curBitcoinListNode = curBitcoinListNode->nextNode;
        }
    }

    return NULL;  // not normal behavior
}

BitcoinListNode* addBitcoinListNodeToBitcoinListByBitcoinTreePointer(BitcoinList* bitcoinList, BitcoinTree* bitcoinTree, char* receiverWalletId) {
    if (findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinTree->bitcoinId) != NULL) {
        return NULL;
    }

    if (bitcoinList->size == 0) {
        bitcoinList->firstNode = initBitcoinListNode(bitcoinTree);

        bitcoinList->size++;
        printf("Inserted bitcoin with id %d by pointer to BitcoinList of %s\n", bitcoinTree->bitcoinId, receiverWalletId);
        return bitcoinList->firstNode;
    } else {
        BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

        if (bitcoinTree->bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {
            // insert at start
            BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(bitcoinTree);
            bitcoinListNodeToInsert->nextNode = curBitcoinListNode;

            curBitcoinListNode->prevNode = bitcoinListNodeToInsert;
            bitcoinList->firstNode = bitcoinListNodeToInsert;
            bitcoinList->size++;
            printf("Inserted bitcoin with id %d by pointer to BitcoinList of %s\n", bitcoinTree->bitcoinId, receiverWalletId);
            return bitcoinList->firstNode;
        }
        while (curBitcoinListNode != NULL) {
            if (curBitcoinListNode->nextNode != NULL) {
                if (bitcoinTree->bitcoinId < curBitcoinListNode->nextNode->bitcoinTree->bitcoinId) {
                    BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(bitcoinTree);
                    bitcoinListNodeToInsert->prevNode = curBitcoinListNode;
                    bitcoinListNodeToInsert->nextNode = curBitcoinListNode->nextNode;

                    curBitcoinListNode->nextNode->prevNode = bitcoinListNodeToInsert;
                    curBitcoinListNode->nextNode = bitcoinListNodeToInsert;
                    bitcoinList->size++;
                    printf("Inserted bitcoin with id %d by pointer to BitcoinList of %s\n", bitcoinTree->bitcoinId, receiverWalletId);
                    return curBitcoinListNode->nextNode;
                }
            } else {
                // insert at the end
                curBitcoinListNode->nextNode = initBitcoinListNode(bitcoinTree);
                curBitcoinListNode->nextNode->prevNode = curBitcoinListNode;

                bitcoinList->size++;
                printf("Inserted bitcoin with id %d by pointer to BitcoinList of %s\n", bitcoinTree->bitcoinId, receiverWalletId);
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
    handleLogRec(transaction->senderWalletId, transaction, amount, bitcoinTree->rootNode);

    return 0;
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

int getCurrentBitcoinBalanceOfWalletId(BitcoinTree* bitcoinTree, char* walletId) {
    int balance = 0;
    getCurrentBitcoinBalanceOfWalletIdRec(walletId, &balance, bitcoinTree->rootNode);

    return balance;
}

int getTransactionsNumOfBitcoin(BitcoinList* bitcoinList, int bitcoinId) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return -1;
    }

    int transactionsNum = 0;
    TransactionList* foundTransactionsList = initTransactionList(NULL);

    getTransactionsNumOfBitcoinRec(&transactionsNum, foundTransactionsList, foundBitcoinListNode->bitcoinTree->rootNode);

    freeTransactionList(&foundTransactionsList, 1);

    return transactionsNum;
}

void getTransactionsOfBitcoin(BitcoinList* bitcoinList, int bitcoinId, TransactionList* foundTransactionList) {
    BitcoinListNode* foundBitcoinListNode = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);
    if (foundBitcoinListNode == NULL) {
        printf("Bitcoin with id %d not found in bitcoins' list\n", bitcoinId);
        return;
    }

    getTransactionsOfBitcoinTreeRec(foundTransactionList, foundBitcoinListNode->bitcoinTree->rootNode);

    return;
}
//////////////////////////////////////////////////////////////////////////////// END OF HIGH LEVEL FUNCTIONS ///////////////////////////////////////////////////////////////