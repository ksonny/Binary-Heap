# Binary Heap

Macros for generating a layer of functionality that allows
arrays to be used as a priority queue.

Features:

- Any type can be used.                                                                                                                                         
                                                                                                                                                                
- Any array can be converted into a binary heap.                                                                                                                
  Reduced overhead when data already loaded.                                                                                                                    
                                                                                                                                                                
- Preprocessor generates native code.                                                                                                                           
  All code is type checked.                                                                                                                                     
  Fast.                                                                                                                                                         
                                                                                                                                                                
- All generated code is enclosed in functions.                                                                                                                  
  Any error inside a macro references correct function.                                                                                                         
  (Very useful when macros break line numbering.)                                                                                                          
                                                                                                                                                                
- No memory allocated.                                                                                                                                          
  Simplify reasoning about code.                                                                                                                                
  (But you have to make sure capacity is available!)                                                                                                            
                                                                                                                                                                
- ANSI C

Example usage
-------------

``` C
#include <stdio.h>
#include "cbh_macro.h"

int compare_test(const int *a, const int *b)
{
	return (*a > *b);
}

bheap_generate_definitions(test, int)

int main(int argv, char *argc[])
{
	struct bheap_test heap;
	int ns[7] = {5, 3, 0, 1, 2, 4};
	int a = 6;
	int b;

	bheap_test_create(&heap, 6, 7, ns);

	bheap_test_add(&heap, &a);
	if (a == *bheap_test_head(&heap))
		printf("largest is first\n");
	else
		printf("fuckup");

	printf("Print sorted order:   ");
	while (!bheap_test_empty(&heap)) {
		printf("%u ", *bheap_test_head(&heap));
		bheap_test_remove(&heap);
	}
	printf("\n");

	return 0;
}
```
