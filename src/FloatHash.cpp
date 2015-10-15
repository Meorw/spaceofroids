#include "RenderChimp.h"
#include "FloatHash.h"

/*---------------------------------------------------------------------------*/

FloatHash *FloatHashInit(
		u32			size
	)
{
	FloatHash *fh;

	if (size == 0)
		size = RC_FLOAT_HASH_DEFAULT_SIZE;

	fh = (FloatHash *) malloc(sizeof(FloatHash));
	fh->hash_table = (u32 *) malloc(sizeof(u32) * size);

	memset(fh->hash_table, 0, sizeof(u32) * size);

	fh->free_list = (FloatHashEntry_t *) malloc(sizeof(FloatHashEntry_t) * (size >> 2));

	fh->max_free = size >> 2;
	fh->next_free = 1;

	fh->table_size = size;

	return fh;
}

/*---------------------------------------------------------------------------*/

void FloatHashDestroy(
		FloatHash	*fh
	)
{
	free(fh->free_list);
	free(fh->hash_table);
	free(fh);
}

/*---------------------------------------------------------------------------*/

 /* "Fattigmanshash" */
u32 FloatHashCalcHash(
		char		*a,
		i32			n
	)
{
	i32 i;
	u32 h = 0;

	for (i = 0; i < n; i++) {
		h = h * 33 + (*a);
		a++;
	}

	return h;
}

/*---------------------------------------------------------------------------*/

void FloatHashAdd(
		FloatHash	*fh,
		f32			*list,
		u32			pindex,
		i32			index,
		i32			n
	)
{
	FloatHashEntry_t *entry;
	u32 h = FloatHashCalcHash((char *) &list[pindex], n * sizeof(f32)) % fh->table_size;

	if (fh->hash_table[h]) {
		entry = &fh->free_list[fh->hash_table[h]];

		for (;;) {
			if (memcmp(&list[entry->pindex], &list[pindex], n * sizeof(f32)) == 0) {
				return;
			}

			if (!entry->next)
				break;

			entry = &fh->free_list[entry->next];
		}
	}

	if (fh->next_free == fh->max_free) {
		fh->max_free = fh->max_free + (fh->max_free >> 1); /* Grow by 50% */
		fh->free_list = (FloatHashEntry_t *) realloc(fh->free_list, sizeof(FloatHashEntry_t) * fh->max_free);
	}

	entry = &fh->free_list[fh->next_free];
	entry->index = index;
	entry->pindex = pindex;
	entry->next = fh->hash_table[h];
	fh->hash_table[h] = fh->next_free++;
}

/*---------------------------------------------------------------------------*/

i32 FloatHashFind(
		FloatHash	*fh,
		f32			*list,
		f32			*ptr,
		i32			n
	)
{
	u32 h = FloatHashCalcHash((char *) ptr, n * sizeof(f32)) % fh->table_size;

	if (fh->hash_table[h]) {
		FloatHashEntry_t *entry = &fh->free_list[fh->hash_table[h]];

		for (;;) {
			if (memcmp(&list[entry->pindex], ptr, n * sizeof(f32)) == 0) {
				return entry->index;
			}

			if (!entry->next)
				break;

			entry = &fh->free_list[entry->next];
		}
	}

	return -1;
}

/*---------------------------------------------------------------------------*/

void FloatHashClean(
		FloatHash	*fh
	)
{
	u32 i;

	for (i = 0; i < fh->table_size; i++) {
		fh->hash_table[i] = 0;
	}

	fh->next_free = 0;
}

/*---------------------------------------------------------------------------*/



