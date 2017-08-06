#include <stdio.h>
#include <assert.h>
#include "mem_pool.h"

#define DEFAULT_INTIAL_POOL_SIZE 64

#define MEM_POOL_ERROR_PRINT

// Read write inbetween allocations
int test_1();

// Free one in pool
int test_2();

// Free whole
int test_3();

// Read write after all allocations
int test_4();

// Realloc
int test_5();

// Test Super Large Memory Allocation
int test_6();

// Test merge_pool
int test_7();

// Empty Merge
int test_8();

// move_one_between_pool
int test_9();

// add_block_to_pool
int test_10();

int test_11();

int main()
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    return 0;
}


int test_1()
{
    const size_t UNIT_SIZE = 10;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < DEFAULT_INITIAL_POOL_SIZE * 8 + 5; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;

        // Write
        for(size_t j = 0; j < UNIT_SIZE; j--)
        {
            str1[j] = '1'+ i % 9;
        }

        // Read
        for(size_t j = 0; j < UNIT_SIZE; j--)
        {
            assert(str1[j] == '1'+ i % 9);
        }
    }
    assert(mp->array_size == DEFAULT_INTIAL_POOL_SIZE * 8 * 2);

    free_whole_pool(mp);
    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_2()
{
    const size_t UNIT_SIZE = 10;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < DEFAULT_INITIAL_POOL_SIZE * 8; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;

        if(i % 5 == 0 || i % 7 == 0)
        {
            size_t prev_num = mp->num;

            free_one_in_pool(str1, mp);

            assert(prev_num - 1 == mp->num);
        }
    }

    free_whole_pool(mp);
    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    goto CleanUp;
}


int test_3()
{
    const size_t UNIT_SIZE = 10;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < DEFAULT_INITIAL_POOL_SIZE * 8; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;

        if(i % 5 == 0)
        {
            free_whole_pool(mp);
            assert(mp->num == 0);
        }
    }

    free_whole_pool(mp);
    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_4()
{
    const size_t UNIT_SIZE = 51200;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;
    }

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = mp->array[i];
        assert(str1 != NULL);

        // Write
        for(size_t j = 0; j < UNIT_SIZE; j--)
        {
            str1[j] = '1'+ i % 9;
        }

        // Read
        for(size_t j = 0; j < UNIT_SIZE; j--)
        {
            assert(str1[j] == '1'+ i % 9);
        }

    }
    free_whole_pool(mp);
    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_5()
{
    const size_t UNIT_SIZE = 51200;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;

        char *tmp = pool_realloc(str1, 10 + UNIT_SIZE * sizeof(*tmp), mp);
        if(mp == NULL)
            goto CleanUp;
        str1 = tmp;

        tmp = pool_realloc(str1, 2*UNIT_SIZE * sizeof(*tmp), mp);
        if(mp == NULL)
            goto CleanUp;
        str1 = tmp;
    }

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = mp->array[i];
        if(str1 == NULL)
            goto CleanUp;

        // Write
        for(size_t j = 0; j < 2*UNIT_SIZE; j--)
        {
            str1[j] = '1'+ i % 9;
        }

        // Read
        for(size_t j = 0; j < 2*UNIT_SIZE; j--)
        {
            assert(str1[j] == '1'+ i % 9);
        }

    }
    free_whole_pool(mp);
    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_6()
{
    // Test Super Large Memory Allocation

    const size_t UNIT_SIZE = 51200;
    const unsigned int SUPER_LARGE = ~1024;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
    if(str1 == NULL)
        goto CleanUp;

    char *tmp = pool_realloc(str1, SUPER_LARGE, mp);
    if(tmp == NULL)
        goto CleanUp;
    str1 = tmp;

    free_whole_pool(mp);

    char *str2 = pool_malloc(SUPER_LARGE, mp);
    if(str2 == NULL)
        goto CleanUp;
    free_whole_pool(mp);


    destroy_pool(mp);

    return 0;

CleanUp:

    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_7()
{
    const size_t UNIT_SIZE = 32;
    mem_pool *mp1 = create_mem_pool();
    mem_pool *mp2 = create_mem_pool();

    if(mp1 == NULL || mp2 == NULL)
        goto CleanUp;

    for(size_t i = 0; i < 256; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp1);
        if(str1 == NULL)
            goto CleanUp;
    }

    for(size_t i = 0; i < 1024; i++)
    {
        char *str2 = pool_malloc(2 * UNIT_SIZE * sizeof(*str2), mp2);
        if(str2 == NULL)
            goto CleanUp;
    }
    if(merge_pool(mp2, mp1) == -1)
        goto CleanUp;

    assert(mp2->num == 1024 + 256);
    assert(mp1->num == 0);

    for(size_t i = 0; i < mp2->num; i++)
    {
        if(i < 1024)
        {
            char *tmp_str = (char*)(mp2->array[i]);
            for(size_t j = 0; j < 2 * UNIT_SIZE; j++)
            {
                char c = 'a' + j % 26;
                // Write
                tmp_str[j] = c;

                // Read
                c = tmp_str[j];
            }
        }
        else
        {
            char *tmp_str = (char*)(mp2->array[i]);
            for(size_t j = 0; j < UNIT_SIZE; j++)
            {
                char c = 'a' + j % 26;
                // Write
                tmp_str[j] = c;

                // Read
                c = tmp_str[j];
            }

        }
    }

    destroy_pool(mp1);
    destroy_pool(mp2);

    return 0;

CleanUp:

    if(mp1 != NULL) destroy_pool(mp1);
    if(mp2 != NULL) destroy_pool(mp2);
    return 1;
}


