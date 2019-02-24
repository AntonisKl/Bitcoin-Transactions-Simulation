#include "tree.h"

Tree* initTree() {
    Tree* tree = malloc(sizeof(Tree));
    tree->rootNode = NULL;
    tree->size = 0;

    return tree;
}

TreeNode* initTreeNode(char* walletId, int amount) {
    TreeNode* treeNode = malloc(sizeof(TreeNode));
    treeNode->walletId = malloc(MAX_WALLET_ID_SIZE);
    strcpy(treeNode->walletId, walletId);
    treeNode->amount = amount;

    return treeNode;
}

void freeTreeNodeRec(TreeNode** treeNode) {
    if (treeNode == NULL)
        return;

    freeTreeNodeRec((*treeNode)->receiverNode);
    freeTreeNodeRec((*treeNode)->remainingNode);

    free((*treeNode)->walletId);
    (*treeNode)->walletId = NULL;
    free(*treeNode);
    (*treeNode) = NULL;

    return;
}

void freeTree(Tree* tree) {
    if (tree == NULL)
        return;

    freeTreeNodeRec(tree->rootNode);
}

TreeNode* findTreeNodeOfTree(char* walletId, int amount, TreeNode* nextTreeNode /*= rootNode initially*/) {
    if (nextTreeNode == NULL || (strcmp(nextTreeNode->walletId) == 0 && nextTreeNode->amount == amount))
        return nextTreeNode;

    return findTreeNodeOfTree(walletId, amount, nextTreeNode->receiverNode);
    return findTreeNodeOfTree(walletId, amount, nextTreeNode->remainingNode);
}

int addLogToTree(Tree* tree, char* senderId, char* receiverId, int amount) {
    TreeNode* foundTreeNode = findTreeNodeOfTree(senderId, amount, tree->rootNode);

    if (foundTreeNode == NULL) {
        printf("Sender node not found\n");
        return 1;
    }

    if (foundTreeNode->amount < amount) {
        printf("Insufficient bitcoin amount\n");
        return 1;
    }

    foundTreeNode->receiverNode = initTreeNode(receiverId, amount);

    if (foundTreeNode->amount > amount) {
        foundTreeNode->remainingNode = initTreeNode(senderId, foundTreeNode->amount - amount);
    }

    return 0;
}