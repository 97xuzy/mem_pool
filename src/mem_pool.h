#ifndef _MEM_POOL_H
#define _MEM_POOL_H

#include <stdlib.h>

#define DEFAULT_INITIAL_POOL_SIZE 64

#ifdef MEM_POOL_ERROR_PRINT
#define E_MALLOC_FAIL 1
#define E_CALLOC_FAIL 2
#define E_REALLOC_FAIL 4
#define E_NONEXIST 8
#define E_POOL_MODIFIED 16
#endif

typedef struct mem_pool_stru
{
    size_t num;
    size_t array_size;
    void **array;
} mem_pool;

mem_pool* create_mem_pool();
mem_pool* create_mem_pool_w_size(size_t inital_pool_size);

void *pool_malloc(size_t size, mem_pool *pool);
void *pool_calloc(size_t nmemb, size_t size, mem_pool *pool);
void *pool_realloc(void *ptr, size_t size, mem_pool *pool);


/**
 * Add block that is not allocated by the pool to this pool
 */
int add_block_to_pool(void *block, mem_pool *pool);

/**
 * In case the block in the pool have to be updated outside the pool
 * e.g. realloc outside the pool
 */
int update_block_in_pool(void *block, void *new_block, mem_pool *pool);

int pool_free(void *mem_space, mem_pool *pool);
#define pool_free free_one_in_pool
int free_whole_pool(mem_pool *pool);

// Move all the memory in src pool to dest pool
int merge_pool(mem_pool *dest, mem_pool *src);

// Move 1 block of memory in src pool to dest pool
int move_one_between_pool(mem_pool *dest, mem_pool *src, void *mem_space);

// Free the whole pool, and destory the pool itself
int destroy_pool(mem_pool *pool);









#endif
