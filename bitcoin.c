#include "bitcoin_tree_list/bitcoin_tree_list.h"
#include "hashtable/hashtable.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    char *bitcoinBalancesFileName, *transactionsFileName;
    int bitcoinValue, senderHashTableSize, receiverHashTableSize, bucketSizeBytes;
    time_t lastTransactionTimestamp = 0;

    handleArgs(argc, argv, &bitcoinBalancesFileName, &transactionsFileName, &bitcoinValue, &senderHashTableSize, &receiverHashTableSize, &bucketSizeBytes);

    int bucketSize = bucketSizeBytes / sizeof(Transaction*);

    HashTable *senderHashTable, *receiverHashTable;
    BitcoinList* bitcoinList;
    WalletList* walletList;

    handleBitcoinBalancesFile(bitcoinBalancesFileName, &walletList, &bitcoinList, bitcoinValue);
    handleTransactionsFile(transactionsFileName, &senderHashTable, &receiverHashTable, bitcoinList, walletList,
                           senderHashTableSize, receiverHashTableSize, bucketSize, &lastTransactionTimestamp);

    handleInput(walletList, senderHashTable, receiverHashTable, bitcoinList, &lastTransactionTimestamp);

    freeMemory(&senderHashTable, &receiverHashTable, &bitcoinList, &walletList);
}