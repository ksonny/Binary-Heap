#ifndef _CBINARYHEAPMACRO_H_
#define _CBINARYHEAPMACRO_H_
/*
 */
typedef int bheap_capacity_t;

typedef int bheap_index_t;

/*
 * Our implementation uses a list for representing a heap.
 * These three functions are used for traversing the heap.
 */
static bheap_index_t bheap_parent(const bheap_index_t i)
{
	return (i - 1) / 2;
}
static bheap_index_t bheap_lchild(const bheap_index_t i)
{
	return i + i + 1;
}
static bheap_index_t bheap_rchild(const bheap_index_t i)
{
	return i + i + 2;
}

#define bheap_struct_define(name, type)		\
	struct bheap_##name {			\
		bheap_capacity_t capacity;	\
		bheap_capacity_t used;		\
		type *ds;			\
	};

#define bheap_empty_define(name)				\
	int bheap_##name##_empty(struct bheap_##name *h)	\
	{							\
		return !(h->used > 0);				\
	}

#define bheap_full_define(name)					\
	int bheap_##name##_full(struct bheap_##name *h)		\
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


#define bheap_rebuild_define(name)					\
	void bheap_##name##_rebuild(struct bheap_##name *h)		\
	{								\
		bheap_index_t i;					\
		for (i = h->used / 2; i >= 0; i--)			\
			bheap_##name##_heapifydown(h, i);		\
	}

#define bheap_create_define(name, type)					\
	void bheap_##name##_create(struct bheap_##name *h,		\
				const bheap_capacity_t used,		\
				const bheap_capacity_t capacity,	\
				type *ds)				\
	{								\
		h->used     = used;					\
		h->capacity = capacity;					\
		h->ds       = ds;					\
		bheap_##name##_rebuild(h);				\
	}

#define bheap_add_define(name, type)					\
	int bheap_##name##_add(struct bheap_##name *h, const type *s)	\
	{								\
		if (bheap_##name##_full(h))				\
			return 0;					\
		h->used++;						\
		h->ds[h->used - 1] = *s;				\
		bheap_##name##_heapifyup(h, h->used - 1);		\
		return 1;						\
	}

#define bheap_remove_define(name)				\
	int bheap_##name##_remove_at(struct bheap_##name *h,	\
				const bheap_index_t i)		\
	{							\
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

#define bheap_head_define(name, type)				\
	type *bheap_##name##_head(struct bheap_##name *h)	\
	{							\
		if (bheap_##name##_empty(h))			\
			return (void *)0;			\
		return h->ds;					\
	}

#define bheap_pop_define(name, type)					\
	int bheap_##name##_pop(struct bheap_##name *h, type *d)		\
	{								\
		const type *tmp = bheap_##name##_head(h);		\
		if (tmp != (void *)0)					\
			*d = *tmp;					\
		return bheap_##name##_remove(h);			\
	}

#define bheap_prototypes(name, type)					\
	struct bheap_##name;						\
	extern int bheap_##name##_empty(struct bheap_##name *h);	\
	extern int bheap_##name##_full(struct bheap_##name *h);		\
	extern void bheap_##name##_rebuild(struct bheap_##name *h);	\
	extern void bheap_##name##_create(struct bheap_##name *h,	\
					const bheap_capacity_t used,	\
					const bheap_capacity_t capacity, \
					type *ds);			\
	extern int bheap_##name##_add(struct bheap_##name *h, const type *s); \
	extern int bheap_##name##_remove_at(struct bheap_##name *h,	\
					const bheap_index_t i);		\
	extern int bheap_##name##_remove(struct bheap_##name *h);	\
	extern type *bheap_##name##_head(struct bheap_##name *h);	\
	extern int bheap_##name##_pop(struct bheap_##name *h, type *d);


#define bheap_generate_definitions(name, type)	\
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
