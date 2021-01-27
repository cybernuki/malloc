#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

/**
 * _sbrk_apply - extend the program break to allocate memory
 * @size: size to allocate
 * @chunk: pointer to the start of the chunk
 * @unused: number of unused allocated bytes
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the start of the allocated memory region.
 */
static void *_sbrk_apply(size_t size, void *chunk, size_t unused)
{
	long page_size = sysconf(_SC_PAGESIZE);
	size_t n_pages =
		((size + 2 * sizeof(size_t) - unused - 1) / page_size) + 1;

	errno = 0;
	if (sbrk(page_size * n_pages) == (void *)-1 && errno == ENOMEM)
	{
		error(0, ENOMEM, NULL);
		return (NULL);
	}
	*((size_t *)(chunk)) = page_size * n_pages + unused;
	return (chunk);
}

/**
 * naive_malloc - naive malloc that is presented in the concept page
 * @size: size needed to be allocated for the user
 * Return: return a pointer to the allocated memory that is suitably
 * aligned for any kind of variable
 */
void *naive_malloc(size_t size)
{
	static void *start;
	static size_t n_chunks;
	unsigned char *chunk = NULL;
	size_t unused = 0;
	size_t chunk_index = 0;

	if (n_chunks)
	{
		for (chunk = start; chunk_index < n_chunks; ++chunk_index)
			chunk += *((size_t *)(chunk));
		unused = *((size_t *)(chunk));
	}
	else
	{
		chunk = start = sbrk(0);
	}
	size += PADDING(size);
	if (unused < size + 2 * sizeof(size_t))
		chunk = _sbrk_apply(size, chunk, unused);
	if (!chunk)
		return (NULL);
	n_chunks += 1;
	unused = *((size_t *)(chunk));
	*((size_t *)(chunk)) =
		size + sizeof(size_t);
	*((size_t *)(chunk + size + sizeof(size_t))) =
		unused - (size + sizeof(size_t));
	return (chunk + sizeof(size_t));
}
