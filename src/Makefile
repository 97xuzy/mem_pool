
mp_test.out: mem_pool.c mem_pool.h mem_pool_test.c
	gcc -std=c11 -Wall -Werror -g mem_pool.c mem_pool_test.c -o mp_test.out

.PHONY: clean

clean:
	rm -f mem_pool.o
	rm -f mem_pool_test.o
	rm -f mp_test.out

.PHONY: test

test: mp_test.out
	valgrind --leak-check=full ./mp_test.out
