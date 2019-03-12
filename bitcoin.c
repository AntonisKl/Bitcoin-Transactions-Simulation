#include "bitcoin_tree_list/bitcoin_tree_list.h"
#include "hashtable/hashtable.h"

static HashTable *senderHashTable, *receiverHashTable;
static BitcoinList* bitcoinList;
static WalletList* walletList;

void intHandler(int dummy) {
    freeMemory(&senderHashTable, &receiverHashTable, &bitcoinList, &walletList);
    exit(1);
}

int main(int argc, char** argv) {
    signal(SIGINT, intHandler);

    srand(time(NULL));

    char *bitcoinBalancesFileName, *transactionsFileName;
    int bitcoinValue, senderHashTableSize, receiverHashTableSize, bucketSizeBytes;
    time_t lastTransactionTimestamp = 0;

    handleArgs(argc, argv, &bitcoinBalancesFileName, &transactionsFileName, &bitcoinValue, &senderHashTableSize, &receiverHashTableSize, &bucketSizeBytes);

    int bucketSize = bucketSizeBytes / sizeof(Transaction*);

    senderHashTable = initHashTable(senderHashTableSize, bucketSize);
    receiverHashTable = initHashTable(receiverHashTableSize, bucketSize);

    handleBitcoinBalancesFile(bitcoinBalancesFileName, &walletList, &bitcoinList, bitcoinValue);

    // BitcoinListNode* curNode = bitcoinList->firstNode;
    // while (curNode != NULL) {
    //     printf("curnode bitcoin id: %d\n", curNode->bitcoinTree->bitcoinId);
    //     curNode = curNode->nextNode;
    // }

    handleTransactionsFile(transactionsFileName, senderHashTable, receiverHashTable, bitcoinList, walletList, &lastTransactionTimestamp);
    // printf("LAST TIMESTAMP: %ld\n", lastTransactionTimestamp);

    // printTransactionsInHashTable(senderHashTable);

    handleInput(walletList, senderHashTable, receiverHashTable, bitcoinList, &lastTransactionTimestamp);

    freeMemory(&senderHashTable, &receiverHashTable, &bitcoinList, &walletList);
}