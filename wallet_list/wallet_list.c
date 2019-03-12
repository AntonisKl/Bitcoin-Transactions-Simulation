#include "wallet_list.h"

//////////////////////////////////////////////////////////////////////////////// START OF WALLET ///////////////////////////////////////////////////////////////

Wallet* initWallet(char* walletId, unsigned int balance, BitcoinList* bitcoinList) {
    Wallet* wallet = (Wallet*)malloc(sizeof(Wallet));

    wallet->walletId = (char*)malloc(strlen(walletId) + 1);
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

    freeBitcoinList(&(*wallet)->bitcoinList, 0, 0);  // free bitcoin list without freeing the transactions and the bitcoin trees to avoid double free of memory

    (*wallet)->nextWallet = NULL;

    free(*wallet);
    (*wallet) = NULL;
}

void freeWalletRec(Wallet** wallet) {
    if ((*wallet) == NULL)
        return;

    freeWalletRec(&((*wallet)->nextWallet));

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

    while (curWallet != NULL) {
        if (strcmp(curWallet->walletId, walletId) == 0) {
            return curWallet;
        } else if (strcmp(walletId, curWallet->walletId) < 0) {  // no need for searching further since the list is sorted by walletId
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

            walletList->firstWallet = walletToInsert;
            walletList->size++;
            printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
            return walletList->firstWallet;
        }
        while (curWallet != NULL) {
            if (curWallet->nextWallet != NULL) {
                if (strcmp(walletId, curWallet->nextWallet->walletId) < 0) {
                    Wallet* walletToInsert = initWallet(walletId, balance, bitcoinList);
                    walletToInsert->nextWallet = curWallet->nextWallet;

                    curWallet->nextWallet = walletToInsert;
                    walletList->size++;
                    printf(ANSI_COLOR_MAGENTA "Inserted wallet with id %s to WalletList\n" ANSI_COLOR_RESET, walletId);
                    return curWallet->nextWallet;
                }
            } else {
                // insert at the end
                curWallet->nextWallet = initWallet(walletId, balance, bitcoinList);

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

    while (amountToSpend > 0) {  // while there is a remaining amount of dollars to complete the transaction
        int prevAmountToSpend = amountToSpend;
        // update the bitcoin tree of the first bitcoin list node of the wallet
        addLogToBitcoinTree(senderWallet->bitcoinList->firstNode->bitcoinTree, transaction, &amountToSpend);

        printf(ANSI_COLOR_GREEN "Transferred %d dollars of bitcoin %d from %s to %s\n" ANSI_COLOR_RESET, prevAmountToSpend - amountToSpend,
               senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId, senderWallet->walletId, receiverWallet->walletId);

        // add a bitcoin list node with the used bitcoin's tree to the bitcoin list of the receiver wallet
        addBitcoinListNodeToBitcoinListByBitcoinTreePointer(receiverWallet->bitcoinList, senderWallet->bitcoinList->firstNode->bitcoinTree, receiverWallet->walletId);
        if (amountToSpend > 0) {  // if there is a remaining amount to spend which means the used bitcoin of the sender wallet was used compeletely
            // delete the compeletely used bitcoin from sender wallet
            deleteBitcoinListNodeFromBitcoinList(senderWallet->bitcoinList, senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId);
        }
    }

    if (getCurrentBitcoinBalanceOfWalletId(senderWallet->bitcoinList->firstNode->bitcoinTree, senderWallet->walletId) <= 0) {  // if the last used bitcoin was compeletely used
        // delete the compeletely used bitcoin from sender wallet
        deleteBitcoinListNodeFromBitcoinList(senderWallet->bitcoinList, senderWallet->bitcoinList->firstNode->bitcoinTree->bitcoinId);
    }

    // update sender and receiver wallets' balances
    senderWallet->balance -= transaction->amount;
    receiverWallet->balance += transaction->amount;

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
