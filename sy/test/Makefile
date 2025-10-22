.PHONY: test debug

CFLAGS=-I ../src -Wall -g #-DSENTINEL

test: test-rbtree
	./test-rbtree
	# valgrind ./test-rbtree

debug: test-rbtree
	gdbserver :1234 ./test-rbtree


test-rbtree: test-rbtree.o ../src/rbtree.o ../src/utils.o

../src/rbtree.o: ../src/rbtree.c
	$(MAKE) -C ../src rbtree.o

../src/utils.o: ../src/utils.c
	$(MAKE) -C ../src utils.o

clean:
	rm -f test-rbtree *.o
