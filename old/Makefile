all : test
.PHONY : all

test : tool.o test.o
	gcc -g -lcrypto test/test.o tool.o -o test/test

tool.o : tool.c tool.h
	gcc -c -g -I/usr/local/include -lcrypto tool.c -o tool.o

test.o : test/binarytree_test.c tool.h
	gcc -c -g -I/home/guangtong/sync_file/ test/binarytree_test.c -o test/test.o

clean : 
	-rm *.o test/test
