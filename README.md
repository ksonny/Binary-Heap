# C Binary Heap

Implementation of binary heap for inclusion in other projects. Built with customization and simplicity in mind, using typedefs for storage type and pointer for comparison function. Implementation includes functions for creating heaps, adding items and removing items from front of list. The heap can be created from any array of compatible type and will reuses array as storage. The heap will not grow automatically when adding items to a full heap, instead the operations will fail with an error message.

Example usage
-------------

``` C       
#include "cbinaryheap.h"

int bheap_max_comparator(const bheap_data_t *a, const bheap_data_t *b) 
{
	if (*a > *b)
		return 1;
	else if (*a < *b)
		return -1;
	else
		return 0;
}

int main(int argv, char *argc[])
{
	struct bheap heap;
	bheap_data_t ds[6] = {2, 0, 1, 1, 3};
	bheap_data_t a = 4, b;
	int i;

	bheap_create(&heap, &bheap_max_comparator, 5, 6, ds);
	bheap_add(&heap, &a);
	bheap_head(&heap, &b);
	if (a == b)
		printf("true\n");
	else
		printf("false, %u, %u.\n", a, b);

	printf("Print sorted order:   ");
	while (!bheap_empty(&heap)) {
		bheap_pop(&heap, &b);
		printf("%u ", b);
	}
	printf("\n");

        return 0;
}
```
