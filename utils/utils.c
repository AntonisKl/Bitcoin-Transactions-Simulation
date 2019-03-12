#include "utils.h"

void printError(char* s) {
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, s);

    return;
}

void raiseIntAndExit(int num) {
    raise(SIGINT);
    exit(num);
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

void removeSpaces(char* str) {
    if (str == NULL)
        return;

    int count = 0;
    int stringSize = strlen(str);

    for (int i = 0; i < stringSize; i++) {
        if (str[i] != ' ')
            str[count++] = str[i];
    }
    str[count] = '\0';
    return;
}

void freeString(char** s) {
    if ((*s) == NULL)
        return;

    free((*s));
    (*s) = NULL;

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

char compareTimesS(char* time1, char* time2) {  // considers time strings valid
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

void dateStringToDayMonthYear(char* dateS, int* day, int* month, int* year) {  // considers date string valid
    char tempDateS[MAX_DATE_SIZE], *token, *endptr;

    strcpy(tempDateS, dateS);

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

char compareDatesS(char* date1, char* date2) {  // considers date strings valid
    int day1, month1, year1, day2, month2, year2;
    dateStringToDayMonthYear(date1, &day1, &month1, &year1);
    dateStringToDayMonthYear(date2, &day2, &month2, &year2);

    // compare years, months and days in this order
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
        raiseIntAndExit(1);
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
        raiseIntAndExit(1);
    }
    timeStruct.tm_mon = month - 1;  // 0 is January

    token = strtok(NULL, " ");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    year = strtol(token, &endptr, 10);
    if ((endptr == token) || ((year == LONG_MAX || year == LONG_MIN) && errno == ERANGE) || year <= 0) {
        printError("Invalid date\n");
        raiseIntAndExit(1);
    }
    timeStruct.tm_year = year - 1900;  // - 1900: to comply with struct tm's standard

    token = strtok(NULL, ":");
    if (token == NULL) {
        printError("Invalid date\n");
        return -1;
    }
    hour = strtol(token, &endptr, 10);
    if ((endptr == token) || ((hour == LONG_MAX || hour == LONG_MIN) && errno == ERANGE) || hour < 0) {
        printError("Invalid date\n");
        raiseIntAndExit(1);
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
        raiseIntAndExit(1);
    }
    timeStruct.tm_min = minute;

    timeStruct.tm_sec = 0;
    timeStruct.tm_isdst = -1;  // -1: don't give information about Daylight Saving Time

    timestamp = mktime(&timeStruct);  // struct tm to timestamp
    return timestamp;
}

void timestampToDatetimeString(time_t timestamp, char (*datetimeS)[MAX_DATETIME_SIZE]) {
    struct tm* timeStruct;
    timeStruct = localtime(&timestamp);  // convert to local time

    strftime(*datetimeS, MAX_DATETIME_SIZE, "%d-%m-%Y %H:%M", timeStruct);

    return;
}

void handleArgs(int argc, char** argv, char** bitcoinBalancesFileName, char** transactionsFileName, int* bitcoinValue, int* senderHashTableSize,
                int* receiverHashTableSize, int* bucketSizeBytes) {
    // validate argument count
    if (argc != 13) {
        printError("Invalid arguments. Exiting...\n");
        raiseIntAndExit(1);
    }

    // validate input arguments one by one
    if (strcmp(argv[1], "-a") == 0) {
        (*bitcoinBalancesFileName) = argv[2];
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    if (strcmp(argv[3], "-t") == 0) {
        (*transactionsFileName) = argv[4];
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    if (strcmp(argv[5], "-v") == 0) {
        (*bitcoinValue) = atoi(argv[6]);
        if ((*bitcoinValue) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            raiseIntAndExit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    if (strcmp(argv[7], "-h1") == 0) {
        (*senderHashTableSize) = atoi(argv[8]);
        if ((*senderHashTableSize) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            raiseIntAndExit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    if (strcmp(argv[9], "-h2") == 0) {
        (*receiverHashTableSize) = atoi(argv[10]);
        if ((*receiverHashTableSize) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            raiseIntAndExit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    if (strcmp(argv[11], "-b") == 0) {
        (*bucketSizeBytes) = atoi(argv[12]);
        if ((*bucketSizeBytes) <= 0) {
            printError("Invalid arguments\nExiting...\n");
            raiseIntAndExit(1);
        }
    } else {
        printError("Invalid arguments\nExiting...\n");
        raiseIntAndExit(1);
    }

    return;
}

void handleBitcoinBalancesFile(char* fileName, WalletList* walletList, BitcoinList* bitcoinList, int bitcoinValue) {

    char line[MAX_FILE_LINE_SIZE], walletId[MAX_WALLET_ID_SIZE], *token;  // token: pointer to traverse each line with strtok
                                                                          // walletId: the wallet id that is read
    BitcoinList* curWalletBitcoinList;                                    // curWalletBitcoinList: pointer that points to the bitcoin list of the current wallet at any given moment
    FILE* fileP;
    int bitcoinId;  // bitcoinId: the bitcoin id that is read

    if ((fileP = fopen(fileName, "r")) == NULL) {
        perror("fopen error");
        raiseIntAndExit(1);
    }

    // read file line by line, parse bitcoin balance data and save them to the corresponding structs
    while (fgets(line, MAX_FILE_LINE_SIZE, fileP) != NULL) {
        token = strtok(line, " \n");
        if (token == NULL) {
            printError("Invalid bitcoin balances file");
            raiseIntAndExit(1);
        }
        strcpy(walletId, token);  // save wallet id

        // init a bitcoin list for the current wallet
        curWalletBitcoinList = initBitcoinList();

        // read all bitcoin ids and save them as bitcoins if they are valid
        while ((token = strtok(NULL, " ")) != NULL) {
            char* endptr;
            errno = 0;
            bitcoinId = strtol(token, &endptr, 10);  // save bitcoin id
            if ((endptr == token) || ((bitcoinId == LONG_MAX || bitcoinId == LONG_MIN) && errno == ERANGE)) {
                printError("Invalid bitcoin balances file\n");
                freeBitcoinList(&curWalletBitcoinList, 0, 0);
                raiseIntAndExit(1);
            }

            // first create and add a bitcoin list node to the main bitcoin list (addBitcoinListNodeToBitcoinList)
            // and then add the new bitcoin tree's pointer to a new bitcoin list node of the bitcoin list of the current wallet (addBitcoinListNodeToBitcoinListByPointer)
            addBitcoinListNodeToBitcoinListByBitcoinTreePointer(curWalletBitcoinList, addBitcoinListNodeToBitcoinList(bitcoinList, walletId, bitcoinId, bitcoinValue)->bitcoinTree, walletId);
        }

        // create and add wallet to walletList
        addWalletToWalletList(walletList, walletId, bitcoinValue * curWalletBitcoinList->size, curWalletBitcoinList);
    }

    fclose(fileP);

    return;
}

void handleTransactionString(char* transactionS, WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, char withTransactionId, time_t* lastTransactionTimestamp) {
    int bitcoinAmount;
    char *endptr, *token, *transactionId, *senderWalletId, *receiverWalletId, dateTimeS[MAX_DATETIME_SIZE],
        *idS = NULL, transactionSTemp[MAX_INPUT_SIZE];  // endptr: pointer used by strtol
                                                        // token: pointer to traverse each line with strtok
                                                        // transactionId: the transaction id that is read/generated
                                                        // senderWalletId: the sender wallet id that is read
                                                        // receiverWalletId: the receiver wallet id that is read
                                                        // dateTimeS: the date time string that is read
                                                        // idS: the transaction id string that may be generated if transaction id does not exist
                                                        // transactionSTemp: temporary transaction string copy that is used to manipulate the transaction string without modifying the pointer

    strcpy(transactionSTemp, transactionS);

    if (withTransactionId == 1) {  // transaction id is given as input in transactionS
        transactionId = strtok(transactionSTemp, " ");
        if (transactionId == NULL) {
            printError("Invalid transactions string\n");
            return;
        }
    } else {  // transaction id is not given as input in transactionS so we must generate a unique transaction id
        idS = (char*)malloc(MAX_STRING_INT_SIZE);
        int id = rand();
        sprintf(idS, "%d", id);
        while (findTransactionInHashTable(senderHashTable, idS) != NULL) {  // while generated transaction id is not unique, generate a different one
            id = rand();
            sprintf(idS, "%d", id);
        }
        transactionId = idS;  // point to the generated transaction id
    }

    if (withTransactionId == 1) {
        // continue tokenizing
        senderWalletId = strtok(NULL, " ");
    } else {
        // start tokenizing from the beginning
        senderWalletId = strtok(transactionSTemp, " ");
    }
    if (senderWalletId == NULL) {
        printError("Invalid transactions string\n");
        freeString(&idS);
        return;
    }

    receiverWalletId = strtok(NULL, " ");
    if (receiverWalletId == NULL) {
        printError("Invalid transactions string\n");
        freeString(&idS);
        return;
    }

    token = strtok(NULL, " ");
    if (token == NULL) {
        printError("Invalid transactions string\n");
        freeString(&idS);
        return;
    }

    errno = 0;
    bitcoinAmount = strtol(token, &endptr, 10);
    if (((endptr == token) || ((bitcoinAmount == LONG_MAX || bitcoinAmount == LONG_MIN) && errno == ERANGE)) || bitcoinAmount <= 0) {
        printError("Invalid transactions string\n");
        freeString(&idS);
        return;
    }

    token = strtok(NULL, "\n");
    if (token == NULL || strcmp(token, "") == 0) {
        // generate current datetime
        timestampToDatetimeString(time(NULL), &dateTimeS);
    } else {
        strcpy(dateTimeS, token);
    }

    time_t curTransactionTimestamp = datetimeStringToTimeStamp(dateTimeS);
    if (curTransactionTimestamp == -1) {
        printError("Invalid transactions string\n");
        freeString(&idS);
        return;
    }

    if (curTransactionTimestamp < (*lastTransactionTimestamp)) {  // < instead of <= : because the precision is in minutes so we are not strict with the timestamp validity
        printError("Transaction date is old\n");
        freeString(&idS);
        return;
    } else {
        // update last transaction's timestamp
        (*lastTransactionTimestamp) = curTransactionTimestamp;
    }

    // see if there are wallets with the given wallet ids
    Wallet* foundSenderWallet = findWalletInWalletList(walletList, senderWalletId);
    if (foundSenderWallet == NULL) {
        printError("Sender wallet id does not exist\n");
        freeString(&idS);
        return;
    }
    Wallet* foundReceiverWallet = findWalletInWalletList(walletList, receiverWalletId);
    if (foundReceiverWallet == NULL) {
        printError("Receiver wallet id does not exist\n");
        freeString(&idS);
        return;
    }

    if (withTransactionId == 1) {  // only if we are using the given transaction id
        // see if transaction already exists
        if (findTransactionInHashTable(senderHashTable, transactionId) != NULL) {
            printError("Duplicate transaction id\n");
            freeString(&idS);
            return;
        }
    }

    // check if transaction can be made
    if (foundSenderWallet->balance < bitcoinAmount) {
        printError("Insufficient balance\n");
        freeString(&idS);
        return;
    }

    printf(ANSI_COLOR_YELLOW "Transaction started\n" ANSI_COLOR_RESET);
    Transaction* createdTransaction = initTransaction(transactionId, bitcoinAmount, senderWalletId, receiverWalletId, dateTimeS);

    // try to make the transaction by transfering the amount of dollars and the bitcoins from sender wallet to receiver wallet and by updating the used bitcoin trees
    if (handleWalletToWalletTransfer(foundSenderWallet, foundReceiverWallet, createdTransaction) == -1) {
        printError("Transaction not completed\n");
        freeTransaction(&createdTransaction);
        freeString(&idS);
        return;
    }

    // update sender and receiver hash tables
    insertTransactionToHashTable(senderHashTable, createdTransaction, SENDER);
    insertTransactionToHashTable(receiverHashTable, createdTransaction, RECEIVER);

    printf(ANSI_COLOR_YELLOW "Transaction finished\n" ANSI_COLOR_RESET);

    freeString(&idS);
}

void handleTransactionsFile(char* fileName, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, WalletList* walletList,
                            time_t* lastTransactionTimestamp) {
    char line[MAX_FILE_LINE_SIZE];
    FILE* fileP;

    if ((fileP = fopen(fileName, "r")) == NULL) {
        perror("fopen error");
        raise(SIGINT);
        raiseIntAndExit(1);
    }

    // handle each line of transactions' file
    while (fgets(line, MAX_FILE_LINE_SIZE, fileP) != NULL) {
        handleTransactionString(line, walletList, senderHashTable, receiverHashTable, 1, lastTransactionTimestamp);
    }

    fclose(fileP);
}

void handleInput(WalletList* walletList, HashTable* senderHashTable, HashTable* receiverHashTable, BitcoinList* bitcoinList, time_t* lastTransactionTimestamp) {
    char inputS[MAX_INPUT_SIZE], inputSCopy[MAX_INPUT_SIZE], *token, *endptr;  // inputSCopy: copy of input string to manipulate the pure form of the input after modifying inputS string

    printf("\nType a command: ");
    fgets(inputS, MAX_INPUT_SIZE, stdin);
    strcpy(inputSCopy, inputS);

    token = strtok(inputS, " ");
    while (token == NULL) {
        printError("Invalid input, try again\n");
        printf("\nType a command: ");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
    }

    while (strcmp(token, "./exit\n") != 0) {
        printf("\n");
        if (!strcmp(token, "./requestTransaction")) {
            // handle the rest of the input as a single transaction
            handleTransactionString(strtok(NULL, "\n"), walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
        } else if (!strcmp(token, "./requestTransactions")) {
            token = strtok(NULL, ";");
            if (token == NULL) {
                printError("Invalid input, try again\n");
            } else {
                char* tToken = strtok(token, " ");
                if (tToken == NULL) {
                    printError("Invalid input, try again\n");
                } else {
                    tToken = strtok(NULL, " ");
                    if (tToken == NULL) {  // filename is given as input
                        token = strtok(inputSCopy, " ");
                        token = strtok(NULL, "\n");  // input file name
                        handleTransactionsFile(token, senderHashTable, receiverHashTable, bitcoinList, walletList, lastTransactionTimestamp);
                    } else {  // transaction strings are given as input
                        char* rest = inputSCopy;
                        token = strtok_r(rest, " ", &rest);
                        token = strtok_r(rest, ";", &rest);
                        while (token != NULL && strcmp(token, "") && strcmp(token, "\n")) {
                            // handle each transaction string
                            handleTransactionString(token, walletList, senderHashTable, receiverHashTable, 0, lastTransactionTimestamp);
                            token = strtok_r(rest, ";", &rest);
                        }
                    }
                }
            }
        } else if (!strcmp(token, "./findEarnings") || !strcmp(token, "./findPayments")) {
            char findEarnings = strcmp(token, "./findEarnings") == 0 ? 1 : 0;  // findEarnings: flag useful for searching in the appropriate hash table
            char *walletId, *time1S, *time2S, *date1S, *date2S;                // walletId: walletId that is read
                                                                               // timeXS: time string that is read (X = 1 or 2)
                                                                               // dateXS: date string that is read (X = 1 or 2)
            char* rest;                                                        // rest: pointer that is used to traverse the input string with function strtok_r
            walletId = strtok(NULL, " \n");
            if (walletId == NULL) {
                printError("Invalid input, try again\n");
            } else {
                // get all transactions of current wallet id from hash table (receiver or sender)
                TransactionList* foundTransactionList = findTransactionListInHashTable(findEarnings == 1 ? receiverHashTable : senderHashTable, walletId);
                if (foundTransactionList == NULL) {
                    printf(ANSI_COLOR_YELLOW "No transactions found for this wallet id\n" ANSI_COLOR_RESET);
                } else {
                    token = strtok(NULL, " ");
                    if (token == NULL) {  // no time and date is given as input
                        // print all found transactions sorted by timestamp and print sum bitcoin amount
                        printTransactionsFromTransactionList(foundTransactionList, NULL, NULL, NULL, NULL, findEarnings);
                    } else {
                        token = strtok(NULL, " ");
                        if (token == NULL) {
                            printError("Invalid input, try again\n");
                        } else {
                            token = strtok(NULL, " ");
                            if (token == NULL) {  // only time1 and time2 or only date1 and date2 are given as input
                                rest = inputSCopy;
                                token = strtok_r(rest, " ", &rest);
                                token = strtok_r(rest, " ", &rest);
                                time1S = strtok_r(rest, " ", &rest);
                                if (validateTimeS(time1S) == -1) {
                                    date1S = time1S;
                                    if (validateDateS(date1S) == -1) {
                                        printError("Invalid input, try again\n");
                                    } else {  // only date1 and date2 are given as input
                                        date2S = strtok_r(rest, "\n", &rest);
                                        if (validateDateS(date2S) == -1) {
                                            printError("Invalid input, try again\n");
                                        } else {
                                            if (compareDatesS(date1S, date2S) > 0) {
                                                printError("date1 must be less than or equal with date2, try again\n");
                                            } else {
                                                // print transaction with date1S < date < date2S sorted by timestamp and print sum bitcoin amount
                                                printTransactionsFromTransactionList(foundTransactionList, NULL, date1S, NULL, date2S, findEarnings);
                                            }
                                        }
                                    }
                                } else {  // only time1 and time2 are given as input
                                    time2S = strtok_r(rest, "\n", &rest);
                                    if (validateTimeS(time2S) == -1) {
                                        printError("Invalid input, try again\n");
                                    } else {
                                        if (compareTimesS(time1S, time2S) > 0) {
                                            printError("time1 must be less than or equal with time2, try again\n");
                                        } else {
                                            // print transaction with time1S < time < time2S sorted by timestamp and print sum bitcoin amount
                                            printTransactionsFromTransactionList(foundTransactionList, time1S, NULL, time2S, NULL, findEarnings);
                                        }
                                    }
                                }
                            } else {
                                token = strtok(NULL, " ");
                                if (token == NULL) {
                                    printError("Invalid input, try again\n");
                                } else {  // time1, time2, date1, date2 are given as input
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
                                                        printError("time1 must be less than or equal with time2, try again\n");
                                                    } else {
                                                        if (compareDatesS(date1S, date2S) > 0) {
                                                            printError("date1 must be less than or equal with date2, try again\n");
                                                        } else {
                                                            // print transaction with time1S < time < time2S and date1S < date < date2S sorted by timestamp and print sum bitcoin amount
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
                        raiseIntAndExit(1);
                    }
                    int unspentAmount = getUnspentAmountOfBitcoin(bitcoinList, bitcoinId);
                    int transactionsNum = getTransactionsNumOfBitcoin(bitcoinList, bitcoinId);
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
                        raiseIntAndExit(1);
                    }
                    TransactionList* tempTransactionList = initTransactionList(NULL);
                    getTransactionsOfBitcoin(bitcoinList, bitcoinId, tempTransactionList);
                    if (tempTransactionList->size == 0) {
                        printf(ANSI_COLOR_YELLOW "No transactions found\n" ANSI_COLOR_RESET);
                    } else {
                        // print transactions of current bitcoin
                        printTransactionsOfTransactionListSimple(tempTransactionList);
                    }
                    // free transactions list and transactions
                    freeTransactionList(&tempTransactionList, 1);
                }
            }

        } else {
            printError("Invalid input, try again\n");
        }

        printf("\nType a command: ");
        fgets(inputS, MAX_INPUT_SIZE, stdin);
        strcpy(inputSCopy, inputS);
        token = strtok(inputS, " ");
        while (token == NULL) {
            printError("Invalid input, try again\n");
            printf("\nType a command: ");
            fgets(inputS, MAX_INPUT_SIZE, stdin);
            strcpy(inputSCopy, inputS);
            token = strtok(inputS, " ");
        }
    }

    return;
}

void freeMemory(HashTable** senderHashTable, HashTable** receiverHashTable, BitcoinList** bitcoinList, WalletList** walletList) {
    freeHashTable(senderHashTable, 1);    // 1 is given as a parameter to free the transactions as well
    freeHashTable(receiverHashTable, 0);  // 0 is given as a parameter to prevent double free of transactions
    freeBitcoinList(bitcoinList, 1, 0);   // 1 is given as a parameter to free the bitcoin trees as well
                                          // 0 is given as a parameter to prevent double free of transactions
    freeWalletList(walletList);           // frees walletList without double freeing neither the transactions nor the bitcoin trees

    printf(ANSI_COLOR_YELLOW "\nMemory freed\n" ANSI_COLOR_RESET);

    return;
}