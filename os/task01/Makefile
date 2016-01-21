all: treetest

treetest: bintree.o treetest.c
	gcc -o treetest treetest.c bintree.o

%.o: %.c
	gcc -c $<
