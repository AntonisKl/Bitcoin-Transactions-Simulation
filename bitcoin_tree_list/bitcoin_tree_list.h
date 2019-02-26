#ifndef TREE_H
#define TREE_H

#include "../utils/utils.h"

// typedef struct BitcoinTreeNode {
//     char* walletId;
//     int amount;
//     struct BitcoinTreeNode *receiverNode, *remainingNode;  // remainingNode may be NULL if remaining amount is 0
// } BitcoinTreeNode;

// typedef struct BitcoinTree {
//     BitcoinTreeNode* rootNode;
//     int bitcoinId;
//     unsigned int size;
//     struct BitcoinTree* nextBitcoinTree;
// } BitcoinTree;

// typedef struct BitcoinList {
//     BitcoinTree *firstBitcoinTree;
//     unsigned int size;
// } BitcoinList;

BitcoinTree* initBitcoinTree();

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount);

BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount);

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode);

void freeBitcoinTree(BitcoinTree* bitcoinTree);

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTree(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, char* senderId, char* receiverId, int amount);

///////////////////////////////////////////////

BitcoinList* initBitcoinList();

void freeBitcoinTreeRec(BitcoinTree** bitcoinTree);

void freeBitcoinList(BitcoinList** bitcoinList);

BitcoinTree* findBitcoinTreeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

BitcoinTree* addBitcoinTreeToBitcoinList(BitcoinList* bitcoinList, int bitcoinId, int startAmount);


#endif