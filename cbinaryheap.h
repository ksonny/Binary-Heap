#ifndef _CBINARYHEAP_H_
#define _CBINARYHEAP_H_
/*
 * Implementation of a binary heap in C.
 *
 * Implementation of binary heap meant for inclusion in other projects. It has
 * therefore been built with customization in mind, using typedefs for types and
 * a function pointer for comparison. The implementation includes functions for
 * creating heaps, adding items and removing items from front of list. The heap
 * can be created from any array of compatible type and will work in-place.
 * The heap will not grow automatically when adding items to a full heap,
 * instead the operations will fail with an error message.
 */
/*
 * Binary heap storage type.
 *
 * Change this type according to your need.
 */
typedef unsigned int bheap_data_t;

typedef unsigned int bheap_capacity_t;

typedef unsigned int bheap_index_t;

typedef int (*bheap_comparator_t)(const bheap_data_t *a, const bheap_data_t *b);

/**
 * struct bheap - binary heap
 *
 * @capacity: capacity of storage
 * @used: capacity in use
 * @cmpf: pointer to comparison function
 * @ds: data storage array
 */
struct bheap {
	bheap_capacity_t capacity;
	bheap_capacity_t used;
	bheap_comparator_t cmpf;
	bheap_data_t *ds;
};

/*
 * bheap_empty - test if heap is empty
 */
int bheap_empty(const struct bheap *h);

/*
 * bheap_full - test if heap is full
 */
int bheap_full(const struct bheap *h);

/*
 * bheap_rebuild - rebuild heap
 */
void bheap_rebuild(struct bheap *h);

/*
 * bheap_create - create heap
 */
void bheap_create(struct bheap *h,
		const bheap_comparator_t cmpf,
		const bheap_capacity_t used,
		const bheap_capacity_t capacity,
		bheap_data_t *ds);

/*
 * bheap_add - add items to binary heap
 */
int bheap_add(struct bheap *h, const bheap_data_t *s);

/*
 * bheap_remove - remove highest priority item
 */
int bheap_remove(struct bheap *h);

/*
 * bheap_head - read highest priority item
 */
int bheap_head(const struct bheap *h, bheap_data_t *d);

/*
 * bheap_pop - read and remove highest priority item
 */
int bheap_pop(struct bheap *h, bheap_data_t *d);

#endif /* _CBINARYHEAP_H_ */
