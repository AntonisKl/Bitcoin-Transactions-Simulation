CC = gcc
CFLAGS  = -g -Wall
RM = rm -rf

default: bitcoin

bitcoin:  bitcoin.o bitcoin_tree_list.o hashtable.o wallet_list.o utils.o
	$(CC) $(CFLAGS) -o exe/bitcoin bitcoin.o bitcoin_tree_list.o hashtable.o wallet_list.o utils.o

bitcoin.o:  bitcoin.c
	$(CC) $(CFLAGS) -c bitcoin.c

bitcoin_tree_list.o:  bitcoin_tree_list/bitcoin_tree_list.c
	$(CC) $(CFLAGS) -c bitcoin_tree_list/bitcoin_tree_list.c

hashtable.o:  hashtable/hashtable.c
	$(CC) $(CFLAGS) -c hashtable/hashtable.c

wallet_list.o:  wallet_list/wallet_list.c
	$(CC) $(CFLAGS) -c wallet_list/wallet_list.c

utils.o:  utils/utils.c
	$(CC) $(CFLAGS) -c utils/utils.c

clean: 
	$(RM) bitcoin *.o exe/*