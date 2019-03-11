#include "wallet_list.h"


//////////////////////////////////////////////////////////////////////////////// START OF WALLET ///////////////////////////////////////////////////////////////

Wallet* initWallet(char* walletId, unsigned int balance, BitcoinList* bitcoinList) {
    Wallet* wallet = (Wallet*)malloc(sizeof(Wallet));

    wallet->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
    strcpy(wallet->walletId, walletId);
    wallet->bitcoinList = bitcoinList;
    wallet->balance = balance;  // dollars
    wallet->nextWallet = NULL;

    return wallet;
}

void freeWallet(Wallet** wallet) {
    if ((*wallet) == NULL)
        return;

    free((*wallet)->walletId);
    (*wallet)->walletId = NULL;

    freeBitcoinList(&(*wallet)->bitcoinList, 0, 0);  ////////////////////////////////// THIS CAUSES FREE ERROR

    (*wallet)->nextWallet = NULL;

    free(*wallet);
    (*wallet) = NULL;
}

void freeWalletRec(Wallet** wallet) {
    if ((*wallet) == NULL)
        return;

    freeWalletRec(&((*wallet)->nextWallet));

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeWallet(&(*wallet));

    return;
}

//////////////////////////////////////////////////////////////////////////////// END OF WALLET ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF WALLET LIST ///////////////////////////////////////////////////////////////

WalletList* initWalletList() {
    WalletList* walletList = (WalletList*)malloc(sizeof(WalletList));
    walletList->size = 0;
    walletList->firstWallet = NULL;

    return walletList;
}

void freeWalletList(WalletList** walletList) {
    if ((*walletList) == NULL)
        return;

    freeWalletRec(&(*walletList)->firstWallet);

    free(*walletList);
    (*walletList) = NULL;

    return;
}

Wallet* findWalletInWalletList(WalletList* walletList, char* walletId) {
    if (walletList == NULL)
        return NULL;

    Wallet* curWallet = walletList->firstWallet;

    // printf("first wallet id: |%s|\n", curWallet->walletId);
    while (curWallet != NULL) {
        // printf("|%s|\n", curWallet->walletId);
        if (strcmp(curWallet->walletId, walletId) == 0) {
            return curWallet;
        } else if (strcmp(walletId, curWallet->walletId) < 0) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curWallet = curWallet->nextWallet;
    }
    return NULL;
}

Wallet* addWalletToWalletList(WalletList* walletList, char* walletId, unsigned int balance, BitcoinList* bitcoinList) {
    if (walletList->size == 0) {
        walletList->firstWallet = initWallet(walletId, balance, bitcoinList);

        walletList->size++;
        printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
        return walletList->firstWallet;
    } else {
        Wallet* curWallet = walletList->firstWallet;

        if (strcmp(walletId, curWallet->walletId) < 0) {
            // insert at start
            Wallet* walletToInsert = initWallet(walletId, balance, bitcoinList);
            walletToInsert->nextWallet = curWallet;

            // curWallet->prevNode = walletToInsert;
            walletList->firstWallet = walletToInsert;
            walletList->size++;
            printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
            return walletList->firstWallet;
        }
        while (curWallet != NULL) {
            if (curWallet->nextWallet != NULL) {
                if (strcmp(walletId, curWallet->nextWallet->walletId) < 0) {
                    Wallet* walletToInsert = initWallet(walletId, balance, bitcoinList);
                    // walletToInsert->prevNode = curWallet;
                    walletToInsert->nextWallet = curWallet->nextWallet;

                    // curWallet->nextNode->prevNode = walletToInsert;
                    curWallet->nextWallet = walletToInsert;
                    walletList->size++;
                    printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
                    return curWallet->nextWallet;
                }
            } else {
                // insert at the end
                curWallet->nextWallet = initWallet(walletId, balance, bitcoinList);
                // curWallet->nextNode->prevNode = curWallet;

                walletList->size++;
                printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
                return curWallet->nextWallet;
            }

            curWallet = curWallet->nextWallet;
        }
    }

    return NULL;  // not normal behavior
}

//////////////////////////////////////////////////////////////////////////////// END OF WALLET LIST ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// START OF HIGH LEVEL FUNCTIONS ///////////////////////////////////////////////////////////////

int handleWalletToWalletTransfer(Wallet* senderWallet, Wallet* receiverWallet, Transaction* transaction) {
    int amountToSpend = transaction->amount;
    if (senderWallet->balance < amountToSpend) {
        printError("Insufficient balance\n");
        return -1;
    }
    //printf("handleWalletToWalletTransfer1\n");
    //printf("amountToSpend before: %d\n", amountToSpend);
    // addLogToBitcoinTree(senderWallet->bitcoinList->firstNode->bitcoinTree, transaction, &amountToSpend);
    // addBitcoinListNodeToBitcoinListByPointer(receiverWallet->bitcoinList,,
    //                                 prevAmountToSpend - amountToSpend);
    while (amountToSpend > 0) {
        int prevAmountToSpend = amountToSpend;
        addLogToBitcoinTree(senderWallet->bitcoinList->firstNode->bitcoinTree, transaction, &amountToSpend);
        printf(ANSI_COLOR_GREEN "Transferred %d dollars of bitcoin %d from %s to %s\n" ANSI_COLOR_RESET, prevAmountToSpend - amountToSpend,
               senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId, senderWallet->walletId, receiverWallet->walletId);
        //printf("11\n");
        addBitcoinListNodeToBitcoinListByPointer(receiverWallet->bitcoinList, senderWallet->bitcoinList->firstNode, receiverWallet->walletId);
        //printf("22\n");
        if (amountToSpend > 0) {
            deleteBitcoinListNodeFromBitcoinList(senderWallet->bitcoinList, senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId);
        }
        //printf("33 amountToSpend: %d\n", amountToSpend);
    }
    //printf("handleWalletToWalletTransfer2\n");

    if (getCurrentBitcoinBalanceOfWalletId(senderWallet->bitcoinList->firstNode->bitcoinTree, senderWallet->walletId) <= 0) {
        //printf("handleWalletToWalletTransfer3\n");
        deleteBitcoinListNodeFromBitcoinList(senderWallet->bitcoinList, senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId);
    }

    //printf("transaction amount: %d\n", transaction->amount);
    senderWallet->balance -= transaction->amount;
    receiverWallet->balance += transaction->amount;
    //printf("balances: %d, %d\n", senderWallet->balance, receiverWallet->balance);

    return 0;
}

int getCurrentBalanceOfWallet(WalletList* walletList, char* walletId) {
    Wallet* foundWallet = findWalletInWalletList(walletList, walletId);
    if (foundWallet == NULL) {
        printf("Wallet not found\n");
        return -1;
    }

    return foundWallet->balance;
}

//////////////////////////////////////////////////////////////////////////////// END OF HIGH LEVEL FUNCTIONS ///////////////////////////////////////////////////////////////
