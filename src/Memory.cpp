#include "RenderChimp.h"
#include "Command.h"
#include "Memory.h"


#ifdef RC_WIN32

#ifdef RC_WIN32
	#pragma warning( disable : 4996 )
#endif

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef new
#undef delete

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static i32 peak_mem = 0;
static i32 local_peak_mem = 0;
static i32 total_mem = 0;
static i32 total_blocks = 0;
static i32 n_allocations = 0;
static i32 n_deallocations = 0;
static i32 n_reallocations = 0;

#ifndef RC_MEMORY_DEBUG
	
/*---------------------------------------------------------------------------*/

void *MemoryAllocate(
		const u32	mem_size
	)
{
	u32 *p;
	void *ptr;

	ptr = malloc(mem_size + 4);

	total_mem += mem_size;
	total_blocks++;

	peak_mem = total_mem > peak_mem ? total_mem : peak_mem;
	local_peak_mem = total_mem > local_peak_mem ? total_mem : local_peak_mem;

	if (!ptr)
		REPORT_ERROR("Out of memory");

	p = (u32 *) ptr;

	p[0] = mem_size;

	n_allocations++;

	return &p[1];
}

/*---------------------------------------------------------------------------*/

void *MemoryAllocateSetZero(
		const u32	n,
		const u32	type_size
	)
{
	u32 s = n * type_size;

	void *ptr = MemoryAllocate(s);

	memset(ptr, 0, s);

	return ptr;
}


/*---------------------------------------------------------------------------*/

void MemoryFree(
		void		*ptr
	)
{
	u32 *p;

	p = (u32 *) ptr;

	p--;

	if (!ptr || !p) {
		REPORT_WARNING("Cannot free null pointer");
		return;
	}

	total_mem -= p[0];
	total_blocks--;

	n_deallocations++;

	free(p);
}

/*---------------------------------------------------------------------------*/

void *MemoryReallocate(
		void		*old_ptr,
		const u32	new_size
	)
{
	u32 *p;
	void *new_ptr;

	p = (u32 *) old_ptr;

	p--;

	total_mem += new_size;
	total_mem -= p[0];

	peak_mem = total_mem > peak_mem ? total_mem : peak_mem;
	local_peak_mem = total_mem > local_peak_mem ? total_mem : local_peak_mem;

	new_ptr = realloc(p, new_size + 4);

	if (!new_ptr)
		REPORT_ERROR("Out of memory");

	p = (u32 *) new_ptr;

	p[0] = new_size;

	n_reallocations++;

	return &p[1];
}

/*---------------------------------------------------------------------------*/

void *operator new(
		size_t		mem_size
	)
{
	void *p;

	p = MemoryAllocate(mem_size);

	return p;
}

/*---------------------------------------------------------------------------*/

void *operator new [](
		size_t		mem_size
	)
{
	void *p;

	p = MemoryAllocate(mem_size);

	return p;
}

/*---------------------------------------------------------------------------*/

void operator delete [](
		void *ptr
	)
{
	MemoryFree(ptr);
}

/*---------------------------------------------------------------------------*/

void operator delete(
		void		*ptr
	)
{
	MemoryFree(ptr);
}

/*---------------------------------------------------------------------------*/

void logAllocations()
{
	FILE *f = fopen("mem.txt", "w");

	if (!f) {
		printf(" - - = = Allocated memory pointers = = - - \n");
		printf("  [ TOTAL MEM    ][%u]\n", total_mem);
		printf("  [ PEAK MEM     ][%u]\n", peak_mem);
		printf("  [ TOTAL BLOCKS ][%u]\n\n\n", total_blocks);
	} else {
		fprintf(f, " - - = = Allocated memory pointers = = - - \n");
		fprintf(f, "  [ TOTAL MEM    ][%u]\n", total_mem);
		fprintf(f, "  [ PEAK MEM     ][%u]\n", peak_mem);
		fprintf(f, "  [ TOTAL BLOCKS ][%u]\n\n\n", total_blocks);
		fclose(f);
	}
}

/*---------------------------------------------------------------------------*/

void ensure_heap()
{
}

#else /* !MEMORY_DEBUG */

//#define __FUNCTION__	"some function"

static char *mem_no_file = "Unknown";
static char *mem_no_func = "Unknown";

char *mem_info_file = mem_no_file;
char *mem_info_func = mem_no_func;
i32 mem_info_line = -1;