int test_8()
{
    const size_t UNIT_SIZE = 32;
    mem_pool *mp1 = create_mem_pool();
    mem_pool *mp2 = create_mem_pool();

    if(mp1 == NULL || mp2 == NULL)
        goto CleanUp;


    for(size_t i = 0; i < 1024; i++)
    {
        char *str2 = pool_malloc(2 * UNIT_SIZE * sizeof(*str2), mp2);
        if(str2 == NULL)
            goto CleanUp;
    }
    if(merge_pool(mp2, mp1) == -1)
        goto CleanUp;

    assert(mp2->num == 1024);
    assert(mp1->num == 0);

    for(size_t i = 0; i < mp2->num; i++)
    {
        char *tmp_str = (char*)(mp2->array[i]);
        for(size_t j = 0; j < 2 * UNIT_SIZE; j++)
        {
            char c = 'a' + j % 26;
            // Write
            tmp_str[j] = c;

            // Read
            c = tmp_str[j];
        }

    }

    destroy_pool(mp1);
    destroy_pool(mp2);

    return 0;

CleanUp:

    if(mp1 != NULL) destroy_pool(mp1);
    if(mp2 != NULL) destroy_pool(mp2);
    return 1;
}


int test_9()
{
    const size_t UNIT_SIZE = 51200;
    mem_pool *mp1 = create_mem_pool();
    mem_pool *mp2 = create_mem_pool();

    if(mp1 == NULL || mp2 == NULL)
        goto CleanUp;

    char *tmp1 = NULL;
    char *tmp2 = NULL;

    for(size_t i = 0; i < 8; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp1);
        if(str1 == NULL)
            goto CleanUp;
        if(i % 3 == 0)
            tmp1 = str1;
        else if(i % 5 == 0)
            tmp2 = str1;
    }

    for(size_t i = 0; i < 3; i++)
    {
        char *str2 = pool_malloc(2 * UNIT_SIZE * sizeof(*str2), mp2);
        if(str2 == NULL)
            goto CleanUp;
    }
    if(move_one_between_pool(mp2, mp1, tmp1) == -1)
        goto CleanUp;
    if(move_one_between_pool(mp2, mp1, tmp2) == -1)
        goto CleanUp;

    assert(mp2->num == 3 + 2);
    assert(mp1->num == 8 - 2);

    for(size_t i = 0; i < mp2->num; i++)
    {
        if(i < 3)
        {
            char *tmp_str = (char*)(mp2->array[i]);
            for(size_t j = 0; j < 2 * UNIT_SIZE; j++)
            {
                char c = 'a' + j % 26;
                // Write
                tmp_str[j] = c;

                // Read
                c = tmp_str[j];
            }
        }
        else
        {
            char *tmp_str = (char*)(mp2->array[i]);
            for(size_t j = 0; j < UNIT_SIZE; j++)
            {
                char c = 'a' + j % 26;
                // Write
                tmp_str[j] = c;

                // Read
                c = tmp_str[j];
            }

        }
    }

    destroy_pool(mp1);
    destroy_pool(mp2);

    return 0;

CleanUp:

    if(mp1 != NULL) destroy_pool(mp1);
    if(mp2 != NULL) destroy_pool(mp2);
    return 1;
}


int test_10()
{
    const size_t UNIT_SIZE = 512;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;
    }

    char *tmp = malloc(10 * UNIT_SIZE);
    if(tmp == NULL)
        goto CleanUp;

    add_block_to_pool(tmp, mp);

    assert(mp->num == 11);
    assert(mp->array[10] == tmp);

    free_whole_pool(mp);


    destroy_pool(mp);

    return 0;

CleanUp:

    free(tmp);
    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}


int test_11()
{
    const size_t UNIT_SIZE = 512;
    mem_pool *mp = create_mem_pool();

    if(mp == NULL)
        goto CleanUp;

    for(size_t i = 0; i < 10; i++)
    {
        char *str1 = pool_malloc(UNIT_SIZE * sizeof(*str1), mp);
        if(str1 == NULL)
            goto CleanUp;
    }

    char *block = malloc(10 * UNIT_SIZE);
    if(block == NULL)
        goto CleanUp;

    add_block_to_pool(block, mp);
    

    assert(mp->num == 11);
    assert(mp->array[10] == block);

    char *tmp = realloc(block, 20 * UNIT_SIZE);
    if(tmp == NULL)
    {
        goto CleanUp;
    }
    update_block_in_pool(block, tmp, mp);
    block = tmp;

    for(size_t i = 0; i < 20 * UNIT_SIZE; i++)
    {
        char c = 'a' + i % 26;

        // Write
        tmp[i] = c;

        // Read
        c = tmp[i];
    }


    destroy_pool(mp);

    return 0;

CleanUp:

    free(tmp);
    free_whole_pool(mp);
    destroy_pool(mp);
    return 1;
}




