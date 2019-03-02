#include "bitcoin_tree_list.h"

BitcoinTree* initBitcoinTree(int startAmount) {
    BitcoinTree* bitcoinTree = (BitcoinTree*)malloc(sizeof(BitcoinTree));
    bitcoinTree->rootNode = initBitCoinTreeRootNode(startAmount);
    bitcoinTree->size = 0;
    // bitcoinTree->nextBitcoinTree = NULL;
    // bitcoinTree->prevBitcoinTree = NULL;

    return bitcoinTree;
}

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction) {  // transaction is NULL for remainingNodes
    BitcoinTreeNode* bitcoinTreeNode = malloc(sizeof(BitcoinTreeNode));
    bitcoinTreeNode->walletId = malloc(MAX_WALLET_ID_SIZE);
    strcpy(bitcoinTreeNode->walletId, walletId);
    bitcoinTreeNode->transaction = transaction;
    bitcoinTreeNode->amount = amount;
    bitcoinTreeNode->receiverNode = NULL;
    bitcoinTreeNode->remainingNode = NULL;

    return bitcoinTreeNode;
}

BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount) {
    BitcoinTreeNode* bitcoinTreeNode = malloc(sizeof(BitcoinTreeNode));
    bitcoinTreeNode->walletId = NULL;
    bitcoinTreeNode->amount = startAmount;
    bitcoinTreeNode->receiverNode = NULL;
    bitcoinTreeNode->remainingNode = NULL;

    return bitcoinTreeNode;
}

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode) {
    if (bitcoinTreeNode == NULL)
        return;

    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->receiverNode);
    freeBitcoinTreeNodeRec(&(*bitcoinTreeNode)->remainingNode);

    free((*bitcoinTreeNode)->walletId);
    (*bitcoinTreeNode)->walletId = NULL;
    if ((*bitcoinTreeNode)->transaction != NULL) {
        free((*bitcoinTreeNode)->transaction);
        (*bitcoinTreeNode)->transaction = NULL;
    }
    free(*bitcoinTreeNode);
    (*bitcoinTreeNode) = NULL;

    return;
}

void freeBitcoinTree(BitcoinTree** bitcoinTree) {
    if (bitcoinTree == NULL)
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

void freeBitcoinListNode(BitcoinListNode** bitcoinListNode) {
    if ((*bitcoinListNode) == NULL)
        return;

    freeBitcoinTree(&(*bitcoinListNode)->bitcoinTree);
    (*bitcoinListNode)->bitcoinTree = NULL;
    (*bitcoinListNode)->nextNode = NULL;
    (*bitcoinListNode)->prevNode = NULL;
    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;

    return;
}

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

void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode) {
    if ((*bitcoinListNode) == NULL)
        return;

    freeBitcoinListNodeRec(&((*bitcoinListNode)->nextNode));

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeBitcoinListNode(bitcoinListNode);

    // free((*bucket)->name);
    // (*bucket)->name = NULL;
    free(*bitcoinListNode);
    (*bitcoinListNode) = NULL;
    return;
}

void freeBitcoinList(BitcoinList** bitcoinList) {
    if ((*bitcoinList) == NULL)
        return;
    // Bucket* curBucket = (*bitcoinList)->firstBucket;

    freeBitcoinListNodeRec(&(*bitcoinList)->firstNode);
    (*bitcoinList)->firstNode = NULL;

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
        } else if (bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curBitcoinListNode = curBitcoinListNode->nextNode;
    }
    return NULL;
}

BitcoinListNode* addBitcoinListNodeToBitcoinList(BitcoinList* bitcoinList, int bitcoinId, int startAmount) {
    if (bitcoinList->size == 0) {
        bitcoinList->firstNode = initBitcoinListNode(initBitcoinTree(startAmount));

        bitcoinList->size++;
        printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
        return bitcoinList->firstNode;
    } else {
        BitcoinListNode* curBitcoinListNode = bitcoinList->firstNode;

        if (bitcoinId < curBitcoinListNode->bitcoinTree->bitcoinId) {
            // insert at start
            BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(initBitcoinTree(startAmount));
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
                    BitcoinListNode* bitcoinListNodeToInsert = initBitcoinListNode(initBitcoinTree(startAmount));
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
                curBitcoinListNode->nextNode = initBitcoinListNode(initBitcoinTree(startAmount));
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

int deleteBitcoinListNodeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return -1;

    BitcoinListNode* bitcoinListNodeToDelete = findBitcoinListNodeInBitcoinList(bitcoinList, bitcoinId);

    if (bitcoinListNodeToDelete == NULL) {
        printf("Name node with name %d not found in names' list\n", bitcoinId);
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

    freeBitcoinListNode(&bitcoinListNodeToDelete);
    bitcoinList->size--;

    return 0;
}

///////////////////////////////////////////////

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction) {
    BitcoinTreeNode* foundBitcoinTreeNode = findBitcoinTreeNodeForLog(transaction->receiverWalletId, transaction->amount, bitcoinTree->rootNode);

    if (foundBitcoinTreeNode == NULL) {
        printf("Suitable node not found or insufficient bitcoin amount\n");
        return 1;
    }

    // if (foundBitcoinTreeNode->amount < amount) {
    //     printf("Insufficient bitcoin amount\n");
    //     return 1;
    // }

    foundBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId, transaction->amount, transaction);

    if (foundBitcoinTreeNode->amount > transaction->amount) {
        foundBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId, foundBitcoinTreeNode->amount - transaction->amount, NULL);
    }

    return 0;
}

void getTransactionsOfBitcoinTree(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode) {
    if (nextBitcoinTreeNode == NULL)
        return;

    if (nextBitcoinTreeNode->transaction != NULL) {
        addTransactionToTransactionListSorted(foundTransactionList, nextBitcoinTreeNode->transaction);
    }

    return getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->receiverNode);
    return getTransactionsOfBitcoinTree(foundTransactionList, nextBitcoinTreeNode->remainingNode);
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