#define MAGIC_PREFIX		0xAC1DBA5E
#define MAGIC_FREED			0xFD
#define MAGIC_ALLOCATED		0xAD
//#define MAGIC_POSTFIX		0xBABECAFE

static const char magic_postfix[4] = { 'a', 'p', 'a', 'n' };


typedef struct loc_t loc_t;

static u32 mem_time = 0;

struct loc_t {
	loc_t		*next;
	i32			size;
	i32			n_blocks;
	char		*file;
	char		*func;
	i32			line;
};


typedef struct ptr_t ptr_t;

struct ptr_t {
	ptr_t		*next;
	void		*ptr;
	loc_t		*loc;
	i32			size;
	u32			time;
};

#define HASH_SIZE				97
#define LOC_HASH_SIZE			97
#define FREE_HASH_SIZE			97

ptr_t *ptr_hash[HASH_SIZE] = { 0 };
loc_t *loc_hash[LOC_HASH_SIZE] = { 0 };
ptr_t *free_hash[FREE_HASH_SIZE] = { 0 };

/*---------------------------------------------------------------------------*/

static void ensure_pointer_validity(
		void		*ptr
	)
{
	u32 *pi;
	char *pc;

	pi = (u32 *) ptr;

	pi -= 2;

	pc = ((char *) ptr) + pi[1];

	if (pi[0] != MAGIC_PREFIX)
		REPORT_ERROR("Heap is corrupted");

	if (strncmp(pc, magic_postfix, 4) != 0)
		REPORT_ERROR("Heap is corrupted");

}

/*---------------------------------------------------------------------------*/

static void ensure_freed_pointer_validity(
		void		*ptr
	)
{
	u32 i;
	u32 *pi;
	char *pc;

	pi = (u32 *) ptr;

	pi -= 2;

	pc = ((char *) ptr) + pi[1];

	pc = (char *) ptr;

	for (i = 0; i < pi[1]; i++) {
		
		if (((unsigned char) *pc) != MAGIC_FREED)
			REPORT_ERROR("Heap is corrupted");

		pc++;
	}


	if (pi[0] != MAGIC_PREFIX)
		REPORT_ERROR("Heap is corrupted");

	if (strncmp(pc, magic_postfix, 4) != 0)
		REPORT_ERROR("Heap is corrupted");


}

/*---------------------------------------------------------------------------*/

void ensure_heap()
{
	/* Check all allocations and make sure that they are all valid */

	i32 i;

#if 0
	for (i = 0; i < FREE_HASH_SIZE; i++) {
		ptr_t *p = free_hash[i];

		while (p) {
			void *ptr = p->ptr;

			ensure_freed_pointer_validity(ptr);

			p = p->next;
		}
	}
#endif

	for (i = 0; i < HASH_SIZE; i++) {
		ptr_t *p = ptr_hash[i];

		while (p) {
			void *ptr = p->ptr;

			ensure_pointer_validity(ptr);

			p = p->next;
		}
	}
}

/*---------------------------------------------------------------------------*/

static u32 loc_hash_value(
		const char	*file_name,
		const i32	line
	)
{
	u32 u = (u32) line;
	return (u * 13) % LOC_HASH_SIZE;
}

/*---------------------------------------------------------------------------*/

static u32 ptr_hash_value(
		const void *ptr
	)
{
	u32 i = *((u32 *) &ptr);
	return ((i >> 3) * 13) % HASH_SIZE;
}


/*---------------------------------------------------------------------------*/

static void free_hash_add(
		void		*p,
		const u32	mem_size
	)
{
	u32 hash;
	ptr_t *ptr;

	/* Update freed pointer table */
	hash = ptr_hash_value(p);

	ptr = free_hash[hash];

	ptr = (ptr_t *) malloc(sizeof(ptr_t));

	if (!ptr)
		REPORT_ERROR("Out of memory");

	ptr->ptr = p;
	ptr->loc = 0;
	ptr->size = (i32) mem_size;
	ptr->next = free_hash[hash];
	ptr->time = 0;

	free_hash[hash] = ptr;

}

/*---------------------------------------------------------------------------*/

