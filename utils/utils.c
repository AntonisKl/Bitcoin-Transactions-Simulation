#include "utils.h"

void printError(char* s) {
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, s);

    return;
}

// Returns true if s is a number else false
char stringIsNumber(char* s) {
    if (s == NULL)
        return 0;

    for (int i = 0; i < strlen(s); i++) {
        char digitIsNumber = 0;
        for (int j = 48; j <= 57; j++) {
            if (s[i] == j)  //Checking for ASCII value of entered character
            {
                digitIsNumber = 1;
                break;
            }
        }
        if (digitIsNumber == 0)
            return 0;
    }

    return 1;
}

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

    // printf("date: %s\n", dateS);
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

    // printf("|%s|\n", tempDatetimeS);
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
        printError("Invalid date\n");
        return -1;
    }
    day = strtol(token, &endptr, 10);
    if ((endptr == token) || ((day == LONG_MAX || day == LONG_MIN) && errno == ERANGE) || day <= 0) {
        printError("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_mday = day;

    token = strtok(NULL, "-");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    month = strtol(token, &endptr, 10);
    if ((endptr == token) || ((month == LONG_MAX || month == LONG_MIN) && errno == ERANGE) || month <= 0) {
        printError("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_mon = month - 1;  // 0 - January

    token = strtok(NULL, " ");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    year = strtol(token, &endptr, 10);
    if ((endptr == token) || ((year == LONG_MAX || year == LONG_MIN) && errno == ERANGE) || year <= 0) {
        printError("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_year = year - 1900;

    token = strtok(NULL, ":");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    hour = strtol(token, &endptr, 10);
    if ((endptr == token) || ((hour == LONG_MAX || hour == LONG_MIN) && errno == ERANGE) || hour < 0) {
        printError("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_hour = hour;

    token = strtok(NULL, "\n");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    minute = strtol(token, &endptr, 10);
    if ((endptr == token) || ((minute == LONG_MAX || minute == LONG_MIN) && errno == ERANGE) || minute < 0) {
        printError("Invalid date\n");
        exit(1);
    }
    timeStruct.tm_min = minute;

    timeStruct.tm_sec = 0;
    timeStruct.tm_isdst = -1;

    // printf("day: %d, month: %d, year: %d, hour: %d, minute: %d\n", timeStruct.tm_mday, timeStruct.tm_mon, timeStruct.tm_year, timeStruct.tm_hour, timeStruct.tm_min);
    timestamp = mktime(&timeStruct);
    // printf("CONVERTED TIMESTAMP: %ld\n", timestamp);
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
    //printf("cur time: %s\n", *datetimeS);

    return;
}

void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes) {
    if (argc != 13) {
        printError("Invalid arguments. Exiting...\n");
        exit(1);
    }

    if (strcmp(argv[1], "-a") == 0) {
        (*bitcoinBalancesFileName) = argv[2];
    } else {
        printError("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[3], "-t") == 0) {
        (*transactionsFileName) = argv[4];
    } else {
        printError("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[5], "-v") == 0) {
        (*bitcoinValue) = atoi(argv[6]);
        if ((*bitcoinValue) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[7], "-h1") == 0) {
        (*senderHashTableSize) = atoi(argv[8]);
        if ((*senderHashTableSize) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[9], "-h2") == 0) {
        (*receiverHashTableSize) = atoi(argv[10]);
        if ((*receiverHashTableSize) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        exit(1);
    }

    if (strcmp(argv[11], "-b") == 0) {
        (*bucketSizeBytes) = atoi(argv[12]);
        if ((*bucketSizeBytes) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            exit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
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
            printError("Invalid bitcoin balances file");
            exit(1);
        }
        strcpy(walletId, token);

        curWalletBitcoinList = initBitcoinList();  // caution!!!!!!!!!!! it is a pointer!!!!!!!!

        while ((token = strtok(NULL, " ")) != NULL) {
            char* endptr;
            errno = 0;
            bitcoinId = strtol(token, &endptr, 10);
            if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                printError("Invalid bitcoin balances file\n");
                exit(1);
            }

            // addBitcoinListNodeToBitcoinList(curWalletBitcoinList, bitcoinId, bitcoinValue); // add to wallet's list
            addBitcoinListNodeToBitcoinListByPointer(curWalletBitcoinList, addBitcoinListNodeToBitcoinList((*bitcoinList), walletId, bitcoinId, bitcoinValue), walletId);
        }

        addWalletToWalletList((*walletList), walletId, bitcoinValue * curWalletBitcoinList->size, curWalletBitcoinList);  // watch out with bitcoin list!!!!!!!!!
    }

    fclose(fileP);

    return;
}

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, char withTransactionId, time_t* lastTransactionTimestamp) {
    int bitcoinAmount;
    char *endptr, *token, *transactionId, *senderWalletId, *receiverWalletId, dateTimeS[MAX_DATETIME_SIZE];
    char idS[MAX_STRING_INT_SIZE];

    char transactionSTemp[MAX_INPUT_SIZE];
    strcpy(transactionSTemp, transactionS);

    //printf("transaction string: %s\n", transactionSTemp);

    if (withTransactionId == 1) {
        transactionId = strtok(transactionSTemp, " ");
        if (transactionId == NULL) {
            printError("Invalid transactions string\n");
            return;
        }

        // errno = 0;
        // transactionId = strtol(token, &endptr, 10);
        // if ((endptr == token) || ((transactionId == LONG_MAX || transactionId == LONG_MIN) && errno == ERANGE)) {
        //     printf("Invalid transactions file");
        //     exit(1);
        // }
        // strcpy(transactionId, token);
        //printf("transaction id: %s\n\n", transactionId);
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
        printError("Invalid transactions string\n");
        return;
    }
    // strcpy(senderWalletId, token);

    receiverWalletId = strtok(NULL, " ");
    if (receiverWalletId == NULL) {
        printError("Invalid transactions string\n");
        return;
    }
    // strcpy(receiverWalletId, token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        printError("Invalid transactions string\n");
        return;
    }
    errno = 0;
    bitcoinAmount = strtol(token, &endptr, 10);
    if (((endptr == token) || ((bitcoinAmount == LONG_MAX || bitcoinAmount == LONG_MIN) && errno == ERANGE)) || bitcoinAmount <= 0) {
        //printf("token: %s\n", token);
        printError("Invalid transactions string\n");
        return;
    }
    //printf("transaction id: %s\n\n", transactionId);

    token = strtok(NULL, "\n");
    if (token == NULL || strcmp(token, "") == 0) {
        timestampToDatetimeString(time(NULL), &dateTimeS);
    } else {
        strcpy(dateTimeS, token);
    }
    // strcpy(transactionId, strtok(transactionS, " "));
    // printf("transaction id, datetimeS: %s, %s\n\n", transactionId, dateTimeS);
    time_t curTransactionTimestamp = datetimeStringToTimeStamp(dateTimeS);
    if (curTransactionTimestamp == -1) {
        return;
    }
    // printf("CURRENT TIMESTAMP: %ld\n", curTransactionTimestamp);
    if (curTransactionTimestamp < (*lastTransactionTimestamp)) {  // < : because the precision is in minutes
        printError("Transaction date is old\n");
        return;
    } else {
        (*lastTransactionTimestamp) = curTransactionTimestamp;
    }

    // printf("sender id: |%s|\n", senderWalletId);

    Wallet* foundSenderWallet = findWalletInWalletList(walletList, senderWalletId);
    if (foundSenderWallet == NULL) {
        printError("Sender wallet id does not exist\n");
        // return;
        return;
    }
    Wallet* foundReceiverWallet = findWalletInWalletList(walletList, receiverWalletId);
    if (foundReceiverWallet == NULL) {
        printError("Receiver wallet id does not exist\n");
        // return;
        return;
    }
    //printf("transaction id: %s\n\n", transactionId);

    // Transaction* foundTransaction = find ////// HERE CHECK IF TRANSACTION ALREADY EXISTSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
    if (findTransactionInHashTable(senderHashTable, transactionId) != NULL) {
        printError("Duplicate transaction id\n");
        return;
    }

    // printf("sender wallet balance: %d, amount: %d\n", foundSenderWallet->balance, bitcoinAmount);

    if (foundSenderWallet->balance < bitcoinAmount) {
        printError("Insufficient balance\n");
        return;
    }

    // BitcoinList* transactionBitcoinList = initBitcoinList();  // NOT USED!!!!!!!!!!!!!!!! (YET)
    Transaction* createdTransaction = initTransaction(transactionId, bitcoinAmount, senderWalletId, receiverWalletId, dateTimeS);
    //printf("hello1.5\n");
    printf(ANSI_COLOR_YELLOW "Transaction started\n" ANSI_COLOR_RESET);
    if (handleWalletToWalletTransfer(foundSenderWallet, foundReceiverWallet, createdTransaction) == -1) {
        printError("Transaction not completed\n");
        freeTransaction(&createdTransaction);
        return;
    }
    // printf("balances: %d, %d\n", foundSenderWallet->balance, foundReceiverWallet->balance);
    //printf("hello2\n");

    insertTransactionToHashTable(senderHashTable, createdTransaction, SENDER);
    insertTransactionToHashTable(receiverHashTable, createdTransaction, RECEIVER);

    printf(ANSI_COLOR_YELLOW "Transaction finished\n" ANSI_COLOR_RESET);
    //printf("end\n");
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
    }

    fclose(fileP);
}

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp) {
    char inputS[MAX_INPUT_SIZE], inputSCopy[MAX_INPUT_SIZE], *token, *endptr;

    printf("\nType a command: ");
    fgets(inputS, MAX_INPUT_SIZE, stdin);
    // removeSpaces(inputS);
    //printf("inputS: %s\n", inputS);
    strcpy(inputSCopy, inputS);

    token = strtok(inputS, " ");
    while (token == NULL) {
        printError("Invalid input, try again\n");
        printf("\nType a command: ");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        // removeSpaces(inputS);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
    }

    while (strcmp(token, "./exit\n") != 0) {
        //printf("token: %s\n", token);
        printf("\n");
        if (!strcmp(token, "./requestTransaction")) {
            //printf("TRANSACTION LAST TIMESTAMP: %ld\n", *lastTransactionTimestamp);
            handleTransactionString(strtok(NULL, "\n"), walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
        } else if (!strcmp(token, "./requestTransactions")) {
            token = strtok(NULL, ";");
            //printf("token1: %s\n", token);
            char* tToken = strtok(token, " ");
            tToken = strtok(NULL, " ");
            if (tToken == NULL) {
                //printf("hello1\n");
                token = strtok(inputSCopy, " ");
                token = strtok(NULL, "\n");  // input file name
                handleTransactionsFile(token, senderHashTable, receiverHashTable, bitcoinList, walletList, lastTransactionTimestamp);
            } else {
                //printf("hello2\n");
                //printf("inpuS: %s\n", inputSCopy);
                char* rest = inputSCopy;
                token = strtok_r(rest, " ", &rest);
                token = strtok_r(rest, ";", &rest);
                // printf("token2: %s\n", token);
                while (token != NULL && strcmp(token, "") && strcmp(token, "\n")) {
                    //printf("token2: %s\n", token);
                    handleTransactionString(token, walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
                    // token = strtok(inputS, " ");
                    token = strtok_r(rest, ";", &rest);
                }
            }
        } else if (!strcmp(token, "./findEarnings") || !strcmp(token, "./findPayments")) {
            char findEarnings = strcmp(token, "./findEarnings") == 0 ? 1 : 0;
            char *walletId, *time1S, *time2S, *date1S, *date2S;
            char* rest;
            walletId = strtok(NULL, " \n");
            if (walletId == NULL) {
                printError("Invalid input, try again\n");
            } else {
                // printf("walletId: %s\n", walletId);
                TransactionList* foundTransactionList = findTransactionListInHashTable(findEarnings == 1 ? receiverHashTable : senderHashTable, walletId);
                //printf("hey\n");
                if (foundTransactionList == NULL) {
                    printf(ANSI_COLOR_YELLOW "No transactions found for this wallet id\n" ANSI_COLOR_RESET);
                } else {
                    token = strtok(NULL, " ");
                    if (token == NULL) {  // no time and date
                        //printf("token null1\n");
                        printTransactionsFromTransactionList(foundTransactionList, NULL, NULL, NULL, NULL, findEarnings);
                    } else {
                        token = strtok(NULL, " ");
                        if (token == NULL) {
                            printError("Invalid input, try again\n");
                        } else {
                            token = strtok(NULL, " ");
                            if (token == NULL) {  // only time1 and time2 or only date1 and date2
                                rest = inputSCopy;
                                token = strtok_r(rest, " ", &rest);
                                token = strtok_r(rest, " ", &rest);
                                time1S = strtok_r(rest, " ", &rest);
                                //printf("%s\n", time1S);
                                //printf("hello\n");
                                if (validateTimeS(time1S) == -1) {
                                    date1S = time1S;
                                    // printf("date1: %s\n", date1S);
                                    if (validateDateS(date1S) == -1) {
                                        printError("Invalid input, try again\n");
                                    } else {  // only date1 and date2
                                        date2S = strtok_r(rest, "\n", &rest);
                                        if (validateDateS(date2S) == -1) {
                                            printError("Invalid input, try again\n");
                                        } else {
                                            // printf("date1: %s, date2: %s\n", date1S, date2S);
                                            if (compareDatesS(date1S, date2S) > 0) {
                                                printError("date1 must be less than date2, try again\n");
                                            } else {
                                                // printf("date1: %s, date2: %s\n", date1S, date2S);
                                                printTransactionsFromTransactionList(foundTransactionList, NULL, date1S, NULL, date2S, findEarnings);
                                            }
                                        }
                                    }
                                } else {  // only time1 and time2
                                    time2S = strtok_r(rest, "\n", &rest);
                                    //printf("time2S: %s\n", time2S);
                                    if (validateTimeS(time2S) == -1) {
                                        printError("Invalid input, try again\n");
                                    } else {
                                        // printf("time1: %s, time2: %s\n", time1S, time2S);
                                        if (compareTimesS(time1S, time2S) > 0) {
                                            printError("time1 must be less than time2, try again\n");
                                        } else {
                                            printTransactionsFromTransactionList(foundTransactionList, time1S, NULL, time2S, NULL, findEarnings);
                                        }
                                    }
                                }
                            } else {
                                token = strtok(NULL, " ");
                                if (token == NULL) {
                                    printError("Invalid input, try again\n");
                                } else {  // time1, time2, date1, date2
                                    rest = inputSCopy;
                                    token = strtok_r(rest, " ", &rest);
                                    token = strtok_r(rest, " ", &rest);
                                    time1S = strtok_r(rest, " ", &rest);
                                    date1S = strtok_r(rest, " ", &rest);
                                    time2S = strtok_r(rest, " ", &rest);
                                    date2S = strtok_r(rest, "\n", &rest);

                                    if (validateTimeS(time1S) == -1) {
                                        printError("Invalid input, try again\n");
                                    } else {
                                        if (validateTimeS(time2S) == -1) {
                                            printError("Invalid input, try again\n");
                                        } else {
                                            if (validateDateS(date1S) == -1) {
                                                printError("Invalid input, try again\n");
                                            } else {
                                                if (validateDateS(date2S) == -1) {
                                                    printError("Invalid input, try again\n");
                                                } else {  // all values are valid
                                                    if (compareTimesS(time1S, time2S) > 0) {
                                                        printError("time1 must be less than time2, try again\n");
                                                    } else {
                                                        if (compareDatesS(date1S, date2S) > 0) {
                                                            printError("date1 must be less than date2, try again\n");
                                                        } else {
                                                            // printf("time1: %s, date1: %s, time2: %s, date2: %s\n", time1S, date1S, time2S, date2S);
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
            char* walletId;
            walletId = strtok(NULL, "\n");
            if (walletId == NULL) {
                printError("Invalid input, try again\n");
            }
            removeSpaces(walletId);

            int balance = getCurrentBalanceOfWallet(walletList, walletId);
            if (balance != -1) {
                printf(ANSI_COLOR_YELLOW "Results:" ANSI_COLOR_RESET "\nBalance of wallet with id %s is %d dollar(s)\n", walletId, balance);
            }
        } else if (!strcmp(token, "./bitCoinStatus")) {
            token = strtok(NULL, "\n");
            if (token == NULL) {
                printError("Invalid input, try again\n");
            } else {
                if (stringIsNumber(token) == 0) {
                    printError("Invalid input, try again\n");
                } else {
                    removeSpaces(token);
                    int bitcoinId = strtol(token, &endptr, 10);
                    if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                        printError("Invalid bitcoin id\n");
                        exit(1);
                    }
                    int unspentAmount = getUnspentAmountOfBitcoin(bitcoinList, bitcoinId);
                    int transactionsNum = getTransactionsNumOfBitcoinById(bitcoinList, bitcoinId);
                    if (unspentAmount == -1 || transactionsNum == -1) {
                        printf(ANSI_COLOR_YELLOW "\nNo results\n" ANSI_COLOR_RESET);
                    } else {
                        printf(ANSI_COLOR_YELLOW "\nResults:\n" ANSI_COLOR_RESET "%d %d %d\n", bitcoinId, transactionsNum, unspentAmount);
                    }
                }
            }
        } else if (!strcmp(token, "./traceCoin")) {
            token = strtok(NULL, "\n");
            if (token == NULL) {
                printError("Invalid input, try again\n");
            } else {
                if (stringIsNumber(token) == 0) {
                    printError("Invalid input, try again\n");
                } else {
                    removeSpaces(token);
                    int bitcoinId = strtol(token, &endptr, 10);
                    if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                        printError("Invalid bitcoin id\n");
                        exit(1);
                    }
                    TransactionList* tempTransactionList = initTransactionList(NULL);
                    getTransactionsOfBitcoin(bitcoinList, bitcoinId, tempTransactionList);
                    if (tempTransactionList->size == 0) {
                        printf(ANSI_COLOR_YELLOW "No transactions found\n" ANSI_COLOR_RESET);
                    } else {
                        printTransactionsOfTransactionListSimple(tempTransactionList);
                    }
                    freeTransactionList(&tempTransactionList, 1);
                }
            }

        } else {
            printError("Invalid input, try again\n");
        }

        printf("\nType a command: ");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        // removeSpaces(inputS);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
        while (token == NULL) {
            printError("Invalid input, try again\n");
            printf("\nType a command: ");
            fgets(inputS, MAX_INPUT_SIZE, stdin);
            // removeSpaces(inputS);
            strcpy(inputSCopy, inputS);
            token = strtok(inputS, " ");
        }
    }

    return;
}

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList) {
    freeHashTable(senderHashTable, 1);
    freeHashTable(receiverHashTable, 0);  // 0 to prevent double free
    freeBitcoinList(bitcoinList, 1, 0);   // 0 to prevent double free
    freeWalletList(walletList);

    printf(ANSI_COLOR_YELLOW "\nMemory freed\n" ANSI_COLOR_RESET);

    return;
}