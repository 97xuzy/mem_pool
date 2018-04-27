
CFLAGS=-std=c11 -Wall -Werror
TEST_CFLAGS=-g3 $(CFLAGS)

LIB_obj=mem_pool.o



.PHONY: static

static: mem_pool.a

mem_pool.a: $(LIB_obj)
	ar rcs $@ $^

mem_pool.o: mem_pool.c mem_pool.h
	gcc $(CFLAGS) -c $< -o $@



.PHONY: shared

shared: mem_pool.so


libmem_pool.so: mem_pool_so.o
	gcc -shared -fPIC -o libqux.so $^

mem_pool_so.o: mem_pool.c mem_pool.h
	gcc $(CFLAGS) -fPIC -c $< -o $@ 



.PHONY: clean

clean:
	rm -f mem_pool.a
	rm -f mem_pool.o
	rm -f mem_pool_test.o
	rm -f mp_test.out



.PHONY: test

test: mp_test.out
	valgrind --leak-check=full ./$<

mp_test.out: mem_pool.o mem_pool_test.o
	gcc $(TEST_CFLAGS) $^ -o $@

mem_pool_test.o: mem_pool_test.c
	gcc $(TEST_CFLAGS) -c $< -o $@



.PHONY: help
	echo -e "make\nmake static\nmake shared\nmake clean\nmake test\n"
