/*
 * Binary heap
 *
 * Macros for generating a layer of functionality that allows
 * any array to be used as a priority queue.
 *
 * Features:
 * - Any type can be used.
 *
 * - Any array can be converted into a binary heap.
 *   Reduced overhead when data already loaded.
 *
 * - Preprocessor generates native code.
 *   All code is type checked.
 *   Fast.
 *
 * - All generated code is enclosed in functions.
 *   Any error inside a macro references correct function.
 *   (Useful as hell since macro's break line numbering.)
 *
 * - No memory allocated.
 *   Simplify reasoning about code.
 *   (But you have to make sure capacity is available!)
 *
 * - ANSI C
 */
#ifndef _CBINARYHEAPMACRO_H_
#define _CBINARYHEAPMACRO_H_

#include <stddef.h>
#include <assert.h>

typedef int bheap_capacity_t;
typedef int bheap_index_t;

static bheap_index_t bheap_parent(const bheap_index_t i)
{
	assert(i >= 0);
	return (i - 1) / 2;
}
static bheap_index_t bheap_lchild(const bheap_index_t i)
{
	assert(i >= 0);
	return i + i + 1;
}
static bheap_index_t bheap_rchild(const bheap_index_t i)
{
	assert(i >= 0);
	return i + i + 2;
}

#define bheap_struct_define(name, type)		\
	struct bheap_##name {			\
		bheap_capacity_t capacity;	\
		bheap_capacity_t used;		\
		type *ds;			\
	};

/**
 * bheap_empty_define() - Generate empty test function
 * @name: identifying name of heap type
 */
#define bheap_empty_define(name)				\
	int bheap_##name##_empty(const struct bheap_##name *h)	\
	{							\
		return !(h->used > 0);				\
	}

/**
 * bheap_full_define() - Generate full test function
 * @name: identifying name of heap type
 */
#define bheap_full_define(name)					\
	int bheap_##name##_full(const struct bheap_##name *h)	\
	{							\
		return !(h->used < h->capacity);		\
	}

#define bheap_swap_define(name, type)					\
	static void bheap_##name##_swap(struct bheap_##name *h,		\
				const bheap_index_t i,			\
				const bheap_index_t j)			\
	{								\
		type t;							\
		t        = h->ds[i];					\
		h->ds[i] = h->ds[j];					\
		h->ds[j] = t;						\
	}

#define bheap_heapifyup_define(name)					\
	static void bheap_##name##_heapifyup(struct bheap_##name *h,	\
					const bheap_index_t i)		\
	{								\
		bheap_index_t j = bheap_parent(i);			\
		if (i < 0 || i >= h->used)				\
			return;						\
		else if (compare_##name(h->ds + i, h->ds + j) > 0) {	\
			bheap_##name##_swap(h, i, j);			\
			bheap_##name##_heapifyup(h, j);			\
		}							\
	}

