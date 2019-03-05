#include "utils.h"

// #include "../bitcoin_tree_list/bitcoin_tree_list.h"

// WalletList* initWalletList() {
//     WalletList* walletList = (WalletList*)malloc(sizeof(WalletList));
//     walletList->size = 0;
//     walletList->firstWallet = NULL;

//     return walletList;
// }

// Wallet* initWallet(char* walletId, unsigned int balance, BitcoinList* bitcoinList) {
//     Wallet* wallet = (Wallet*)malloc(sizeof(Wallet));

//     wallet->walletId = (char*)malloc(MAX_WALLET_ID_SIZE);
//     wallet->bitcoinList = bitcoinList;
//     wallet->nextWallet = NULL;

//     return wallet;
// }

// void freeWallet(Wallet** wallet) {
//     free((*wallet)->walletId);
//     (*wallet)->walletId = NULL;

//     freeBitcoinList(&(*wallet)->bitcoinList);
//     (*wallet)->nextWallet = NULL;
// }

// // Wallet* initWallet(char* name) {
// //     Wallet* wallet = (Wallet*)malloc(sizeof(Wallet));
// //     wallet->name = (char*)malloc(PATH_MAX);
// //     strcpy(wallet->name, name);
// //     wallet->nextNode = NULL;
// //     wallet->prevNode = NULL;

// //     return wallet;
// // }

// // void freeWallet(Wallet** wallet) {
// //     if ((*wallet) == NULL)
// //         return;

// //     free((*wallet)->name);
// //     (*wallet)->name = NULL;
// //     free(*wallet);
// //     (*wallet) = NULL;
// //     return;
// // }

// // void freeWalletList(WalletList** walletList) {
// //     Wallet* curWallet = (*walletList)->firstWallet;

// //     while (curWallet != NULL) {
// //         free(curWallet->name);
// //         curWallet->name = NULL;

// //         if (curWallet->nextNode != NULL) {
// //             curWallet = curWallet->nextNode;
// //             free(curWallet->prevNode);

// //             curWallet->prevNode = NULL;
// //         } else {
// //             free(curWallet);
// //             curWallet = NULL;
// //         }
// //     }
// //     free(*walletList);
// //     (*walletList) = NULL;
// // }

// void freeWalletRec(Wallet** wallet) {
//     if ((*wallet) == NULL)
//         return;

//     freeWalletRec(&((*wallet)->nextWallet));

//     // freeTransactionArray(&(*bucket)->transactions, hashTable->bucketSize);
//     freeWallet(&(*wallet));

//     free((*wallet)->walletId);
//     (*wallet)->walletId = NULL;
//     free(*wallet);
//     (*wallet) = NULL;
//     return;
// }

// void freeWalletList(WalletList** walletList) {
//     if ((*walletList) == NULL)
//         return;

//     freeWalletRec(&(*walletList)->firstWallet);
//     (*walletList)->firstWallet = NULL;

//     free(*walletList);
//     (*walletList) = NULL;

//     return;
// }

// Wallet* findWalletInWalletList(WalletList* walletList, char* walletId) {
//     if (walletList == NULL)
//         return NULL;

//     Wallet* curWallet = walletList->firstWallet;

//     while (curWallet != NULL) {
//         if (strcmp(curWallet->walletId, walletId) == 0) {
//             return curWallet;
//         } else if (strcmp(walletId, curWallet->walletId) < 0) {  // no need for searching further since the list is sorted
//             return NULL;
//         }
//         curWallet = curWallet->nextWallet;
//     }
//     return NULL;
// }

// Wallet* addWalletToWalletList(WalletList* walletList, char* walletId, unsigned int balance, BitcoinList* bitcoinList) {
//     if (walletList->size == 0) {
//         walletList->firstWallet = initWallet(balance);

//         walletList->size++;
//         printf("Inserted |%s| to WalletList\n\n", walletId);
//         return walletList->firstWallet;
//     } else {
//         Wallet* curWallet = walletList->firstWallet;

//         if (strcmp(walletId, curWallet->walletId) < 0) {
//             // insert at start
//             Wallet* walletToInsert = initWallet(balance);
//             walletToInsert->nextWallet = curWallet;

