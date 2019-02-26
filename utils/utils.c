#include "utils.h"

WalletList* initWalletList() {
    WalletList* walletList = (WalletList*)malloc(sizeof(WalletList));
    walletList->size = 0;
    walletList->firstWallet = NULL;

    return walletList;
}

// Wallet* initWallet(char* name) {
//     Wallet* wallet = (Wallet*)malloc(sizeof(Wallet));
//     wallet->name = (char*)malloc(PATH_MAX);
//     strcpy(wallet->name, name);
//     wallet->nextNode = NULL;
//     wallet->prevNode = NULL;

//     return wallet;
// }

// void freeWallet(Wallet** wallet) {
//     if ((*wallet) == NULL)
//         return;

//     free((*wallet)->name);
//     (*wallet)->name = NULL;
//     free(*wallet);
//     (*wallet) = NULL;
//     return;
// }

// void freeWalletList(WalletList** walletList) {
//     Wallet* curWallet = (*walletList)->firstWallet;

//     while (curWallet != NULL) {
//         free(curWallet->name);
//         curWallet->name = NULL;

//         if (curWallet->nextNode != NULL) {
//             curWallet = curWallet->nextNode;
//             free(curWallet->prevNode);

//             curWallet->prevNode = NULL;
//         } else {
//             free(curWallet);
//             curWallet = NULL;
//         }
//     }
//     free(*walletList);
//     (*walletList) = NULL;
// }

void freeWalletRec(Wallet** wallet) {
    if ((*wallet) == NULL)
        return;

    freeWalletRec(&((*wallet)->nextWallet));

    // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
    freeWallet((*wallet));

    free((*wallet)->walletId);
    (*wallet)->walletId = NULL;
    free(*wallet);
    (*wallet) = NULL;
    return;
}

void freeWalletList(WalletList** walletList) {
    if ((*walletList) == NULL)
        return;

    freeWalletRec(&(*walletList)->firstWallet);
    (*walletList)->firstWallet = NULL;

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
        } else if (strcmp(curWallet->walletId, walletId) < 0) {  // no need for searching further since the list is sorted
            return NULL;
        }
        curWallet = curWallet->nextWallet;
    }
    return NULL;
}

Wallet* addWalletToWalletList(WalletList* walletList, char* walletId, unsigned int balance) {
    if (walletList->size == 0) {
        walletList->firstWallet = initWallet(balance);

        walletList->size++;
        printf("Inserted |%d| to WalletList\n\n", bitcoinId);
        return walletList->firstWallet;
    } else {
        Wallet* curWallet = walletList->firstWallet;

        if (strcmp(curWallet->walletId, bitcoinId) < 0) {
            // insert at start
            Wallet* walletToInsert = initWallet(startAmount);
            walletToInsert->nextWallet = curWallet;

            // curWallet->prevNode = walletToInsert;
            walletList->firstWallet = walletToInsert;
            walletList->size++;
            printf("Inserted |%d| to WalletList\n\n", bitcoinId);
            return walletList->firstWallet;
        }
        while (curWallet != NULL) {
            if (curWallet->nextWallet != NULL) {
                if (strcmp(curWallet->walletId, bitcoinId) < 0) {
                    Wallet* walletToInsert = initWallet(startAmount);
                    // walletToInsert->prevNode = curWallet;
                    walletToInsert->nextWallet = curWallet->nextWallet;

                    // curWallet->nextNode->prevNode = walletToInsert;
                    curWallet->nextWallet = walletToInsert;
                    walletList->size++;
                    printf("Inserted |%d| to WalletList\n\n", bitcoinId);
                    return curWallet->nextWallet;
                }
            } else {
                // insert at the end
                curWallet->nextWallet = initWallet(startAmount);
                // curWallet->nextNode->prevNode = curWallet;

                walletList->size++;
                printf("Inserted |%d| to WalletList\n\n", bitcoinId);
                return curWallet->nextWallet;
            }

            curWallet = curWallet->nextWallet;
        }
    }

    return NULL;  // not normal behavior
}

// int deleteWalletFromWalletList(WalletList* walletList, char* name) {
//     if (walletList == NULL)
//         return -1;
//     Wallet* walletToDelete = findWalletInWalletList(walletList, name);

//     if (walletToDelete == NULL) {
//         printf("Name node with name %s not found in names' list\n", name);
//         return -1;
//     }

//     if (strcmp(walletToDelete->name, walletList->firstWallet->name) == 0) {
//         walletList->firstWallet = walletToDelete->nextNode;
//     }
//     if (walletToDelete->prevNode != NULL) {
//         walletToDelete->prevNode->nextNode = walletToDelete->nextNode;
//     }
//     if (walletToDelete->nextNode != NULL) {
//         walletToDelete->nextNode->prevNode = walletToDelete->prevNode;
//     }

//     freeWallet(&walletToDelete);
//     walletList->size--;

//     return 0;
// }
