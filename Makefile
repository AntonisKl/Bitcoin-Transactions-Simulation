CC = gcc
CFLAGS  = -g -Wall
RM = rm -rf

default: bitcoin

bitcoin:  bitcoin.o tree.o hashtable.o utils.o
	$(CC) $(CFLAGS) -o executables/bitcoin bitcoin.o tree.o hashtable.o utils.o

bitcoin.o:  bitcoin.c
	$(CC) $(CFLAGS) -c bitcoin.c

tree.o:  tree/tree.c
	$(CC) $(CFLAGS) -c tree/tree.c

hashtable.o:  hashtable/hashtable.c
	$(CC) $(CFLAGS) -c hashtable/hashtable.c

utils.o:  utils/utils.c
	$(CC) $(CFLAGS) -c utils/utils.c

clean: 
	$(RM) bitcoin *.o executables/*