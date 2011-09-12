#include "cbinaryheap.h"

static bheap_index_t bheap_parent(const bheap_index_t i)
{
	return (i - 1) / 2;
}
static bheap_index_t bheap_lchild(const bheap_index_t i)
{
	return 2 * i + 1;
}
static bheap_index_t bheap_rchild(const bheap_index_t i)
{
	return 2 * i + 2;
}

static void bheap_swap(struct bheap *h,
		const bheap_index_t i, 
		const bheap_index_t j)
{
	bheap_data_t t;
	t	 = h->ds[i];
	h->ds[i] = h->ds[j];
	h->ds[j] = t;
}

/**
 * bheap_compare - compares two elements in binary heap
 *
 * @h: pointer to binary heap
 * @i: index of node
 * @j: index of node
 *
 * Function is used in heap to determine relative priority of two items.
 * Higher priority items go towards top of heap.
 * Returns negative integer, zero or a positive integer as the priority of
 * node i is less then, equal to, or greater than node j.
 *
 * Note: Entries outside of used heap always compare less then entries inside.
 */
static int bheap_compare(struct bheap *h,
			const bheap_index_t i,
			const bheap_index_t j)
{
	if (i >= h->used)
		return -1;
	else if (j >= h->used)
		return 1;
	else
		return (*h->cmpf)(h->ds + i, h->ds + j);
}

static void bheap_heapifyup(struct bheap *h, const bheap_index_t i)
{
	bheap_index_t j = bheap_parent(i);

	if (i > 0 && bheap_compare(h, i, j) > 0) {
		bheap_swap(h, i, j);
		bheap_heapifyup(h, j);
	}
}

static void bheap_heapifydown(struct bheap *h, const bheap_index_t i)
{
	bheap_index_t largest = i, j;
	
	j = bheap_lchild(i);
	if (bheap_compare(h, j, largest) > 0)
		largest = j;

	j = bheap_rchild(i);
	if (bheap_compare(h, j, largest) > 0) 
		largest = j;
	
	if (largest != i) {
		bheap_swap(h, i, largest);
		bheap_heapifydown(h, largest);
	}
}

/**
 * bheap_empty - test if heap is empty
 *
 * @h: pointer to binary heap
 *
 * Check if any capacity is in use.
 */
int bheap_empty(const struct bheap *h)
{
	return !(h->used > 0);
}

/**
 * bheap_full - test if heap is full
 *
 * @h: pointer to binary heap
 *
 * Check if used is less then available capacity.
 */
int bheap_full(const struct bheap *h)
{
	return !(h->used < h->capacity);
}

/**
 * bheap_rebuild - rebuild heap
 *
 * @h: pointer to binary heap
 *
 * For every child in heap, swap with parent if priority is higher.
 */
void bheap_rebuild(struct bheap *h)
{
	bheap_index_t i, j;
	for (i = h->used - 1; i > 0; i--) {
		j = bheap_parent(i);
		if (bheap_compare(h, i, j) > 0)
			bheap_swap(h, i, j);
	}	
}

/**
 * bheap_create - create heap
 *
 * @h: pointer to binary heap
 * @cmpf: pointer to compare function
 * @used: capacity in use
 * @capacity: capacity available
 * @ds: pointer to array of items
 *
 * Initiate entries and sort binary heap.
 */
void bheap_create(struct bheap *h,
		const bheap_comparator_t cmpf,
		const bheap_capacity_t used,
		const bheap_capacity_t capacity,
		bheap_data_t *ds)
{
	h->cmpf = cmpf;
	h->used = used;
	h->capacity = capacity;
	h->ds = ds;
	bheap_rebuild(h);
}

/**
 * bheap_add - add items to binary heap
 *
 * @h: pointer to binary heap
 * @s: pointer to item
 *
 * Copy item into heap and move upward until heap properties are satisfied.
 */
int bheap_add(struct bheap *h, const bheap_data_t *s)
{
	if (bheap_full(h))
		return 0;
	h->used++;
	h->ds[h->used - 1] = *s;
	bheap_heapifyup(h, h->used - 1);
	return 1;
}

/**
 * bheap_remove - remove highest priority item
 *
 * @h: pointer to binary heap
 *
 * Move last item first and move downwards until heap priorities are satisfied.
 */
int bheap_remove(struct bheap *h)
{
	if (bheap_empty(h))
		return 0;
	h->used--;
	h->ds[0] = h->ds[h->used];
	bheap_heapifydown(h, 0);
	return 1;
}

/**
 * bheap_head - read highest priority item
 *
 * @h: pointer to binary heap
 * @d: item destination
 */
int bheap_head(const struct bheap *h, bheap_data_t *d)
{
	if (bheap_empty(h))
		return 0;
	*d = h->ds[0];
	return 1;
}

/**
 * bheap_pop - read and remove highest priority item
 *
 * @h: pointer to binary heap
 * @d: item destination
 */
int bheap_pop(struct bheap *h, bheap_data_t *d)
{
	return (bheap_head(h, d) && bheap_remove(h));
}
