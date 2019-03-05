#ifndef BITCOIN_TREE_LIST_H
#define BITCOIN_TREE_LIST_H

#include "../utils/utils.h"
#include "../hashtable/hashtable.h"

typedef struct Transaction Transaction; // forward declaration for compilation
typedef struct TransactionList TransactionList; // forward declaration for compilation

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
} BitcoinTree;

typedef struct BitcoinListNode {
    BitcoinTree* bitcoinTree;
    struct BitcoinListNode* nextNode, *prevNode;
} BitcoinListNode;

typedef struct BitcoinList { // all the bitcoins that exist
    BitcoinListNode *firstNode;
    unsigned int size;
} BitcoinList;

BitcoinTree* initBitcoinTree(int startAmount, char* walletId);

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction);

// BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount);

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode);

void freeBitcoinTree(BitcoinTree** bitcoinTree);

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTree(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

BitcoinTreeNode* findBitcoinTreeNodeForLog(char* walletId, int bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

void getTotalBitcoinBalanceOfWalletId(char* walletId, int* balance, BitcoinTreeNode* nextBitcoinTreeNode);

void handleLog(char* walletId, Transaction* transaction, int* bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

void getTransactionsOfBitcoinTree(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode);

///////////////////////////////////////////////

BitcoinList* initBitcoinList();

BitcoinListNode* initBitcoinListNode(BitcoinTree* bitcoinTree);

void freeBitcoinListNode(BitcoinListNode** bitcoinListNode);

void freeBitcoinListNodeNoTree(BitcoinListNode** bitcoinListNode);

void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode);

void freeBitcoinListNodeRecNoTree(BitcoinListNode** bitcoinListNode);

void freeBitcoinList(BitcoinList** bitcoinList);

void freeBitcoinListNoTrees(BitcoinList** bitcoinList);

BitcoinListNode* findBitcoinListNodeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

BitcoinListNode* addBitcoinListNodeToBitcoinList(BitcoinList* bitcoinList, char* walletId, int bitcoinId, int startAmount);

BitcoinListNode* addBitcoinListNodeToBitcoinListByPointer(BitcoinList* bitcoinList, BitcoinListNode* bitcoinListNode);

int deleteBitcoinListNodeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

///////////////////////////////////////////////
// high level functions

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int* amount);

TransactionList* findTransactionsInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

int getUnspentAmountOfBitcoin(BitcoinList* bitcoinList, int bitcoinId);

int getUnspentAmountOfBitcoinByTree(BitcoinTree* bitcoinTree);

int getCurrentBitcoinBalanceOfWalletId(BitcoinTree* bitcoinTree, char* walletId);

#endif