#ifndef RC_FLOATHASH_H
#define RC_FLOATHASH_H

typedef struct FloatHashEntry_t FloatHashEntry_t;

struct FloatHashEntry_t {

	u32					index;
	u32					pindex;
	u32					next;
//	FloatHashEntry_t	*next;

};

typedef struct {

	u32					table_size;
	u32					*hash_table;

	u32					next_free;
	u32					max_free;
	FloatHashEntry_t	*free_list;

} FloatHash;

#define RC_FLOAT_HASH_DEFAULT_SIZE		40001

FloatHash *FloatHashInit(
		u32			size
	);

void FloatHashDestroy(
		FloatHash	*fh
	);

u32 FloatHashCalcHash(
		char		*a,
		i32			n
	);

void FloatHashAdd(
		FloatHash	*fh,
		f32			*ptr,
		u32			pindex,
		i32			index,
		i32			n
	);

i32 FloatHashFind(
		FloatHash	*fh,
		f32			*list,
		f32			*ptr,
		i32			n
	);

void FloatHashClean(
		FloatHash	*fh
	);

#endif /* __FLOATHASH_H */

