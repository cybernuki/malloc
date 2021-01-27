#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "malloc.h"

/**
 * _naive_malloc_sbrk - extend the program break to allocate memory
 *
 * @size: size to allocate
 * @chunk: pointer to the start of the chunk
 * @remaining: number of unused allocated bytes
 *
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the start of the allocated memory region.
 */
static void *_naive_malloc_sbrk(size_t size, void *chunk, size_t remaining)
{
	long page_size = sysconf(_SC_PAGESIZE);
	size_t n_pages =
		((size + 2 * sizeof(size_t) - remaining - 1) / page_size) + 1;

	errno = 0;
	if (sbrk(page_size * n_pages) == (void *)-1 && errno == ENOMEM)
	{
		error(0, ENOMEM, NULL);
		return (NULL);
	}
	*((size_t *)(chunk)) = page_size * n_pages + remaining;
	return (chunk);
}

/**
 * naive_malloc - dynamically allocate memory
 *
 * @size: size to allocate
 *
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the start of the allocated memory region.
 */
void *naive_malloc(size_t size)
{
	static void *start;
	static size_t n_chunks;
	unsigned char *chunk = NULL;
	size_t remaining = 0;
	size_t chunk_index = 0;

	if (n_chunks)
	{
		for (chunk = start; chunk_index < n_chunks; ++chunk_index)
			chunk += *((size_t *)(chunk));
		remaining = *((size_t *)(chunk));
	}
	else
	{
		chunk = start = sbrk(0);
	}
	size += PADDING(size);
	if (remaining < size + 2 * sizeof(size_t))
		chunk = _naive_malloc_sbrk(size, chunk, remaining);
	if (!chunk)
		return (NULL);
	n_chunks += 1;
	remaining = *((size_t *)(chunk));
	*((size_t *)(chunk)) =
		size + sizeof(size_t);
	*((size_t *)(chunk + size + sizeof(size_t))) =
		remaining - (size + sizeof(size_t));
	return (chunk + sizeof(size_t));
}
