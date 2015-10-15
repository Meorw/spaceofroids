#include "RenderChimp.h"
#include "AuxData.h"

/*---------------------------------------------------------------------------*/

Auxiliary::Auxiliary(
		u32					default_entries_size
	)
{
	entries = (AuxData_t *) malloc(sizeof(AuxData_t) * default_entries_size);
	n_entries = 0;
	max_entries = default_entries_size;
}

/*---------------------------------------------------------------------------*/

Auxiliary::~Auxiliary()
{
	u32 n;
	u32 i;

	n = n_entries;

	for (i = 0; i < n; i++) {

		if (entries[i].data)
			free(entries[i].data);

		if (entries[i].tag)
			free(entries[i].tag);

	}

	ASSERT(n_entries == 0);

	free(entries);

}

/*---------------------------------------------------------------------------*/

i32 Auxiliary::findEntry(
		const char			*tag
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].tag, tag) == 0) {
			return (i32) i;
		}
	}

	return -1;
}

/*---------------------------------------------------------------------------*/

void Auxiliary::ensureStorage()
{
	if (n_entries > max_entries) {
		max_entries = smax2<u32>(max_entries + 1, max_entries + (max_entries >> 1)); /* Grow by 50% */
		entries = (AuxData_t *) realloc(entries, sizeof(AuxData_t) * max_entries);
	}
}

/*---------------------------------------------------------------------------*/

void Auxiliary::setData(
		char				*tag,
		void				*data,
		u32					size
	)
{
	i32 idx;
	
	idx = findEntry(tag);

	if (idx == -1) {
		idx = n_entries;
		n_entries++;
		ensureStorage();
		entries[idx].data = malloc(size);
		entries[idx].tag = duplicateString(tag);
	} else if (size != entries[idx].size) {
		entries[idx].data = realloc(entries[idx].data, size);
	}

	memcpy(entries[idx].data, data, size);
	entries[idx].size = size;
}

/*---------------------------------------------------------------------------*/

void Auxiliary::setString(
		char				*tag,
		char				*str
	)
{
	setData(tag, str, strlen(str)+1);
}

/*---------------------------------------------------------------------------*/

void Auxiliary::remove(
		char				*tag
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].tag, tag) == 0) {

			if (entries[i].data)
				free(entries[i].data);

			if (entries[i].tag)
				free(entries[i].tag);

			entries[i].size = 0;

			return;
		}
	}

	REPORT_WARNING("No auxiliary data tag \"%s\" found", tag);
}

/*---------------------------------------------------------------------------*/

void *Auxiliary::getData(
		char				*tag
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].tag, tag) == 0)
			return entries[i].data;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/

void *Auxiliary::getData(
		char				*tag,
		i32					*size
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		if (strcmp(entries[i].tag, tag) == 0) {
			*size = entries[i].size;
			return entries[i].data;
		}
	}

	*size = 0;
	return 0;
}

/*---------------------------------------------------------------------------*/

char *Auxiliary::getString(
		char				*tag
	)
{
	return (char *) getData(tag);
}

/*---------------------------------------------------------------------------*/

void Auxiliary::copyTo(
		Auxiliary			*aux
	)
{
	u32 i;

	for (i = 0; i < n_entries; i++) {
		aux->setData(entries[i].tag, entries[i].data, entries[i].size);
	}
}

/*---------------------------------------------------------------------------*/