static void hash_add(
		void		*p,
		const u32	mem_size,
		const char	*file,
		const char	*func,
		const i32	line
	)
{
	u32 hash;
	loc_t *loc;
	ptr_t *ptr;

	/* Update location table */
	hash = loc_hash_value(func, line);

	loc = loc_hash[hash];

	while (loc) {
		if (line == loc->line && strcmp(file, loc->file) == 0)
			break;

		loc = loc->next;
	}

	if (!loc) {
		loc = (loc_t *) malloc(sizeof(loc_t));

		if (!loc)
			REPORT_ERROR("Out of memory");

		memset(loc, 0, sizeof(loc_t));

		loc->file = strdup(file);
		loc->func = strdup(func);
		loc->line = line;

		loc->next = loc_hash[hash];
		loc_hash[hash] = loc;
	}

	loc->size += mem_size;
	loc->n_blocks++;

	/* Update pointer table */
	hash = ptr_hash_value(p);

	ptr = (ptr_t *) malloc(sizeof(ptr_t));

	if (!ptr)
		REPORT_ERROR("Out of memory");

	ptr->ptr = p;
	ptr->loc = loc;
	ptr->size = (i32) mem_size;
	ptr->next = ptr_hash[hash];
	ptr->time = mem_time++;
	ptr_hash[hash] = ptr;

}

/*---------------------------------------------------------------------------*/

static void hash_remove(
		const void	*ptr
	)
{
	u32 hash;
	loc_t *loc;
	ptr_t *ph;
	ptr_t *pph = 0;

	/* ensure_pointer_validity(ptr); */

	/* Remove from pointer table */

	hash = ptr_hash_value(ptr);

	ph = ptr_hash[hash];

	while (ph) {
		if (ph->ptr == ptr)
			break;

		pph = ph;
		ph = ph->next;
	}

	if (!ph)
		REPORT_ERROR("Heap is corrupted");

	loc = ph->loc;

	loc->n_blocks--;
	loc->size -= ph->size;

	if (loc->n_blocks < 0 || loc->size < 0)
		REPORT_ERROR("Heap is corrupted");

	if (!pph) {
		ptr_hash[hash] = ph->next;
	} else {
		pph->next = ph->next;
	}
	
	free(ph);
	ph = 0;

	if (loc->n_blocks == 0) {
		/* Remove from location table */

		loc_t *ploc = 0;
		loc_t *cloc;

		if (loc->size != 0)
			REPORT_ERROR("Heap is corrupted");

		hash = loc_hash_value(loc->file, loc->line);

		cloc = loc_hash[hash];

		while (cloc != loc && cloc) {
			ploc = cloc;
			cloc = cloc->next;
		}

		if (!cloc)
			REPORT_ERROR("Heap is corrupted");

		if (!ploc) {
			loc_hash[hash] = loc->next;
		} else {
			ploc->next = loc->next;
		}

		free(loc->file);
		free(loc->func);
		free(loc);
		loc = 0;

	}

}

/*---------------------------------------------------------------------------*/

void *MemoryAllocateDebug(
		const u32	mem_size,
		const char	*file,
		const char	*func,
		const int	line
	)
{
	void *ptr;
	char *pc;
	u32 *pi;

	ptr = malloc(mem_size + 12);

	if (!ptr)
		REPORT_ERROR("Out of memory");

	memset(ptr, MAGIC_ALLOCATED, mem_size + 12);

	pi = (u32 *) ptr;
	pc = ((char *) ptr) + mem_size + 8;

	pi[0] = MAGIC_PREFIX;
	pi[1] = mem_size;
	memcpy(pc, magic_postfix, 4);

	total_mem += mem_size;
	total_blocks++;

	peak_mem = total_mem > peak_mem ? total_mem : peak_mem;
	local_peak_mem = total_mem > local_peak_mem ? total_mem : local_peak_mem;

	hash_add(&pi[2], mem_size, file, func, line);

	ensure_heap();

	n_allocations++;

	return &pi[2];
}

/*---------------------------------------------------------------------------*/

void *MemoryAllocateSetZeroDebug(
		const u32	n,
		const u32	type_size,
		const char	*file,
		const char	*func,
		const int	line
	)
{
	u32 s = n * type_size;

	void *ptr = MemoryAllocateDebug(s, file, func, line);

	memset(ptr, 0, s);

	return ptr;
}

/*---------------------------------------------------------------------------*/

void MemoryFreeDebug(
		void		*ptr
	)
{
	u32 mem_size;
	u32 *p;

	ensure_pointer_validity(ptr);

	p = (u32 *) ptr;

	p -= 2;

	mem_size = p[1];

	total_mem -= mem_size;
	total_blocks--;

	hash_remove(ptr);

	memset(ptr, MAGIC_FREED, mem_size);

	free_hash_add(ptr, mem_size);

	ensure_heap();

	n_deallocations++;
}

