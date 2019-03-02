#ifndef BITCOIN_TREE_LIST_H
#define BITCOIN_TREE_LIST_H

#include "../utils/utils.h"
#include "../hashtable/hashtable.h"

typedef struct Transaction Transaction; // forward declaration for compilation

typedef struct BitcoinTreeNode {
    char* walletId;
    int amount;
    Transaction* transaction; // transaction is NULL on remainingNodes
    struct BitcoinTreeNode *receiverNode, *remainingNode;  // remainingNode may be NULL if remaining amount is 0
} BitcoinTreeNode;

typedef struct BitcoinTree {
    BitcoinTreeNode* rootNode;
    int bitcoinId;
    unsigned int size;
    struct BitcoinTree* nextBitcoinTree, *prevBitcoinTree;
} BitcoinTree;

typedef struct BitcoinList { // all the bitcoins that exist
    BitcoinTree *firstBitcoinTree;
    unsigned int size;
} BitcoinList;

BitcoinTree* initBitcoinTree();

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction);

BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount);

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode);

void freeBitcoinTree(BitcoinTree** bitcoinTree);

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTree(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int amount);

///////////////////////////////////////////////

BitcoinList* initBitcoinList();

void freeBitcoinTreeRec(BitcoinTree** bitcoinTree);

void freeBitcoinList(BitcoinList** bitcoinList);

BitcoinTree* findBitcoinTreeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

BitcoinTree* addBitcoinTreeToBitcoinList(BitcoinList* bitcoinList, int bitcoinId, int startAmount);

int deleteBitcoinTreeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

#endif