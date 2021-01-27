#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <stddef.h>
#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define FOOT_SIZE (sizeof(size_t))
#define HEAD_SIZE (sizeof(size_t))

#define MIN_SIZE (FOOT_SIZE + HEAD_SIZE)
#define TOP_SIZE MIN_SIZE

#define ALIGNMENT (sizeof(void *))
#define PADDING(n) ((ALIGNMENT - ((n) & (ALIGNMENT - 1))) & (ALIGNMENT - 1))
#define ALIGNED(n) ((n) + PADDING(n))
#define NEEDED(n) (MIN_SIZE + ALIGNED(n))

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

#define CHUNK_PREV_USED_MASK ((size_t)1)
#define CHUNK_PREV_SIZE(p) ((p)->prev_size)
#define CHUNK_SIZE(p) ((p)->size & ~CHUNK_PREV_USED_MASK)
#define CHUNK_PREV_USED(p) ((p)->size & CHUNK_PREV_USED_MASK)
#define CHUNK_NEXT(p) ((chunk_t *)((char *)(p) + CHUNK_SIZE(p)))
#define CHUNK_PREV(p) ((chunk_t *)((char *)(p) - CHUNK_PREV_SIZE(p)))
#define CHUNK_SET_PREV_SIZE(p, n) ((p)->prev_size = (n))
#define CHUNK_SET_SIZE(p, n) ((p)->size = (n) | CHUNK_PREV_USED(p))
#define CHUNK_SET_PREV_USED(p) ((p)->size |= CHUNK_PREV_USED_MASK)
#define CHUNK_UNSET_PREV_USED(p) ((p)->size &= ~CHUNK_PREV_USED_MASK)

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void *_calloc(size_t nmemb, size_t size);
void *_realloc(void *ptr, size_t size);
void _free(void *ptr);

#endif /* _MALLOC_H_ */