/*---------------------------------------------------------------------------*/

void *MemoryReallocateDebug(
		void		*old_ptr,
		const u32	new_size,
		const char	*file,
		const char	*func,
		const int	line
	)
{
	u32 old_size;
	void *new_ptr;
	u32 *pi;
	char *pc;

	ensure_pointer_validity(old_ptr);

	pi = (u32 *) old_ptr;

	pi -= 2;

	old_size = pi[1];

	hash_remove(old_ptr);

	new_ptr = realloc(pi, new_size + 12);

	if (!new_ptr)
		REPORT_ERROR("Out of memory");

	pi = (u32 *) new_ptr;
	pc = ((char *) new_ptr) + new_size + 8;

	pi[0] = MAGIC_PREFIX;
	pi[1] = new_size;
	memcpy(pc, magic_postfix, 4);

	total_mem += new_size;
	total_mem -= old_size;

	peak_mem = total_mem > peak_mem ? total_mem : peak_mem;
	local_peak_mem = total_mem > local_peak_mem ? total_mem : local_peak_mem;

	if (new_size > old_size)
		memset(((char *) &pi[2]) + old_size, MAGIC_ALLOCATED, new_size - old_size);

	hash_add(&pi[2], new_size, file, func, line);

	ensure_heap();

	n_reallocations++;

	return &pi[2];
}

/*---------------------------------------------------------------------------*/

void *operator new(
		size_t		mem_size
	)
{
	void *p;

	ensure_heap();

	p = MemoryAllocateDebug(mem_size,
		mem_info_file, mem_info_func, mem_info_line);

	if (mem_info_line != -1) {
		free(mem_info_file);
		free(mem_info_func);
		mem_info_line = -1;

		mem_info_file = mem_no_file;
		mem_info_func = mem_no_func;
	} else {
		printf("Suspicious mem activity... (new)\n");
	}

	return p;
}

/*---------------------------------------------------------------------------*/

void *operator new [](
		size_t		mem_size
	)
{
	void *p;

	p = MemoryAllocateDebug(mem_size,
		mem_info_file, mem_info_func, mem_info_line);

	if (mem_info_line != -1) {
		free(mem_info_file);
		free(mem_info_func);
		mem_info_line = -1;

		mem_info_file = mem_no_file;
		mem_info_func = mem_no_func;
	} else {
		/* TODO: I don't know how to overload new[]-operator and capture
		 * the line, funtion and file at the same time. If you do, e-mail me!
		 */
		/* printf("Suspicious mem activity... (new[])\n"); */
	}

	return p;
}

/*---------------------------------------------------------------------------*/

void operator delete [](
		void *ptr
	)
{
	MemoryFreeDebug(ptr);
}

/*---------------------------------------------------------------------------*/

void operator delete(
		void		*ptr
	)
{
	MemoryFreeDebug(ptr);
}

/*---------------------------------------------------------------------------*/

i32 MemorySetInfo(
		const char		*new_file,
		const char		*new_func,
		i32				new_line
	)
{
	mem_info_file = strdup(new_file);
	mem_info_func = strdup(new_func);
	mem_info_line = new_line;

	return 0;
}

/*---------------------------------------------------------------------------*/


