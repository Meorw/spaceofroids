#include <cstring>
#include "RenderChimp.h"
#include "Resource.h"
#include "ResourceList.h"

/*---------------------------------------------------------------------------*/

ResourceList::ResourceList()
{
	length = 997;
	list = (Resource **) malloc(length * sizeof(Resource *));
	memset(list, 0, length * sizeof(Resource *));

	n_entries = 0;
}

/*---------------------------------------------------------------------------*/

ResourceList::ResourceList(
		u32			list_length
	)
{
	length = list_length;
	list = (Resource **) malloc(length * sizeof(Resource *));
	memset(list, 0, length * sizeof(Resource *));
}

/*---------------------------------------------------------------------------*/

ResourceList::~ResourceList()
{
	empty();
	free(list);
}

/*---------------------------------------------------------------------------*/

void ResourceList::empty()
{
	Resource *ref;
	u32 i;

	for (i = 0; i < length; i++) {
		ref = list[i];

		while (ref) {
			Resource* oldRef = ref;
			ref = ref->next;
			n_entries--;
			delete oldRef;
		}

		list[i] = 0;
	}
	
	ASSERT(n_entries == 0);
}

/*---------------------------------------------------------------------------*/

void ResourceList::add(
		Resource		*obj
	)
{
	u32 h = obj->hash % length;
	Resource *ref = list[h];

	if (!ref) {

		list[h] = obj;

	} else {

		obj->next = list[h];
		list[h] = obj;

	}

	obj->n_refs = 1;

	n_entries++;
}

/*---------------------------------------------------------------------------*/

bool ResourceList::dereference(
		Resource		*obj
	)
{
	if (obj->n_refs > 0)
		obj->n_refs--;

	if (obj->n_refs == 0 && obj->purgeLevel == 0) {

		u32 h = obj->hash % length;
		Resource *ref = list[h];
		Resource *prev = 0;
		
		while (ref) {

			if (ref->isEqual(obj)) {

				if (prev) {
					prev->next = ref->next;
				} else {
					list[h] = ref->next;
				}

				n_entries--;

				delete obj;

				return true;
			}

			prev = ref;
			ref = ref->next;

		}

		REPORT_WARNING("The resource %s was not found in the resource list", obj->getIdentifier());

	}

	return false;
}

/*---------------------------------------------------------------------------*/

void ResourceList::purge(
		u32			max_purge_level,
		bool		force_remove
	)
{
	Resource *ref;
	Resource *prev = 0;
	u32 i;

	for (i = 0; i < length; i++) {
		ref = list[i];
		prev = 0;

		while (ref) {

			if (ref->purgeLevel > 0 && ref->purgeLevel <= max_purge_level && (ref->n_refs == 0 || force_remove)) {
				Resource *oldRef = ref;

				if (prev) {
					prev->next = ref->next;
				} else {
					list[i] = ref->next;
				}

				ref = ref->next;

				n_entries--;

				delete oldRef;

				ensure_heap();

			} else {

				prev = ref;
				ref = ref->next;

			}

		}

	}

}

/*---------------------------------------------------------------------------*/

void ResourceList::extend()
{
	/* TODO: Implement me! */
}

/*---------------------------------------------------------------------------*/

Resource* ResourceList::getResource(
		const char			*id
	)
{
	u32 hash = calculateStringHash(id);
	/* Check hash table for resource */
	u32 h = hash % length;

	Resource *c = list[h];

	while (c) {
		if (c->hash == hash && strcmp(c->identifier, id) == 0) {

			/* Object found */
			c->n_refs++;

			return c;
		}

		c = c->next;
	}

	/* The object was not found */

	return 0;
}

/*---------------------------------------------------------------------------*/

u32 ResourceList::getResourceCount() const
{
	return n_entries;
}

/*---------------------------------------------------------------------------*/

void ResourceList::getAllResources(
		Resource			**res_list
	)
{
	u32 i, n = 0;

	ASSERT(res_list);

	for (i = 0; i < length; i++) {
		Resource *c = list[i];

		while (c) {
			res_list[n] = c;
			n++;
			c = c->next;
		}
	}

	ASSERT(n == n_entries);
}

/*---------------------------------------------------------------------------*/

#ifdef DEBUG

u32 reslist_time = 0;

void ResourceList::printContents(
		char				*list_name
	)
{
	u32 i;
	FILE *f = NULL;

#ifdef RC_LOG_TO_FILE
	#define prnt0(a)		fprintf(f, a)
	#define prnt1(a, b)		fprintf(f, a, b)

	f = fopen("reslist.txt", "a");

	if (!f) {
		return;
	}

#else /* RC_LOG_TO_FILE */
	#define prnt0(a)		printf(a)
	#define prnt1(a, b)		printf(a, b)
#endif /* RC_LOG_TO_FILE */

	prnt1("\n\n\n\n - - = = %s list contents = = - - \n", list_name);
	prnt1("  [ N ENTRIES    ][ %u ]\n", n_entries);
	prnt1("  [ HASH SIZE    ][ %u ]\n", length);

	for (i = 0; i < length; i++) {
		Resource *ptr = list[i];

		while (ptr) {
			prnt1("  -- %s --\n", ptr->identifier);
			prnt1("    [ N REFS   ][ %u ]\n", ptr->n_refs);
			prnt1("    [ PURGE    ][ %u ]\n", ptr->purgeLevel);
			prnt1("    [ HASH     ][ %0x ]\n", ptr->hash);

			ptr->print(f);

			prnt0("\n");

			ptr = ptr->next;
		}
	}

#ifdef RC_LOG_TO_FILE

	fclose(f);

#endif /* RC_LOG_TO_FILE */


	reslist_time++;

}

#endif /* DEBUG */

/*---------------------------------------------------------------------------*/

