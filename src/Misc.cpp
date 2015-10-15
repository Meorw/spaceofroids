#include <cstring>
#include "RenderChimp.h"
#include "Misc.h"

/*---------------------------------------------------------------------------*/

u32 nextPow2(
		u32			i
	)
{
   i = i - 1;
   i = i | (i >> 1);
   i = i | (i >> 2);
   i = i | (i >> 4);
   i = i | (i >> 8);
   i = i | (i >> 16);

   return i + 1;
}

/*---------------------------------------------------------------------------*/

u32 calculateStringHash(
		const char		*str
	)
{
	u32 len = strlen(str);
	u32 i;

	u32 hash = 0;

	for (i = 0; i < len; i++) {
		hash *= 33;
		hash += (u32) str[i];
	}

	return hash;
}

/*---------------------------------------------------------------------------*/

#ifdef RC_MEMORY_DEBUG

char *duplicateStringDebug(
		const char		*str,
		const char		*file,
		const char		*func,
		i32				line
	)
{
	u32 len = strlen(str);
	char *s = (char *) MemoryAllocateDebug((sizeof(char) * (len + 1)), file, func, line);
	s[len] = '\0';

	strncpy(s, str, len);

	return s;
}

#else /* RC_MEMORY_DEBUG */

char *duplicateString(
		const char		*str
	)
{
	u32 len = strlen(str);
	char *s = (char *) malloc(sizeof(char) * (len + 1));
	s[len] = '\0';

	strncpy(s, str, len);

	return s;
}

#endif /* RC_MEMORY_DEBUG */

/*---------------------------------------------------------------------------*/

#ifdef RC_MEMORY_DEBUG

char *duplicateStringNDebug(
		const char		*str,
		u32				n,
		const char		*file,
		const char		*func,
		i32				line
	)
{
	char *s = (char *) MemoryAllocateDebug((sizeof(char) * (n + 1)), file, func, line);
	s[n] = '\0';

	strncpy(s, str, n);

	return s;
}

#else /* RC_MEMORY_DEBUG */

char *duplicateStringN(
		const char		*str,
		u32				n
	)
{
	char *s = (char *) malloc(sizeof(char) * (n + 1));
	s[n] = '\0';

	strncpy(s, str, n);

	return s;
}

#endif /* RC_MEMORY_DEBUG */

/*---------------------------------------------------------------------------*/

bool parseFloat(
		f32			*num,
		char		*str
	)
{
	i32 number = 0;
	i32 fraction = 0;
	u32 frac_part = 0;
	
	bool minus = false;
	bool found_number = false;


	if (str[0] == '-') {
		minus = true;
		str++;
	}

	while (*str && ((*str >= '0' && *str <= '9') || *str == '.')) {

		if (*str == '.') {

			if (frac_part > 0)
				return false;

			frac_part = 1;

		} else {

			found_number = true;

			if (frac_part == 0) {
				number = (number * 10) + (*str - '0');
			} else {
				fraction = (fraction * 10) + (*str - '0');
				frac_part *= 10;
			}

		}

		str++;
	}

	if (!found_number) {
		return false;
	}

	if (frac_part) {
		*num = ((f32) number) + (((f32) fraction) / ((float) frac_part));
	} else {
		*num = (f32) number;
	}

	*num = minus ? -(*num) : *num;

	return true;
}

/*---------------------------------------------------------------------------*/

bool parseInt(
		i32			*num,
		char		*str
	)
{
	i32 number = 0;

	bool minus = false;
	bool found_number = false;

	if (str[0] == '-') {
		minus = true;
		str++;
	}

	while (*str && *str >= '0' && *str <= '9') {

		found_number = true;

		number = (number * 10) + (*str - '0');

		str++;
	}

	if (!found_number) {
		return false;
	}

	*num = minus ? -number : number;

	return true;
}

/*---------------------------------------------------------------------------*/

bool parseFloats(
		u32			n,
		f32			*num,
		char		**str
	)
{
	u32 i;

	for (i = 0; i < n; i++) {

		if (!parseFloat(&num[i], str[i]))
			return false;

	}

	return true;
}

/*---------------------------------------------------------------------------*/

bool parseInts(
		u32			n,
		i32			*num,
		char		**str
	)
{
	u32 i;

	for (i = 0; i < n; i++) {

		if (!parseInt(&num[i], str[i]))
			return false;

	}

	return true;
}

/*---------------------------------------------------------------------------*/

