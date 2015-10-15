/*
 * Misc. useful functions
 *
 */

#ifndef RC_MISC_H
#define RC_MISC_H

/** Pad up to closest 4 byte value */
#define	PAD4(a)				((((a) - 1) & (~0x3)) + 0x4)

/** Returns the next power of 2 for a given value */
u32 nextPow2(
		u32			i
	);

/** Calculates a hash value from a string */
u32 calculateStringHash(
		const char		*str
	);

#ifdef RC_MEMORY_DEBUG

	#define duplicateString(str)		duplicateStringDebug((str), __FILE__, __FUNCTION__, __LINE__)

	#define duplicateStringN(str, n)	duplicateStringNDebug((str), (n), __FILE__, __FUNCTION__, __LINE__)

	/** Allocates new memory and occupies it with the string */
	char *duplicateStringDebug(
			const char		*str,
			const char		*file,
			const char		*func,
			i32				line
		);

	/**
	 * Allocates new memory and occupies it with N chars of the incoming
	 * string
	 */
	char *duplicateStringNDebug(
			const char		*str,
			u32				n,
			const char		*file,
			const char		*func,
			i32				line
		);

#else /* RC_MEMORY_DEBUG */

	/** Allocates new memory and occupies it with the string */
	char *duplicateString(
			const char		*str
		);

	/**
	 * Allocates new memory and occupies it with N chars of the incoming
	 * string
	 */
	char *duplicateStringN(
			const char		*str,
			u32				n
		);

#endif /* RC_MEMORY_DEBUG */

/** Parse a float value from a string */
bool parseFloat(
		f32			*num,
		char		*str
	);

/** Parse an int value from a string */
bool parseInt(
		i32			*num,
		char		*str
	);

/** Parse n floats from n strings */
bool parseFloats(
		u32			n,
		f32			*num,
		char		**str
	);

/** Parse n ints from n strings */
bool parseInts(
		u32			n,
		i32			*num,
		char		**str
	);

#endif /* RC_MISC_H */


