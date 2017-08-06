#ifdef MEM_POOL_ERROR_PRINT
#include <stdio.h>
#endif

#include <assert.h>
#include "mem_pool.h"


mem_pool* create_mem_pool()
{
    mem_pool *mp = malloc(sizeof(*mp));
    if(mp == NULL)
    {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_MALLOC_FAIL);
#endif
        return NULL;
    }
    mp->num = 0;
    mp->array_size = DEFAULT_INITIAL_POOL_SIZE;
    mp->array = calloc(mp->array_size , sizeof(void*));

    if(mp->array == NULL)
    {
        free(mp);
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_CALLOC_FAIL);
#endif
        return NULL;
    }

    return mp;
}


mem_pool* create_mem_pool_w_size(size_t inital_pool_size)
{
    mem_pool *mp = malloc(sizeof(*mp));
    if(mp == NULL)
    {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_MALLOC_FAIL);
#endif
        return NULL;
    }
    mp->num = 0;
    mp->array_size = inital_pool_size;
    mp->array = calloc(mp->array_size , sizeof(void*));

    if(mp->array == NULL)
    {
        free(mp);
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_CALLOC_FAIL);
#endif
        return NULL;
    }

    return mp;
}


void *pool_malloc(size_t size, mem_pool *pool)
{
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    // Allocate memory and add to pool
    void *mem_space = malloc(size);

    if(mem_space == NULL)
    {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_MALLOC_FAIL);
#endif
        return NULL;
    }
    pool->array[pool->num] = mem_space;
    pool->num++;

    // Increase the size of the pool
    if(pool->num >= pool->array_size)
    {
        void **tmp = realloc(pool->array, pool->array_size * 2 * sizeof(*tmp));
        if(tmp == NULL)
        {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
            return NULL;
        }
        pool->array = tmp;
        pool->array_size *= 2;
    }

    return mem_space;
}


void *pool_calloc(size_t nmemb, size_t size, mem_pool *pool)
{
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    // Allocate memory and add to pool
    void *mem_space = calloc(nmemb, size);

    if(mem_space == NULL)
    {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_CALLOC_FAIL);
#endif
        return NULL;
    }
    pool->array[pool->num] = mem_space;
    pool->num++;

    // Increase the size of the pool
    if(pool->num >= pool->array_size)
    {
        void **tmp = realloc(pool->array, pool->array_size * 2 * sizeof(*tmp));
        if(tmp == NULL)
        {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
            return NULL;
        }
        pool->array = tmp;
        pool->array_size *= 2;
    }

    return mem_space;
}


void *pool_realloc(void *mem_space, size_t size, mem_pool *pool)
{
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    int find = 0;
    size_t index = 0;

    for(; index < pool->num; index++)
    {
        if(find == 0 && pool->array[index] == mem_space)
        {
            find = 1;
            break;
        }
    }

    // If the memory doesn't belong to the pool
    if(find == 0)
    {
        assert(0);
        return NULL;
    }

    // Reallocate memory and add to pool
    void *tmp = realloc(mem_space, size);

    // If fail to realloc
    if(tmp == NULL)
    {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
        return NULL;
    }

    // Overwrite the old address in the pool
    pool->array[index] = tmp;

    return tmp;
}


int add_block_to_pool(void *block, mem_pool *pool)
{
    assert(block != NULL);
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    int find = 0;

    for(size_t i = 0; i < pool->num; i++)
    {
        if(find == 0 && pool->array[i] == block)
        {
            find = 1;
            break;
        }
    }

    // The memory should NOT be in the pool
    assert(find == 0);

    if(pool->num >= pool->array_size)
    {
        void **tmp = realloc(pool->array, pool->array_size * 2);
        if(tmp == NULL)
        {
#ifdef MEM_POOL_ERROR_PRINT
            fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
            return -1;
        }
        pool->array = tmp;
        pool->array_size *= 2;
    }

    pool->array[pool->num] = block;
    pool->num++;

    return 0;
}


int update_block_in_pool(void *block, void *new_block, mem_pool *pool)
{
    assert(block != NULL);
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    int find = 0;
    size_t index = 0;
    for(; index < pool->num; index++)
    {
        if(find == 0 && pool->array[index] == block)
        {
            find = 1;
            break;
        }
    }

    // The memory MUST be in the pool
    assert(find == 1);

    // Update
    pool->array[index] = new_block;

    return 0;
}


int free_one_in_pool(void *mem_space, mem_pool *pool)
{
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    int find = 0;

    for(size_t i = 0; i < pool->num; i++)
    {
        if(find == 0 && pool->array[i] == mem_space)
        {
            find = 1;
            free((pool->array)[i]);
            continue;
        }
        else if(find == 1)
        {
            pool->array[i-1] = pool->array[i];
        }
    }

    // If the memory doesn't belong to the pool
    assert(find == 1);

    pool->num--;

    return 0;
}


int free_whole_pool(mem_pool *pool)
{
    assert(pool != NULL);
    assert(pool->num <= pool->array_size);
    assert(pool->array != NULL);

    for(size_t i = 0; i < pool->num; i++)
    {
        free((pool->array)[i]);
    }
    pool->num = 0;

    return 0;
}


int merge_pool(mem_pool *dest, mem_pool *src)
{
    assert(dest != NULL);
    assert(src != NULL);

    if(src->num == 0)
    {
        return 0;
    }

    if(dest->array_size - dest->num < src->num)
    {
        size_t new_array_size = dest->array_size;

        while(new_array_size - dest->num < src->num)
        {
            new_array_size *= 2;
        }

        void **tmp = realloc(dest->array, new_array_size);

        if(tmp == NULL)
        {
#ifdef MEM_POOL_ERROR_PRINT
            fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
            return -1;
        }
        dest->array = tmp;
        dest->array_size = new_array_size;
    }

    for(size_t i = 0; i < src->num; i++)
    {
        (dest->array)[dest->num + i] = (src->array)[i];
    }

    dest->num += src->num;

    // Clear the src pool
    src->num = 0;

    return 0;
}


int move_one_between_pool(mem_pool *dest, mem_pool *src, void *mem_space)
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(mem_space != NULL);

    size_t index = 0;
    int find = 0;

    for(; index < src->num; index++)
    {
        if(src->array[index] == mem_space)
        {
            find = 1;
            break;
        }
    }

    assert(find == 1);

    // If reach the limit of array_size
    if(dest->num >= dest->array_size)
    {
        void **tmp = realloc(dest->array, dest->array_size * 2);
        if(tmp == NULL)
        {
#ifdef MEM_POOL_ERROR_PRINT
        fprintf(stderr, "mem_pool_error: %d\n", E_REALLOC_FAIL);
#endif
            return -1;
        }
        dest->array = tmp;
        dest->array_size *= 2;
    }

    // Add to dest
    dest->array[dest->num] = mem_space;
    dest->num++;

    // Remove from src
    for(size_t i = index+1; i < src->num; i++)
    {
        src->array[i-1] = src->array[i];
    }
    src->num--;

    return 0;
}


int destroy_pool(mem_pool *pool)
{
    free_whole_pool(pool);

    free(pool->array);
    free(pool);

    return 0;
}



