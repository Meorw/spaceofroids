/* Standard type definitions */

#ifndef RC_TYPES_H
#define RC_TYPES_H

#include "RenderChimp.h"

#ifdef RC_OPENGL

	typedef GLfloat				f32;
	typedef	double				f64;
	typedef GLbyte				i8;
	typedef GLubyte				u8;
	typedef GLshort				i16;
	typedef GLushort			u16;
	typedef GLint				i32;
	typedef GLuint				u32;

#else /* RC_OPENGL */

	typedef float				f32;
	typedef	double				f64;
	typedef signed char			i8;
	typedef unsigned char		u8;
	typedef signed short		i16;
	typedef unsigned short		u16;
	typedef signed int			i32;
	typedef unsigned int		u32;

#endif /* RC_OPENGL */

#ifdef RC_WIN32
	typedef signed __int64		i64;
	typedef unsigned __int64	u64;
#elif defined RC_APPLE
	typedef signed long long	i64;
	typedef unsigned long long	u64;
#endif /* WIN32/MAC */




#endif /* RC_TYPES_H */