void logAllocations()
{
	u32 i;

	FILE *f;
	
#ifdef RC_LOG_TO_FILE
	#define prnt0(a)		fprintf(f, a)
	#define prnt1(a, b)		fprintf(f, a, b)

	f = fopen("mem.txt", "w");

	if (!f) {
		REPORT_WARNING("Couldn't open memory log file");
		return;
	}

#else /* RC_LOG_TO_FILE */
	#define prnt0(a)		printf(a)
	#define prnt1(a, b)		printf(a, b)
#endif /* RC_LOG_TO_FILE */

	prnt0("\n\n\n\n - - = = Allocated memory pointers = = - - \n");
	prnt1("  [ TOTAL MEM    ][ %u ]\n", total_mem);
	prnt1("  [ PEAK MEM     ][ %u ]\n", peak_mem);
	prnt1("  [ TOTAL BLOCKS ][ %u ]\n\n\n", total_blocks);

	for (i = 0; i < HASH_SIZE; i++) {
		ptr_t *ptr = ptr_hash[i];

		while (ptr) {
			prnt1("  -- %8x --\n", (u32) ptr);
			prnt1("    [ FILE     ][ %s ]\n", (u32) ptr->loc->file);
			prnt1("    [ FUNCTION ][ %s ]\n", (u32) ptr->loc->func);
			prnt1("    [ LINE     ][ %d ]\n", (u32) ptr->loc->line);
			prnt1("    [ MEM      ][ %d ]\n", (u32) ptr->size);
			prnt1("    [ TIME     ][ %d ]\n\n", (u32) ptr->time);
			ptr = ptr->next;
		}
	}

	prnt0(" - - = = Locations = = - - \n\n\n");

	for (i = 0; i < LOC_HASH_SIZE; i++) {
		loc_t *loc = loc_hash[i];

		while (loc) {
			if (loc->size != 0) {
				prnt1("  -- %8x --\n", (u32) loc);
				prnt1("    [ FILE     ][ %s ]\n", (u32) loc->file);
				prnt1("    [ FUNCTION ][ %s ]\n", (u32) loc->func);
				prnt1("    [ LINE     ][ %d ]\n", (u32) loc->line);
				prnt1("    [ MEM      ][ %d ]\n", (u32) loc->size);
				prnt1("    [ BLOCKS   ][ %d ]\n\n", (u32) loc->n_blocks);
			}
			loc = loc->next;
		}
	}

#ifdef RC_LOG_TO_FILE

	fclose(f);

#endif /* RC_LOG_TO_FILE */

}

#endif /* !MEMORY_DEBUG */

/*---------------------------------------------------------------------------*/

u32 getAllocationSize()
{
	return total_mem;
}

/*---------------------------------------------------------------------------*/

u32 getCurrentAllocationCount()
{
	return total_blocks;
}

/*---------------------------------------------------------------------------*/

u32 getAllocationPeakLocal()
{
	u32 t = local_peak_mem;

	local_peak_mem = total_mem;

	return t;
}

/*---------------------------------------------------------------------------*/

u32 getAllocationPeak()
{
	return peak_mem;
}

/*---------------------------------------------------------------------------*/

u32 getAllocationCount()
{
	return n_allocations;
}

/*---------------------------------------------------------------------------*/

u32 getDeallocationCount()
{
	return n_deallocations;
}

/*---------------------------------------------------------------------------*/

u32 getReallocationCount()
{
	return n_reallocations;
}

/*====================================================[ Console commands ]====*/

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_mem_info = {
	"/mem_info"
};

bool CMD_mem_info(
		u32		n_args,
		char	**args
	)
{
	u32 alloc_size = getAllocationSize();
	u32 alloc_peak = getAllocationPeak();
	u32 n_blocks = getCurrentAllocationCount();

	u32 n_allocs = getAllocationCount();
	u32 n_deallocs = getDeallocationCount();
	u32 n_reallocs = getReallocationCount();

	Console::log("  Memory info:");

	Console::log("Memory usage: %u bytes", alloc_size);

	Console::log("Peak memory usage: %u bytes", alloc_peak);

	Console::log("Memory blocks: %u", n_blocks);

	Console::log("Average allocation size: %.3f bytes", ((f32) alloc_size) / ((f32) n_blocks));

	Console::log("Total allocation calls: %u", n_allocs);
	Console::log("Total re-allocation calls: %u", n_reallocs);
	Console::log("Total free calls: %u", n_deallocs);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_mem_log = {
	"/mem_log"
};

bool CMD_mem_log(
		u32		n_args,
		char	**args
	)
{
	logAllocations();

	return true;
}

/*---------------------------------------------------------------------------*/

void MemoryRegisterCommands()
{
	Command::add("mem_info", CMDHELP_mem_info, CMD_mem_info);
	Command::add("mem_log", CMDHELP_mem_log, CMD_mem_log);
}

/*---------------------------------------------------------------------------*/

#else /* 1 */

u32 getAllocationSize()
{
	return 0;
}

u32 getCurrentAllocationCount()
{
	return 0;
}

u32 getAllocationPeak()
{
	return 0;
}

u32 getAllocationPeakLocal()
{
	return 0;
}

u32 getAllocationCount()
{
	return 0;
}

u32 getReallocationCount()
{
	return 0;
}

u32 getDeallocationCount()
{
	return 0;
}

void logAllocations()
{
}

void ensure_heap()
{
}

void MemoryRegisterCommands()
{
	printf("Memory console commands unavailable\n");
}

#endif /* 1 */





