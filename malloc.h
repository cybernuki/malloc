#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "ANSI-color-codes.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define GET_SIZE(p) ((((block_info *)(p))->size) & LSB_ZERO_MASK)
#define GET_PREV(p) ((((block_info *)(p))->prev) & LSB_ZERO_MASK)
#define _GET_SIZE(p) (((block_info *)(p))->size)
#define _GET_PREV(p) (((block_info *)(p))->prev)

#define ALIGNMENT sizeof(void *)
#define align(offset, align) ((offset + (align - 1)) & -align)

#define MIN_SIZE sizeof(void *)

#define HDR_SZ sizeof(block_info)
#define align_up(num, align) (((num) + ((align)-1)) & ~((align)-1))

#define PAGESIZE sysconf(_SC_PAGESIZE)
#define LSB_ZERO_MASK 0xfffffffffffffffe

/**
 * struct chunk_s - malloc chunk header
 *
 * @prev_size: size of the previous chunk (only if not in-use)
 * @size: size of the chunk and in-use bit of the previous chunk
 * @next_free: pointer to the next free chunk in a circular doubly-linked list
 * @prev_free: pointer to the prev free chunk in a circular doubly-linked list
 */
typedef struct chunk_s
{
    size_t prev_size;
    size_t size;
    struct chunk_s *next_free;
    struct chunk_s *prev_free;
} chunk_t;

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t size);
void _free(void *ptr);

#endif /* _MALLOC_H_ */