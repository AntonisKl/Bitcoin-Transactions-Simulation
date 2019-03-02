#include "bitcoin_tree_list.h"

BitcoinTree* initBitcoinTree(int startAmount) {
    BitcoinTree* bitcoinTree = (BitcoinTree*)malloc(sizeof(BitcoinTree));
    bitcoinTree->rootNode = initBitCoinTreeRootNode(startAmount);
    bitcoinTree->size = 0;
    bitcoinTree->nextBitcoinTree = NULL;
    bitcoinTree->prevBitcoinTree = NULL;

    return bitcoinTree;
}

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction) { // transaction is NULL for remainingNodes
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
    (*bitcoinTree)->nextBitcoinTree = NULL;
    (*bitcoinTree)->prevBitcoinTree = NULL;
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

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int amount) {
    BitcoinTreeNode* foundBitcoinTreeNode = findBitcoinTreeNodeOfBitcoinTree(transaction->receiverWalletId, amount, bitcoinTree->rootNode);

    if (foundBitcoinTreeNode == NULL) {
        printf("Sender node not found\n");
        return 1;
    }

    if (foundBitcoinTreeNode->amount < amount) {
        printf("Insufficient bitcoin amount\n");
        return 1;
    }

    foundBitcoinTreeNode->receiverNode = initBitcoinTreeNode(transaction->receiverWalletId, amount, transaction);

    if (foundBitcoinTreeNode->amount > amount) {
        foundBitcoinTreeNode->remainingNode = initBitcoinTreeNode(transaction->senderWalletId, foundBitcoinTreeNode->amount - amount, NULL);
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BitcoinList* initBitcoinList() {
    BitcoinList* bitcoinList = (BitcoinList*)malloc(sizeof(BitcoinList));
    bitcoinList->size = 0;
    bitcoinList->firstBitcoinTree = NULL;

    return bitcoinList;
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

void freeBitcoinTreeRec(BitcoinTree** bitcoinTree) {
    if ((*bitcoinTree) == NULL)
        return;

    freeBitcoinTreeRec(&((*bitcoinTree)->nextBitcoinTree));

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeBitcoinTree(bitcoinTree);

    // free((*bucket)->name);
    // (*bucket)->name = NULL;
    free(*bitcoinTree);
    (*bitcoinTree) = NULL;
    return;
}

void freeBitcoinList(BitcoinList** bitcoinList) {
    if ((*bitcoinList) == NULL)
        return;
    // Bucket* curBucket = (*bitcoinList)->firstBucket;

    freeBitcoinTreeRec(&(*bitcoinList)->firstBitcoinTree);
    (*bitcoinList)->firstBitcoinTree = NULL;

    free(*bitcoinList);
    (*bitcoinList) = NULL;

    return;
}

BitcoinTree* findBitcoinTreeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return NULL;

    BitcoinTree* curBitcoinTree = bitcoinList->firstBitcoinTree;

    while (curBitcoinTree != NULL) {
        if (curBitcoinTree->bitcoinId == bitcoinId) {
            return curBitcoinTree;
        } else if (bitcoinId < curBitcoinTree->bitcoinId) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curBitcoinTree = curBitcoinTree->nextBitcoinTree;
    }
    return NULL;
}

BitcoinTree* addBitcoinTreeToBitcoinList(BitcoinList* bitcoinList, int bitcoinId, int startAmount) {
    if (bitcoinList->size == 0) {
        bitcoinList->firstBitcoinTree = initBitcoinTree(startAmount);

        bitcoinList->size++;
        printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
        return bitcoinList->firstBitcoinTree;
    } else {
        BitcoinTree* curBitcoinTree = bitcoinList->firstBitcoinTree;

        if (bitcoinId < curBitcoinTree->bitcoinId) {
            // insert at start
            BitcoinTree* bitcoinTreeToInsert = initBitcoinTree(startAmount);
            bitcoinTreeToInsert->nextBitcoinTree = curBitcoinTree;

            curBitcoinTree->prevBitcoinTree = bitcoinTreeToInsert;
            bitcoinList->firstBitcoinTree = bitcoinTreeToInsert;
            bitcoinList->size++;
            printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
            return bitcoinList->firstBitcoinTree;
        }
        while (curBitcoinTree != NULL) {
            if (curBitcoinTree->nextBitcoinTree != NULL) {
                if (bitcoinId < curBitcoinTree->nextBitcoinTree->bitcoinId) {
                    BitcoinTree* bitcoinTreeToInsert = initBitcoinTree(startAmount);
                    bitcoinTreeToInsert->prevBitcoinTree = curBitcoinTree;
                    bitcoinTreeToInsert->nextBitcoinTree = curBitcoinTree->nextBitcoinTree;

                    curBitcoinTree->nextBitcoinTree->prevBitcoinTree = bitcoinTreeToInsert;
                    curBitcoinTree->nextBitcoinTree = bitcoinTreeToInsert;
                    bitcoinList->size++;
                    printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
                    return curBitcoinTree->nextBitcoinTree;
                }
            } else {
                // insert at the end
                curBitcoinTree->nextBitcoinTree = initBitcoinTree(startAmount);
                curBitcoinTree->nextBitcoinTree->prevBitcoinTree = curBitcoinTree;

                bitcoinList->size++;
                printf("Inserted |%d| to BitcoinList\n\n", bitcoinId);
                return curBitcoinTree->nextBitcoinTree;
            }

            curBitcoinTree = curBitcoinTree->nextBitcoinTree;
        }
    }

    return NULL;  // not normal behavior
}

int deleteBitcoinTreeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId) {
    if (bitcoinList == NULL)
        return -1;

    BitcoinTree* bitcoinTreeToDelete = findBitcoinTreeInBitcoinList(bitcoinList, bitcoinId);

    if (bitcoinTreeToDelete == NULL) {
        printf("Name node with name %d not found in names' list\n", bitcoinId);
        return -1;
    }

    if (bitcoinTreeToDelete->bitcoinId == bitcoinList->firstBitcoinTree->bitcoinId) {
        bitcoinList->firstBitcoinTree = bitcoinTreeToDelete->nextBitcoinTree;
    }
    if (bitcoinTreeToDelete->prevBitcoinTree != NULL) {
        bitcoinTreeToDelete->prevBitcoinTree->nextBitcoinTree = bitcoinTreeToDelete->nextBitcoinTree;
    }
    if (bitcoinTreeToDelete->nextBitcoinTree != NULL) {
        bitcoinTreeToDelete->nextBitcoinTree->prevBitcoinTree = bitcoinTreeToDelete->prevBitcoinTree;
    }

    freeBitcoinTree(&bitcoinTreeToDelete);
    bitcoinList->size--;

    return 0;
}
