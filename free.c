#include "malloc.h"

/**
 * _free - free implementation
 * @ptr: pointer
*/
void _free(void *ptr)
{
	char *index = NULL, *next = NULL;

	if (!ptr)
		return;
	index = ptr;
	index -= HDR_SZ;
	next = p + GET_SIZE(index);
	if (_GET_SIZE(next) & 1)
	{
		putchar('\n');
		kill(getpid(), SIGABRT);
	}
	((block_info *)next)->prev = GET_SIZE(index);
	((block_info *)next)->size |= 1;
}
