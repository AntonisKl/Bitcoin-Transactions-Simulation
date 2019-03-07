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

// Function to remove all spaces from a given string
void removeSpaces(char* str) {
    // To keep track of non-space character count
    if (str == NULL)
        return;

    int count = 0;
    int stringSize = strlen(str);

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; i < stringSize; i++) {
        if (str[i] != ' ')
            str[count++] = str[i];  // here count is
                                    // incremented
    }
    str[count] = '\0';
    return;
}

void timeStringToHourMinute(char* timeS, int* hour, int* minute) {  // considers time string valid
    char tempTimeS[MAX_TIME_SIZE], *token, *endptr;

    strcpy(tempTimeS, timeS);

    token = strtok(tempTimeS, ":");
    (*hour) = strtol(token, &endptr, 10);

    token = strtok(NULL, " ");
    (*minute) = strtol(token, &endptr, 10);

    return;
}

char validateTimeS(char* timeS) {
    if (strlen(timeS) > MAX_TIME_SIZE)
        return -1;

    char tempTimeS[MAX_TIME_SIZE], *token, *endptr;
    int hour, minute;

    strcpy(tempTimeS, timeS);

    token = strtok(tempTimeS, ":");
    if (token == NULL) {
        return -1;
    }
    hour = strtol(token, &endptr, 10);
    if ((endptr == token) || ((hour == LONG_MAX || hour == LONG_MIN) && errno == ERANGE) || hour < 0 || hour > 23) {
        return -1;
    }

    token = strtok(NULL, " ");
    if (token == NULL) {
        return -1;
    }
    minute = strtol(token, &endptr, 10);
    if ((endptr == token) || ((minute == LONG_MAX || minute == LONG_MIN) && errno == ERANGE) || minute < 0 || minute > 59) {
        return -1;
    }

    return 0;
}

