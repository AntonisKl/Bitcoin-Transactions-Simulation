#ifndef TREE_H
#define TREE_H

#include "../utils/utils.h"

typedef struct TreeNode {
    char* walletId;
    int amount;
    struct TreeNode *receiverNode, *remainingNode;  // remainingNode may be NULL if remaining amount is 0
} TreeNode;

typedef struct Tree {
    struct TreeNode* rootNode;
    int bitCoinId;
    unsigned int size;
} Tree;

Tree* initTree();

TreeNode* initTreeNode(char* walletId, int amount);

void freeTreeNodeRec(TreeNode** treeNode);

void freeTree(Tree* tree);

TreeNode* findTreeNodeOfTree(char* walletId, int amount, TreeNode* nextTreeNode /*= rootNode initially*/);

int addLogToTree(Tree* tree, char* senderId, char* receiverId, int amount);

#endif