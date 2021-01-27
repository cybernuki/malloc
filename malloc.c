#include "malloc.h"

/**
 * _malloc_sbrk - extend the program break to allocate memory
 *
 * @needed: number of bytes needed for requested memory, padding and header
 * @chunk: pointer to the start of the unused chunk
 * @unused: number of unused allocated bytes
 *
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the start of the allocated memory region.
 */
static void *_malloc_sbrk(size_t needed, void *chunk, size_t unused)
{
	chunk_header_t *header = chunk;
	long page_size = sysconf(_SC_PAGESIZE);
	size_t n_pages =
		((needed + sizeof(*header) - unused - 1) / page_size) + 1;

	errno = 0;
	if (sbrk(page_size * n_pages) == (void *)-1 && errno)
	{
		error(0, errno, NULL);
		return (NULL);
	}
	CHUNK_SET_PREV(header, 0);
	CHUNK_SET_SIZE(header, page_size * n_pages + unused);
	CHUNK_SET_FREE(header);
	return (chunk);
}

/**
 * _malloc - dynamically allocate heap memory
 *
 * @size: number of bytes to allocate
 *
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the start of the allocated memory region.
 */
void *_malloc(size_t size)
{
	static void *start;
	void *chunk = start;
	chunk_header_t *header = chunk;
	size_t needed = size + PADDING(size) + sizeof(*header);
	size_t unused = 0;

	if (start)
	{
		while (CHUNK_GET_USED(header))
			header = chunk =
				(char *)chunk + CHUNK_GET_SIZE(header);
		unused = CHUNK_GET_SIZE(header);
	}
	else
	{
		chunk = start = sbrk(0);
	}
	if (unused < needed + sizeof(*header))
	{
		header = chunk = _malloc_sbrk(needed, chunk, unused);
		unused = CHUNK_GET_SIZE(header);
	}
	if (!chunk)
		return (NULL);
	CHUNK_SET_SIZE(header, needed);
	CHUNK_SET_USED(header);
	header = (chunk_header_t *)((char *)chunk + needed);
	CHUNK_SET_PREV(header, 0);
	CHUNK_SET_SIZE(header, unused - needed);
	CHUNK_SET_FREE(header);
	return ((char *)chunk + sizeof(*header));
}
