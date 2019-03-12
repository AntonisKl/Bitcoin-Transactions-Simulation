#ifndef BITCOIN_TREE_LIST_H
#define BITCOIN_TREE_LIST_H

#include "../hashtable/hashtable.h"
#include "../utils/utils.h"

typedef struct Transaction Transaction;          // forward declaration for compilation
typedef struct TransactionList TransactionList;  // forward declaration for compilation

typedef struct BitcoinTreeNode {
    char* walletId;            // walletId: receiver/remaining
    int amount;                // dollars
    Transaction* transaction;  // transaction is NULL on remainingNodes
    struct BitcoinTreeNode *receiverNode, *remainingNode;
} BitcoinTreeNode;

typedef struct BitcoinTree {
    BitcoinTreeNode* rootNode;
    int bitcoinId;      // bitcoinId: id of bitcoin that corresponds to the bitcoin tree
    unsigned int size;  // size: number of tree nodes
} BitcoinTree;

typedef struct BitcoinListNode {
    BitcoinTree* bitcoinTree;
    struct BitcoinListNode *nextNode, *prevNode;
} BitcoinListNode;

// BitcoinList: sorted list of bitcoin list nodes by ascending bitcoinId of bitcoin trees
typedef struct BitcoinList {
    BitcoinListNode* firstNode;
    unsigned int size;
} BitcoinList;

// Bitcoin Tree

BitcoinTree* initBitcoinTree(int startAmount, char* walletId, int bitcoinId);

BitcoinTreeNode* initBitcoinTreeNode(char* walletId, int amount, Transaction* transaction);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeBitcoinTreeNodeRec(BitcoinTreeNode** bitcoinTreeNode, char shouldFreeTransactions);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
void freeBitcoinTree(BitcoinTree** bitcoinTree, char shouldFreeTransactions);

// returns NULL if not found
BitcoinTreeNode* findBitcoinTreeNodeOfBitcoinTreeRec(char* walletId, int amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

// returns NULL if not found
BitcoinTreeNode* findBitcoinTreeNodeForLogRec(char* walletId, int bitcoinAmount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

// traverses bitcoin tree and calculates the current balance of walletId of a particular bitcoin
void getCurrentBitcoinBalanceOfWalletIdRec(char* walletId, int* balance, BitcoinTreeNode* nextBitcoinTreeNode);

// traverses bitcoin tree and calculates the number of transactions in which a particular bitcoin has been used
void getTransactionsNumOfBitcoinRec(int* transactionsNum, TransactionList* foundTransactionsList, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

// traverses bitcoin tree and handles the logging of a transaction in a particular bitcoin tree
void handleLogRec(char* walletId, Transaction* transaction, int* amount, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

// traverses bitcoin tree and adds (in a sorted manner) to foundTransactionList the found transactions in which a particular bitcoin has been used
void getTransactionsOfBitcoinTreeRec(TransactionList* foundTransactionList, BitcoinTreeNode* nextBitcoinTreeNode /*= rootNode initially*/);

// End

// Bitcoin List

BitcoinList* initBitcoinList();

BitcoinListNode* initBitcoinListNode(BitcoinTree* bitcoinTree);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
// if shoudFreeBitcoinTrees == 1: frees bitcoin trees as well
// if shoudFreeBitcoinTrees == 0: does not free bitcoin trees
void freeBitcoinListNode(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees, char shouldFreeTransactions);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
// if shoudFreeBitcoinTrees == 1: frees bitcoin trees as well
// if shoudFreeBitcoinTrees == 0: does not free bitcoin trees
void freeBitcoinListNodeRec(BitcoinListNode** bitcoinListNode, char shouldFreeBitcoinTrees, char shouldFreeTransactions);

// if shoudFreeTransactions == 1: frees transactions as well
// if shoudFreeTransactions == 0: does not free transactions
// if shoudFreeBitcoinTrees == 1: frees bitcoin trees as well
// if shoudFreeBitcoinTrees == 0: does not free bitcoin trees
void freeBitcoinList(BitcoinList** bitcoinList, char shouldFreeBitcoinTrees, char shouldFreeTransactions);

// returns NULL if not found
// takes advantage of sorting order of bitcoinList
BitcoinListNode* findBitcoinListNodeInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

// maintains sorting order of bitcoinList
BitcoinListNode* addBitcoinListNodeToBitcoinList(BitcoinList* bitcoinList, char* walletId, int bitcoinId, int startAmount);

// adds the bitcoin tree to the bitcoinList by pointer
BitcoinListNode* addBitcoinListNodeToBitcoinListByBitcoinTreePointer(BitcoinList* bitcoinList, BitcoinTree* bitcoinTree, char* receiverWalletId);

// deletes bitcoin list node from bitcoinList and frees unnecessary memory
int deleteBitcoinListNodeFromBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

// End

// High level functions

// handles logging of transaction to a particular bitcoinTree
// amount: amount in dollars that are remaining in order for the transaction to be completed
int addLogToBitcoinTree(BitcoinTree* bitcoinTree, Transaction* transaction, int* amount);

// returns NULL if bitcoin list node with bitcoin id bitcoinId is not found in bitcoinList
// else, returns sorted transaction list by timestamp
TransactionList* findTransactionsInBitcoinList(BitcoinList* bitcoinList, int bitcoinId);

// returns -1 if bitcoin list node with bitcoin id bitcoinId is not found in bitcoinList
int getUnspentAmountOfBitcoin(BitcoinList* bitcoinList, int bitcoinId);

// returns balance of wallet id of a particular bitcoinTree
int getCurrentBitcoinBalanceOfWalletId(BitcoinTree* bitcoinTree, char* walletId);

// returns -1 if bitcoin list node with bitcoin id bitcoinId is not found in bitcoinList
// else, returns the number of transactions in which the bitcoin with id bitcoinId was involved
int getTransactionsNumOfBitcoin(BitcoinList* bitcoinList, int bitcoinId);

// fills foundTransactionList with the transactions in which the bitcoin with id bitcoinId was involved
void getTransactionsOfBitcoin(BitcoinList* bitcoinList, int bitcoinId, TransactionList* foundTransactionList);

// End

#endif