//             // curWallet->prevNode = walletToInsert;
//             walletList->firstWallet = walletToInsert;
//             walletList->size++;
//             printf("Inserted |%s| to WalletList\n\n", walletId);
//             return walletList->firstWallet;
//         }
//         while (curWallet != NULL) {
//             if (curWallet->nextWallet != NULL) {
//                 if (strcmp(walletId, curWallet->nextWallet->walletId) < 0) {
//                     Wallet* walletToInsert = initWallet(balance);
//                     // walletToInsert->prevNode = curWallet;
//                     walletToInsert->nextWallet = curWallet->nextWallet;

//                     // curWallet->nextNode->prevNode = walletToInsert;
//                     curWallet->nextWallet = walletToInsert;
//                     walletList->size++;
//                     printf("Inserted |%s| to WalletList\n\n", walletId);
//                     return curWallet->nextWallet;
//                 }
//             } else {
//                 // insert at the end
//                 curWallet->nextWallet = initWallet(balance);
//                 // curWallet->nextNode->prevNode = curWallet;

//                 walletList->size++;
//                 printf("Inserted |%s| to WalletList\n\n", walletId);
//                 return curWallet->nextWallet;
//             }

//             curWallet = curWallet->nextWallet;
//         }
//     }

//     return NULL;  // not normal behavior
// }

// // int deleteWalletFromWalletList(WalletList* walletList, char* name) {
// //     if (walletList == NULL)
// //         return -1;
// //     Wallet* walletToDelete = findWalletInWalletList(walletList, name);

// //     if (walletToDelete == NULL) {
// //         printf("Name node with name %s not found in names' list\n", name);
// //         return -1;
// //     }

// //     if (strcmp(walletToDelete->name, walletList->firstWallet->name) == 0) {
// //         walletList->firstWallet = walletToDelete->nextNode;
// //     }
// //     if (walletToDelete->prevNode != NULL) {
// //         walletToDelete->prevNode->nextNode = walletToDelete->nextNode;
// //     }
// //     if (walletToDelete->nextNode != NULL) {
// //         walletToDelete->nextNode->prevNode = walletToDelete->prevNode;
// //     }

// //     freeWallet(&walletToDelete);
// //     walletList->size--;

// //     return 0;
// // }

