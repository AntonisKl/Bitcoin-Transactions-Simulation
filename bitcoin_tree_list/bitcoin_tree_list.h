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

// Bitcoin Tree

BitcoinTree* initBitcoinTree(int startAmount, char* walletId, int bitcoinId);

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction);

// BitcoinTreeNode* initBitCoinTreeRootNode(int startAmount);

void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode, char shouldFreeTransactions);

void freeBitcoinTree(BitcoinTree** bitcoinTree, char shouldFreeTransactions);

BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTree(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

BitcoinTreeNode* findBitcoinTreeNodeForLog(char* walletId, int bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

void getTotalBitcoinBalanceOfWalletId(char* walletId, int* balance, BitcoinTreeNode* nextBitcoinTreeNode);

void getTransactionsNumOfBitcoin(int* transactionsNum, TransactionList* foundTransactionsList, BitcoinTreeNode* nextBitcoinTreeNode);

void handleLog(char* walletId, Transaction* transaction, int* bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

void getTransactionsOfBitcoinTree(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode);

// End

// Bitcoin List

BitcoinList* initBitcoinList();

BitcoinListNode* initBitcoinListNode(BitcoinTree* bitcoinTree);

void freeBitcoinListNode(BitcoinListNode** bitcoinListNode,  char shouldFreeBitcoinTrees, char shouldFreeTransactions);

void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees, char shouldFreeTransactions);

void freeBitcoinList(BitcoinList** bitcoinList,  char shouldFreeBitcoinTrees, char shouldFreeTransactions);

BitcoinListNode* findBitcoinListNodeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

BitcoinListNode* addBitcoinListNodeToBitcoinList(BitcoinList* bitcoinList, char* walletId, int bitcoinId, int startAmount);

BitcoinListNode* addBitcoinListNodeToBitcoinListByPointer(BitcoinList* bitcoinList, BitcoinListNode* bitcoinListNode, char* receiverWalletId);

int deleteBitcoinListNodeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

// End

// High level functions

int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int* amount);

TransactionList* findTransactionsInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

int getUnspentAmountOfBitcoin(BitcoinList* bitcoinList, int bitcoinId);

int getUnspentAmountOfBitcoinByTree(BitcoinTree* bitcoinTree);

int getCurrentBitcoinBalanceOfWalletId(BitcoinTree* bitcoinTree, char* walletId);

int getTransactionsNumOfBitcoinById(BitcoinList* bitcoinList, int bitcoinId);

void getTransactionsOfBitcoin(BitcoinList* bitcoinList, int bitcoinId, TransactionList* foundTransactionList);

// End

#endif