#define bheap_heapifydown_define(name)					\
	static void bheap_##name##_heapifydown(struct bheap_##name *h,	\
					const bheap_index_t i)		\
	{								\
		bheap_index_t j, largest = i;				\
		j = bheap_lchild(i);					\
		if (j < h->used && compare_##name(h->ds + j, h->ds + largest) > 0) \
			largest = j;					\
		j = bheap_rchild(i);					\
		if (j < h->used && compare_##name(h->ds + j, h->ds + largest) > 0) \
			largest = j;					\
		if (largest != i) {					\
			bheap_##name##_swap(h, i, largest);		\
			bheap_##name##_heapifydown(h, largest);		\
		}							\
	}

/**
 * bheap_rebuild_define() - Generate rebuild function
 * @name: identifying name of heap type
 *
 * Generate function that rebuilds heap structure,
 * argument is a heap.
 *
 * Realizing that a heap is a collection of smaller heaps,
 * we make sure all heaps individually satisfies heap
 * property. This can be done in O(n), faster then inserting
 * all elements in new heap O(n*log(n)).
 */
#define bheap_rebuild_define(name)					\
	void bheap_##name##_rebuild(struct bheap_##name *h)		\
	{								\
		bheap_index_t i;					\
		if (bheap_##name##_empty(h))				\
			return;						\
		for (i = h->used / 2; i >= 0; i--)			\
			bheap_##name##_heapifydown(h, i);		\
	}

/**
 * bheap_create_define() - Generate create function
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Generate function that initiate a new heap structure,
 * arguments are a heap, capacity used, available capacity
 * and a pointer to a array of elements.
 *
 * Both heap and element array must be exist!
 */
#define bheap_create_define(name, type)					\
	void bheap_##name##_create(struct bheap_##name *h,		\
				const bheap_capacity_t used,		\
				const bheap_capacity_t capacity,	\
				type *ds)				\
	{								\
		assert(h != NULL);					\
		assert(used <= capacity);				\
		assert(ds != NULL);					\
		h->used     = used;					\
		h->capacity = capacity;					\
		h->ds       = ds;					\
		bheap_##name##_rebuild(h);				\
	}

/**
 * bheap_add_define() - Generate add function
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Generate function that adds an element to heap,
 * arguments are a heap and a pointer to element.
 *
 * Note that memory allocation is all up to the user.
 * If add returns 0, you need to add more space by either
 * realloc h->ds and increment h->capacity, or create
 * a new heap and copy all data.
 */
#define bheap_add_define(name, type)					\
	int bheap_##name##_add(struct bheap_##name *h, const type *s)	\
	{								\
		assert(s != NULL);					\
		if (bheap_##name##_full(h))				\
			return 0;					\
		h->used++;						\
		h->ds[h->used - 1] = *s;				\
		bheap_##name##_heapifyup(h, h->used - 1);		\
		return 1;						\
	}

/**
 * bheap_remove_define() - Generate remove functions
 * @name: identifying name of heap type
 *
 * Generate two remove functions, remove_at and remove.
 * remove_at removes an element at any position in heap,
 * arguments are a heap and an index.
 * remove removes first element on heap,
 * arguments is a heap.
 *
 * Heap is resorted by replacing removed element with last element and
 * sorting downwards.
 */
#define bheap_remove_define(name)				\
	int bheap_##name##_remove_at(struct bheap_##name *h,	\
				const bheap_index_t i)		\
	{							\
		assert(i >= 0 && i < h->used);			\
		if (bheap_##name##_empty(h))			\
			return 0;				\
		h->used--;					\
		h->ds[i] = h->ds[h->used];			\
		bheap_##name##_heapifydown(h, i);		\
		return 1;					\
	}							\
	int bheap_##name##_remove(struct bheap_##name *h)	\
	{							\
		return bheap_##name##_remove_at(h, 0);		\
	}

/**
 * bheap_head_define() - Generate head function
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Generate function that returns pointer to first element,
 * argument is a heap.
 */
#define bheap_head_define(name, type)				\
	type *bheap_##name##_head(const struct bheap_##name *h)	\
	{							\
		if (bheap_##name##_empty(h))			\
			return NULL;				\
		return h->ds;					\
	}

/**
 * bheap_pop_define() - Generate pop function
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Generate function that extracts and remove first element from heap,
 * arguments are a heap and an element to add.
 */
#define bheap_pop_define(name, type)					\
	int bheap_##name##_pop(struct bheap_##name *h, type *d)		\
	{								\
		assert(d != NULL);					\
		if (bheap_##name##_empty(h))				\
			return 0;					\
		*d = *bheap_##name##_head(h);				\
		return bheap_##name##_remove(h);			\
	}

/**
 * bheap_prototypes() - Allows using already defined heap
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Allows heap function definitions to be used in any file by
 * generating the correct prototypes.
 *
 */
#define bheap_prototypes(name, type)					\
	struct bheap_##name;						\
	extern int bheap_##name##_empty(const struct bheap_##name *h);	\
	extern int bheap_##name##_full(const struct bheap_##name *h);	\
	extern void bheap_##name##_rebuild(struct bheap_##name *h);	\
	extern void bheap_##name##_create(struct bheap_##name *h,	\
					const bheap_capacity_t used,	\
					const bheap_capacity_t capacity, \
					type *ds);			\
	extern int bheap_##name##_add(struct bheap_##name *h, const type *s); \
	extern int bheap_##name##_remove_at(struct bheap_##name *h,	\
					const bheap_index_t i);		\
	extern int bheap_##name##_remove(struct bheap_##name *h);	\
	extern type *bheap_##name##_head(const struct bheap_##name *h);	\
	extern int bheap_##name##_pop(struct bheap_##name *h, type *d);

/**
 * bheap_definitions() - Generate struct and functions
 * @name: identifying name of heap type
 * @type: type stored in heap
 *
 * Expects a compare function to exist in same scope,
 * int compare_<name>(const type *a, const type *b).
 * Compare returns 1 if a has higher priority then b else return 0.
 * If compare is not defined, you will get an undefined symbols error.
 */
#define bheap_definitions(name, type)		\
	bheap_struct_define(name, type)		\
	bheap_swap_define(name, type)		\
	bheap_empty_define(name)		\
	bheap_full_define(name)			\
	bheap_heapifyup_define(name)		\
	bheap_heapifydown_define(name)		\
	bheap_rebuild_define(name)		\
	bheap_create_define(name, type)		\
	bheap_add_define(name, type)		\
	bheap_remove_define(name)		\
	bheap_head_define(name, type)		\
	bheap_pop_define(name, type)

#endif /* _CBINARYHEAPMACRO_H_ */