char compareTimesS(char* time1, char* time2) {  // considers time string valid
    int hour1, minute1, hour2, minute2;
    timeStringToHourMinute(time1, &hour1, &minute1);
    timeStringToHourMinute(time2, &hour2, &minute2);

    if (hour1 > hour2) {
        return 1;
    } else if (hour1 == hour2) {
        if (minute1 > minute2) {
            return 1;
        } else if (minute1 == minute2) {
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }

    return 2;  // error
}

void dateStringToDayMonthYear(char* timeS, int* day, int* month, int* year) {  // considers time string valid
    char tempDateS[MAX_DATE_SIZE], *token, *endptr;

    strcpy(tempDateS, timeS);

    token = strtok(tempDateS, "-");
    (*day) = strtol(token, &endptr, 10);

    token = strtok(NULL, "-");
    (*month) = strtol(token, &endptr, 10);

    token = strtok(NULL, " ");
    (*year) = strtol(token, &endptr, 10);

    return;
}

char validateDateS(char* dateS) {
    if (strlen(dateS) > MAX_DATE_SIZE)
        return -1;

    char tempDateS[MAX_DATE_SIZE], *token, *endptr;
    int day, month, year;

    printf("date: %s\n", dateS);
    strcpy(tempDateS, dateS);

    token = strtok(tempDateS, "-");
    if (token == NULL) {
        return -1;
    }
    day = strtol(token, &endptr, 10);
    if ((endptr == token) || ((day == LONG_MAX || day == LONG_MIN) && errno == ERANGE) || day < 1 || day > 31) {
        return -1;
    }

    token = strtok(NULL, "-");
    if (token == NULL) {
        return -1;
    }
    month = strtol(token, &endptr, 10);
    if ((endptr == token) || ((month == LONG_MAX || month == LONG_MIN) && errno == ERANGE) || month < 1 || month > 12) {
        return -1;
    }

    token = strtok(NULL, " ");
    if (token == NULL) {
        return -1;
    }
    year = strtol(token, &endptr, 10);
    if ((endptr == token) || ((year == LONG_MAX || year == LONG_MIN) && errno == ERANGE) || year < 0) {
        return -1;
    }

    return 0;
}

char compareDatesS(char* date1, char* date2) {  // considers time string valid
    int day1, month1, year1, day2, month2, year2;
    dateStringToDayMonthYear(date1, &day1, &month1, &year1);
    dateStringToDayMonthYear(date2, &day2, &month2, &year2);

    if (year1 > year2) {
        return 1;
    } else if (year1 == year2) {
        if (month1 > month2) {
            return 1;
        } else if (month1 == month2) {
            if (day1 > day2) {
                return 1;
            } else if (day1 == day2) {
                return 0;
            } else {
                return -1;
            }
        } else {
            return -1;
        }
    } else {
        return -1;
    }

    return 2;  // never happens
}

time_t datetimeStringToTimeStamp(char* datetimeS) {
    struct tm timeStruct;
    time_t timestamp;
    char tempDatetimeS[MAX_DATETIME_SIZE];
    strcpy(tempDatetimeS, datetimeS);

    printf("|%s|\n", tempDatetimeS);
    // if (strptime(datetimeS, "%d-%m-%Y %H:%M", &timeStruct) != NULL) {
    //     timestamp = mktime(&timeStruct);
    // } else {
    //     perror("Time convert error");
    //     return -1;
    // }

    char *token, *endptr;
    int day, month, year, hour, minute;

    token = strtok(tempDatetimeS, "-");
    if (token == NULL) {
        printf("Invalid date\n");
        return -1;
    }
    day = strtol(token, &endptr, 10);
    if ((endptr == token) || ((day == LONG_MAX || day == LONG_MIN) && errno == ERANGE) || day <= 0) {
        printf("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_mday = day;

    token = strtok(NULL, "-");
    if (token == NULL) {
        printf("Invalid date\n");
        return -1;
    }
    month = strtol(token, &endptr, 10);
    if ((endptr == token) || ((month == LONG_MAX || month == LONG_MIN) && errno == ERANGE) || month <= 0) {
        printf("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_mon = month - 1;  // 0 - January

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Invalid date\n");
        return -1;
    }
    year = strtol(token, &endptr, 10);
    if ((endptr == token) || ((year == LONG_MAX || year == LONG_MIN) && errno == ERANGE) || year <= 0) {
        printf("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_year = year - 1900;

    token = strtok(NULL, ":");
    if (token == NULL) {
        printf("Invalid date\n");
        return -1;
    }
    hour = strtol(token, &endptr, 10);
    if ((endptr == token) || ((hour == LONG_MAX || hour == LONG_MIN) && errno == ERANGE) || hour < 0) {
        printf("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_hour = hour;

    token = strtok(NULL, "\n");
    if (token == NULL) {
        printf("Invalid date\n");
        return -1;
    }
    minute = strtol(token, &endptr, 10);
    if ((endptr == token) || ((minute == LONG_MAX || minute == LONG_MIN) && errno == ERANGE) || minute < 0) {
        printf("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_min = minute;

    timeStruct.tm_sec = 0;
    timeStruct.tm_isdst = -1;

    printf("day: %d, month: %d, year: %d, hour: %d, minute: %d\n", timeStruct.tm_mday, timeStruct.tm_mon, timeStruct.tm_year, timeStruct.tm_hour, timeStruct.tm_min);
    timestamp = mktime(&timeStruct);
    printf("CONVERTED TIMESTAMP: %ld\n", timestamp);
    return timestamp;
}

void timestampToDatetimeString(time_t timestamp, char (*datetimeS)[MAX_DATETIME_SIZE]) {
    // char* dateTimeSt;

    // dateTimeSt = ctime(timestamp);

    // struct tm timeStruct;
    // if (strptime(ctime(&timestamp), "%a %b %d %H:%M:%S %Y\n", &timeStruct) != NULL) {
    //     strftime(*datetimeS, MAX_DATETIME_SIZE, "%d-%m-%Y %H:%M", &timeStruct);
    // } else {
    //     perror("Time convert error");
    //     return;
    // }

    struct tm* timeStruct;
    timeStruct = localtime(&timestamp);

    strftime(*datetimeS, MAX_DATETIME_SIZE, "%d-%m-%Y %H:%M", timeStruct);
    printf("cur time: %s\n", *datetimeS);

    return;
}

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
        token = strtok(line, " \n");
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

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, int withTransactionId, time_t* lastTransactionTimestamp) {
    int bitcoinAmount;
    char *endptr, *token, *transactionId, *senderWalletId, *receiverWalletId, dateTimeS[MAX_DATETIME_SIZE];
    char idS[MAX_STRING_INT_SIZE];

    char transactionSTemp[MAX_INPUT_SIZE];
    strcpy(transactionSTemp, transactionS);

    printf("transaction string: %s\n", transactionSTemp);

    if (withTransactionId == 1) {
        transactionId = strtok(transactionSTemp, " ");
        if (transactionId == NULL) {
            printf("Invalid transactions string1\n");
            return;
        }

        // errno = 0;
        // transactionId = strtol(token, &endptr, 10);
        // if ((endptr == token) || ((transactionId == LONG_MAX || transactionId == LONG_MIN) && errno == ERANGE)) {
        //     printf("Invalid transactions file");
        //     exit(1);
        // }
        // strcpy(transactionId, token);
        printf("transaction id: %s\n\n", transactionId);
    } else {
        int id = rand();
        sprintf(idS, "%d", id);
        while (findTransactionInHashTable(senderHashTable, idS) != NULL) {
            id = rand();
            sprintf(idS, "%d", id);
        }

        transactionId = idS;
    }

    if (withTransactionId == 1) {
        senderWalletId = strtok(NULL, " ");
    } else {
        senderWalletId = strtok(transactionSTemp, " ");
    }
    if (senderWalletId == NULL) {
        printf("Invalid transactions string2\n");
        return;
    }
    // strcpy(senderWalletId, token);

    receiverWalletId = strtok(NULL, " ");
    if (receiverWalletId == NULL) {
        printf("Invalid transactions string3\n");
        return;
    }
    // strcpy(receiverWalletId, token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Invalid transactions string4\n");
        return;
    }
    errno = 0;
    bitcoinAmount = strtol(token, &endptr, 10);
    if ((endptr == token) || ((bitcoinAmount == LONG_MAX || bitcoinAmount == LONG_MIN) && errno == ERANGE)) {
        printf("token: %s\n", token);
        printf("Invalid transactions string5\n");
        return;
    }
    printf("transaction id: %s\n\n", transactionId);

    token = strtok(NULL, "\n");
    if (token == NULL || strcmp(token, "") == 0) {
        timestampToDatetimeString(time(NULL), &dateTimeS);
    } else {
        strcpy(dateTimeS, token);
    }
    // strcpy(transactionId, strtok(transactionS, " "));
    printf("transaction id, datetimeS: %s, %s\n\n", transactionId, dateTimeS);
    time_t curTransactionTimestamp = datetimeStringToTimeStamp(dateTimeS);
    printf("CURRENT TIMESTAMP: %ld\n", curTransactionTimestamp);
    if (curTransactionTimestamp < (*lastTransactionTimestamp)) {  // < : because the precision is in minutes
        printf("Transaction date is old\n");
        return;
    } else {
        (*lastTransactionTimestamp) = curTransactionTimestamp;
    }

    printf("sender id: |%s|\n", senderWalletId);

    Wallet* foundSenderWallet = findWalletInWalletList(walletList, senderWalletId);
    if (foundSenderWallet == NULL) {
        printf("sender wallet id does not exist\n");
        // return;
        return;
    }
    Wallet* foundReceiverWallet = findWalletInWalletList(walletList, receiverWalletId);
    if (foundReceiverWallet == NULL) {
        printf("receiver wallet id does not exist\n");
        // return;
        return;
    }
    printf("transaction id: %s\n\n", transactionId);

    // Transaction* foundTransaction = find ////// HERE CHECK IF TRANSACTION ALREADY EXISTSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
    if (findTransactionInHashTable(senderHashTable, transactionId) != NULL) {
        printf("Invalid transaction id: %s\n", transactionId);
        return;
    }
    printf("hello1\n");

    printf("sender wallet balance: %d, amount: %d\n", foundSenderWallet->balance, bitcoinAmount);

    if (foundSenderWallet->balance < bitcoinAmount) {
        printf("Insufficient balance\n");
        return;
    }

    BitcoinList* transactionBitcoinList = initBitcoinList();  // NOT USED!!!!!!!!!!!!!!!! (YET)
    Transaction* createdTransaction = initTransaction(transactionId, bitcoinAmount, senderWalletId, receiverWalletId, dateTimeS, transactionBitcoinList);
    printf("hello1.5\n");

    if (handleWalletToWalletTransfer(foundSenderWallet, foundReceiverWallet, createdTransaction) == -1) {
        printf("Transaction not completed\n");
        freeTransaction(&createdTransaction);
        return;
    }
    printf("balances: %d, %d\n", foundSenderWallet->balance, foundReceiverWallet->balance);
    printf("hello2\n");

    insertTransactionToHashTable(senderHashTable, createdTransaction, SENDER);
    insertTransactionToHashTable(receiverHashTable, createdTransaction, RECEIVER);
    printf("end\n");
}

void handleTransactionsFile(char* fileName, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            time_t* lastTransactionTimestamp) {
    char line[MAX_FILE_LINE_SIZE] /*, senderWalletId[MAX_WALLET_ID_SIZE], receiverWalletId[MAX_WALLET_ID_SIZE], dateTimeS[MAX_DATETIME_SIZE], *token*/;
    FILE* fileP;

    if ((fileP = fopen(fileName, "r")) == NULL) {
        perror("fopen error");
        exit(1);
    }

    while (fgets(line, MAX_FILE_LINE_SIZE, fileP) != NULL) {
        handleTransactionString(line, walletList, senderHashTable, receiverHashTable, 1, lastTransactionTimestamp);
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

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp) {
    char inputS[MAX_INPUT_SIZE], inputSCopy[MAX_INPUT_SIZE], *token, *endptr;

    fgets(inputS, MAX_INPUT_SIZE, stdin);
    // removeSpaces(inputS);
    printf("inputS: %s\n", inputS);
    strcpy(inputSCopy, inputS);

    token = strtok(inputS, " ");
    while (token == NULL) {
        printf("Invalid input, try again\n");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        // removeSpaces(inputS);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
    }

    while (strcmp(token, "./exit\n") != 0) {
        printf("token: %s\n", token);
        if (!strcmp(token, "./requestTransaction")) {
            printf("TRANSACTION LAST TIMESTAMP: %ld\n", *lastTransactionTimestamp);
            handleTransactionString(strtok(NULL, "\n"), walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
        } else if (!strcmp(token, "./requestTransactions")) {
            token = strtok(NULL, ";");
            printf("token1: %s\n", token);
            char* tToken = strtok(token, " ");
            tToken = strtok(NULL, " ");
            if (tToken == NULL) {
                printf("hello1\n");
                token = strtok(inputSCopy, " ");
                token = strtok(NULL, "\n");  // input file name
                handleTransactionsFile(token, senderHashTable, receiverHashTable, bitcoinList, walletList, lastTransactionTimestamp);
            } else {
                printf("hello2\n");
                printf("inpuS: %s\n", inputSCopy);
                char* rest = inputSCopy;
                token = strtok_r(rest, " ", &rest);
                token = strtok_r(rest, ";", &rest);
                // printf("token2: %s\n", token);
                while (token != NULL && strcmp(token, "") && strcmp(token, "\n")) {
                    printf("token2: %s\n", token);
                    handleTransactionString(token, walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
                    // token = strtok(inputS, " ");
                    token = strtok_r(rest, ";", &rest);
                }
            }
        } else if (!strcmp(token, "./findEarnings") || !strcmp(token, "./findPayments")) {
            char findEarnings = strcmp(token, "./findEarnings") == 0 ? 1 : 0;
            char *walletId, *time1S, *time2S, *date1S, *date2S;
            char* rest;
            walletId = strtok(NULL, " ");
            if (walletId == NULL) {
                printf("Invalid input, try again\n");
            } else {
                TransactionList* foundTransactionList = findTransactionListInHashTable(findEarnings == 1 ? receiverHashTable : senderHashTable, walletId);
                printf("hey\n");
                if (foundTransactionList == NULL) {
                    printf("No transactions found for this wallet id\n");
                } else {
                    token = strtok(NULL, " ");
                    if (token == NULL) {  // no time and date
                        // printf("token null1\n");
                        printTransactionsFromTransactionList(foundTransactionList, NULL, NULL, NULL, NULL, findEarnings);
                    } else {
                        token = strtok(NULL, " ");
                        if (token == NULL) {
                            printf("Invalid input, try again\n");
                        } else {
                            token = strtok(NULL, " ");
                            if (token == NULL) {  // only time1 and time2 or only date1 and date2
                                rest = inputSCopy;
                                token = strtok_r(rest, " ", &rest);
                                token = strtok_r(rest, " ", &rest);
                                time1S = strtok_r(rest, " ", &rest);
                                printf("%s\n", time1S);
                                printf("hello\n");
                                if (validateTimeS(time1S) == -1) {
                                    date1S = time1S;
                                    printf("date1: %s\n", date1S);
                                    if (validateDateS(date1S) == -1) {
                                        printf("Invalid input, try again\n");
                                    } else {  // only date1 and date2
                                        date2S = strtok_r(rest, " ", &rest);
                                        if (validateDateS(date2S) == -1) {
                                            printf("Invalid input, try again\n");
                                        } else {
                                            printf("date1: %s, date2: %s\n", date1S, date2S);
                                            if (compareDatesS(date1S, date2S) > 0) {
                                                printf("date1 must be less than date2, try again\n");
                                            } else {
                                                printf("date1: %s, date2: %s\n", date1S, date2S);
                                                printTransactionsFromTransactionList(foundTransactionList, NULL, date1S, NULL, date2S, findEarnings);
                                            }
                                        }
                                    }
                                } else {  // only time1 and time2
                                    time2S = strtok_r(rest, "\n", &rest);
                                    printf("time2S: %s\n", time2S);
                                    if (validateTimeS(time2S) == -1) {
                                        printf("Invalid input, try again\n");
                                    } else {
                                        printf("time1: %s, time2: %s\n", time1S, time2S);
                                        if (compareTimesS(time1S, time2S) > 0) {
                                            printf("time1 must be less than time2, try again\n");
                                        } else {
                                            printTransactionsFromTransactionList(foundTransactionList, time1S, NULL, time2S, NULL, findEarnings);
                                        }
                                    }
                                }
                            } else {
                                token = strtok(NULL, " ");
                                if (token == NULL) {
                                    printf("Invalid input, try again\n");
                                } else {  // time1, time2, date1, date2
                                    printf("all\n");
                                    rest = inputSCopy;
                                    token = strtok_r(rest, " ", &rest);
                                    token = strtok_r(rest, " ", &rest);
                                    time1S = strtok_r(rest, " ", &rest);
                                    date1S = strtok_r(rest, " ", &rest);
                                    time2S = strtok_r(rest, " ", &rest);
                                    date2S = strtok_r(rest, "\n", &rest);

                                    if (validateTimeS(time1S) == -1) {
                                        printf("Invalid input, try again\n");
                                    } else {
                                        if (validateTimeS(time2S) == -1) {
                                            printf("Invalid input, try again\n");
                                        } else {
                                            if (validateDateS(date1S) == -1) {
                                                printf("Invalid input, try again\n");
                                            } else {
                                                if (validateDateS(date2S) == -1) {
                                                    printf("Invalid input, try again\n");
                                                } else {  // all values are valid
                                                    if (compareTimesS(time1S, time2S) > 0) {
                                                        printf("time1 must be less than time2, try again\n");
                                                    } else {
                                                        if (compareDatesS(date1S, date2S) > 0) {
                                                            printf("date1 must be less than date2, try again\n");
                                                        } else {
                                                            printf("time1: %s, date1: %s, time2: %s, date2: %s\n", time1S, date1S, time2S, date2S);
                                                            printTransactionsFromTransactionList(foundTransactionList, time1S, date1S, time2S, date2S, findEarnings);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if (!strcmp(token, "./walletStatus")) {
            char *walletId;
            walletId = strtok(NULL, "\n");
            if (walletId == NULL) {
                printf("Invalid input, try again\n");
            }
            removeSpaces(walletId);
            
            int balance = getCurrentBalanceOfWallet(walletList, walletId);
            if (balance != -1) {
                printf("Balance of wallet with id \"%s\": %d\n", walletId, balance);
            }
        } else if (!strcmp(token, "./bitCoinStatus")) {
            token = strtok(NULL, "\n");
            if (token == NULL) {
                printf("Invalid input, try again\n");
            } else {
                removeSpaces(token);
                int bitcoinId = strtol(token, &endptr, 10);
                if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                    printf("Invalid bitcoin id\n");
                    exit(1);
                }
                int unspentAmount = getUnspentAmountOfBitcoin(bitcoinList, bitcoinId);
                int transactionsNum = getTransactionsNumOfBitcoinById(bitcoinList, bitcoinId);

                printf("%d %d %d\n", bitcoinId, transactionsNum, unspentAmount);
            }
        } else if (!strcmp(token, "./traceCoin")) {
            token = strtok(NULL, "\n");
            if (token == NULL) {
                printf("Invalid input, try again\n");
            } else {
                removeSpaces(token);
                int bitcoinId = strtol(token, &endptr, 10);
                if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                    printf("Invalid bitcoin id\n");
                    exit(1);
                }
                TransactionList* tempTransactionList = initTransactionList(NULL);
                getTransactionsOfBitcoin(bitcoinList, bitcoinId, tempTransactionList);
                if (tempTransactionList->size == 0) {
                    printf("No transactions found\n");
                } else {
                    printTransactionsOfTransactionListSimple(tempTransactionList);
                }
                freeTransactionList(&tempTransactionList, 0);
            }

        } else {
            printf("Invalid input, try again\n");
        }

        fgets(inputS, MAX_INPUT_SIZE, stdin);
        // removeSpaces(inputS);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
        while (token == NULL) {
            printf("Invalid input, try again\n");
            fgets(inputS, MAX_INPUT_SIZE, stdin);
            // removeSpaces(inputS);
            strcpy(inputSCopy, inputS);
            token = strtok(inputS, " ");
        }
    }

    return;
}

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList) {
    printf("in free memory\n");
    freeHashTable(senderHashTable, 1);
    freeHashTable(receiverHashTable, 0);  // double free
    freeBitcoinList(bitcoinList, 1);
    freeWalletList(walletList);

    return;
}