void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes) {
    if (argc != 13) {
        printf("Invalid arguments. Exiting...\n");
        exit(1);
    }

    if (strcmp(argv[1], "-a") == 0) {
        (*bitcoinBalancesFileName) = argv[2];
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[3], "-t") == 0) {
        (*transactionsFileName) = argv[4];
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[5], "-v") == 0) {
        (*bitcoinValue) = atoi(argv[6]);
        if ((*bitcoinValue) <= 0) {
            printf("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[7], "-h1") == 0) {
        (*senderHashTableSize) = atoi(argv[8]);
        if ((*senderHashTableSize) <= 0) {
            printf("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[9], "-h2") == 0) {
        (*receiverHashTableSize) = atoi(argv[10]);
        if ((*receiverHashTableSize) <= 0) {
            printf("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[11], "-b") == 0) {
        (*bucketSizeBytes) = atoi(argv[12]);
        if ((*bucketSizeBytes) <= 0) {
            printf("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printf("Invalid arguments\nExiting...\n");
        exit(1);
    }

    return;
}

void handleBitcoinBalancesFile(char* fileName, WalletList** walletList, BitcoinList** bitcoinList, int bitcoinValue) {
    (*walletList) = initWalletList();
    (*bitcoinList) = initBitcoinList();

    char line[MAX_FILE_LINE_SIZE], walletId[MAX_WALLET_ID_SIZE], *token;
    BitcoinList* curWalletBitcoinList;
    FILE* fileP;
    int bitcoinId;

    if ((fileP = fopen(fileName, "r")) == NULL) {
        perror("fopen error");
        exit(1);
    }

    while (fgets(line, MAX_FILE_LINE_SIZE, fileP) != NULL) {
        token = strtok(line, " ");
        if (token == NULL) {
            printf("Invalid bitcoin balances file");
            exit(1);
        }
        strcpy(walletId, token);

        curWalletBitcoinList = initBitcoinList();  // caution!!!!!!!!!!! it is a pointer!!!!!!!!
        
        while ((token = strtok(NULL, " ")) != NULL) {
            char* endptr;
            errno = 0;
            bitcoinId = strtol(token, &endptr, 10);
            if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                printf("Invalid bitcoin balances file\n");
                exit(1);
            }

            // addBitcoinListNodeToBitcoinList(curWalletBitcoinList, bitcoinId, bitcoinValue); // add to wallet's list
            addBitcoinListNodeToBitcoinListByPointer(curWalletBitcoinList, addBitcoinListNodeToBitcoinList((*bitcoinList), walletId, bitcoinId, bitcoinValue));
        }

        addWalletToWalletList((*walletList), walletId, bitcoinValue * curWalletBitcoinList->size, curWalletBitcoinList);  // watch out with bitcoin list!!!!!!!!!
    }

    fclose(fileP);

    return;
}

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, int withTransactionId) {
    int bitcoinAmount;
    char *endptr, *token, transactionId[MAX_TRANSACTION_ID_SIZE], senderWalletId[MAX_WALLET_ID_SIZE], receiverWalletId[MAX_WALLET_ID_SIZE], dateTimeS[MAX_DATETIME_SIZE];

    if (withTransactionId == 1) {
        token = strtok(transactionS, " ");
        if (token == NULL) {
            printf("Invalid transactions string");
            exit(1);
        }

        // errno = 0;
        // transactionId = strtol(token, &endptr, 10);
        // if ((endptr == token) || ((transactionId == LONG_MAX || transactionId == LONG_MIN) && errno == ERANGE)) {
        //     printf("Invalid transactions file");
        //     exit(1);
        // }
        strcpy(transactionId, token);
    } else {
        int id = rand();
        char idS[MAX_STRING_INT_SIZE];
        sprintf(idS, "%d", id);
        while (findTransactionInHashTable(senderHashTable, idS) != NULL) {
            id = rand();
            sprintf(idS, "%d", id);
        }

        strcpy(transactionId, idS);
    }

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Invalid transactions string");
        exit(1);
    }
    strcpy(senderWalletId, token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Invalid transactions string");
        exit(1);
    }
    strcpy(receiverWalletId, token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Invalid transactions string");
        exit(1);
    }
    errno = 0;
    bitcoinAmount = strtol(token, &endptr, 10);
    if ((endptr == token) || ((bitcoinAmount == LONG_MAX || bitcoinAmount == LONG_MIN) && errno == ERANGE)) {
        perror("Invalid transactions string");
        exit(1);
    }

    token = strtok(NULL, "\n");
    if (token == NULL) {
        printf("Invalid transactions string\n");
        exit(1);
    }
    strcpy(dateTimeS, token);

    Wallet* foundSenderWallet = findWalletInWalletList(walletList, senderWalletId);
    if (foundSenderWallet == NULL) {
        printf("sender wallet id does not exist\n");
        // exit(1);
        return;
    }
    Wallet* foundReceiverWallet = findWalletInWalletList(walletList, senderWalletId);
    if (foundReceiverWallet == NULL) {
        printf("receiver wallet id does not exist\n");
        // exit(1);
        return;
    }
    printf("hello\n");

    // Transaction* foundTransaction = find ////// HERE CHECK IF TRANSACTION ALREADY EXISTSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
    if (findTransactionInHashTable(senderHashTable, transactionId) != NULL) {
        printf("Invalid transaction id\n");
        return;
    }
    printf("hello1\n");

    BitcoinList* transactionBitcoinList = initBitcoinList();  // NOT USED!!!!!!!!!!!!!!!! (YET)
    Transaction* createdTransaction = initTransaction(transactionId, senderWalletId, receiverWalletId, dateTimeS, transactionBitcoinList);
    printf("hello1.5\n");

    if (handleWalletToWalletTransfer(foundSenderWallet, foundReceiverWallet, createdTransaction) == -1) {
        printf("Transaction not completed\n");
        return;
    }
    printf("hello2\n");

    insertTransactionToHashTable(senderHashTable, createdTransaction, SENDER);
    insertTransactionToHashTable(receiverHashTable, createdTransaction, RECEIVER);
}

void handleTransactionsFile(char* fileName, HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            int senderHashTableSize, int receiverHashTableSize, int bucketSize) {
    (*senderHashTable) = initHashTable(senderHashTableSize, bucketSize);
    (*receiverHashTable) = initHashTable(receiverHashTableSize, bucketSize);

    char line[MAX_FILE_LINE_SIZE] /*, senderWalletId[MAX_WALLET_ID_SIZE], receiverWalletId[MAX_WALLET_ID_SIZE], dateTimeS[MAX_DATETIME_SIZE], *token*/;
    FILE* fileP;

    if ((fileP = fopen(fileName, "r")) == NULL) {
        perror("fopen error");
        exit(1);
    }

    while (fgets(line, MAX_FILE_LINE_SIZE, fileP) != NULL) {
        handleTransactionString(line, walletList, (*senderHashTable), (*receiverHashTable), 1);
        // char* endptr;

        // token = strtok(line, " ");
        // if (token == NULL) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }

        // errno = 0;
        // transactionId = strtol(token, &endptr, 10);
        // if ((endptr == token) || ((transactionId == LONG_MAX || transactionId == LONG_MIN) && errno == ERANGE)) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }

        // token = strtok(NULL, " ");
        // if (token == NULL) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }
        // strcpy(senderWalletId, token);

        // token = strtok(NULL, " ");
        // if (token == NULL) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }
        // strcpy(receiverWalletId, token);

        // token = strtok(NULL, " ");
        // if (token == NULL) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }
        // errno = 0;
        // bitcoinAmount = strtol(token, &endptr, 10);
        // if ((endptr == token) || ((bitcoinAmount == LONG_MAX || bitcoinAmount == LONG_MIN) && errno == ERANGE)) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }

        // token = strtok(NULL, "\n");
        // if (token == NULL) {
        //     perror("Invalid transactions file");
        //     exit(1);
        // }
        // strcpy(dateTimeS, token);

        // Wallet* foundSenderWallet = findWalletInWalletList(walletList, senderWalletId);
        // if (foundSenderWallet == NULL) {
        //     perror("sender wallet id does not exist");
        //     // exit(1);
        //     continue;
        // }
        // Wallet* foundReceiverWallet = findWalletInWalletList(walletList, senderWalletId);
        // if (foundReceiverWallet == NULL) {
        //     perror("receiver wallet id does not exist");
        //     // exit(1);
        //     continue;
        // }
        // BitcoinList* transactionBitcoinList = initBitcoinList(); // NOT USED!!!!!!!!!!!!!!!! (YET)
        // Transaction* createdTransaction = initTransaction(transactionId, senderWalletId, receiverWalletId, dateTimeS, transactionBitcoinList);

        // if (handleWalletToWalletTransfer(foundSenderWallet, foundReceiverWallet, createdTransaction) == -1) {
        //     perror("Transaction not completed");
        //     continue;
        // }

        // insertTransactionToHashTable((*senderHashTable), createdTransaction, SENDER);
        // insertTransactionToHashTable((*receiverHashTable), createdTransaction, RECEIVER);
    }

    fclose(fileP);
}

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList) {
    char inputS[MAX_INPUT_SIZE], *token;

    fgets(inputS, MAX_INPUT_SIZE, stdin);
    token = strtok(inputS, " ");
    while (token == NULL) {
        printf("Invalid input, try again\n");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        token = strtok(inputS, " ");
    }

    while (strcmp(token, "./exit") != 0) {
        if (strcmp(token, "./requestTransaction")) {
            handleTransactionString(strtok(NULL, "\n"), walletList, senderHashTable, receiverHashTable, 0);
        } else if (strcmp(token, "./requestTransactions")) {
            token = strtok(NULL, ";");
            while (token != NULL) {
                handleTransactionString(token, walletList, senderHashTable, receiverHashTable, 0);
            }
        } else if (strcmp(token, "./findEarnings")) {
        } else if (strcmp(token, "./findPayments")) {
        } else if (strcmp(token, "./walletStatus")) {
        } else if (strcmp(token, "./bitCoinStatus")) {
        } else if (strcmp(token, "./traceCoin")) {
        }

        fgets(inputS, MAX_INPUT_SIZE, stdin);
        token = strtok(inputS, " ");
        while (token == NULL) {
            printf("Invalid input, try again\n");
            fgets(inputS, MAX_INPUT_SIZE, stdin);
            token = strtok(inputS, " ");
        }
    }

    return;
}

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList) {
    printf("in free memory\n");
    freeHashTable(senderHashTable);
    freeHashTable(receiverHashTable);
    freeBitcoinList(bitcoinList);
    freeWalletList(walletList);

    return;
}

time_t datetimeStringToTimeStamp(char* datetimeS) {
    struct tm timeStruct;
    time_t timestamp;
    if (strptime(datetimeS, "%d-%m-%Y %H:%M", &timeStruct) != NULL) {
        timestamp = mktime(&timeStruct);
    } else {
        perror("time convert error");
        return -1;
    }

    return timestamp;
}