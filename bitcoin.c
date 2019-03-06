#include "bitcoin_tree_list/bitcoin_tree_list.h"
#include "hashtable/hashtable.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    char *bitcoinBalancesFileName, *transactionsFileName;
    int bitcoinValue, senderHashTableSize, receiverHashTableSize, bucketSizeBytes;

    handleArgs(argc, argv, &bitcoinBalancesFileName, &transactionsFileName, &bitcoinValue, &senderHashTableSize, &receiverHashTableSize, &bucketSizeBytes);

    int bucketSize = bucketSizeBytes / sizeof(Transaction*);

    HashTable *senderHashTable, *receiverHashTable;
    BitcoinList* bitcoinList;
    WalletList* walletList;

    handleBitcoinBalancesFile(bitcoinBalancesFileName, &walletList, &bitcoinList, bitcoinValue);
    handleTransactionsFile(transactionsFileName, &senderHashTable, &receiverHashTable, bitcoinList, walletList,
                           senderHashTableSize, receiverHashTableSize, bucketSize);

    handleInput(walletList, senderHashTable, receiverHashTable, bitcoinList);

    freeMemory(&senderHashTable, &receiverHashTable, &bitcoinList, &walletList);
}