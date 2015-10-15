/*
 * The following code sets up additional defines, includes necessary headers
 * and makes sure that the selected combination of settings is valid. DO NOT
 * MODIFY THIS CODE (unless you're absolutely sure what you're doing...)
 *
 */

#ifndef RC_RESOLVE_H
#define RC_RESOLVE_H

/*====================================================[ DO NOT TOUCH!!! ]====*/

/* TODO: Complete */


/* Platform */
#if defined RC_MACOSX || defined RC_IPHONE
	#define RC_APPLE
#endif

#if defined RC_IPHONE && defined RC_LOG_TO_FILE
	#warning iPhone build doesn't allow logging to file. RC_LOG_TO_FILE is \
now disabled
	#undef RC_LOG_TO_FILE
#endif


#ifdef RC_WIN32
	#include <windows.h>
#endif

/* Get rid of dirty Micro$oft warnings */
#ifdef RC_WIN32
	#pragma warning( disable : 4996 )
	#pragma warning( disable : 4201 )
	#pragma warning( disable : 4100 )
	#pragma warning( disable : 4706 ) /* Assignmen withing conditional expr. */

	#ifdef RC_GLUT
		#pragma warning( disable : 4505 ) /* Unreferenced local function has been removed. */
	#endif

#endif

/* Renderer */
#if defined RC_OPENGL_ES_20 && defined RC_OPENGL_20
	#error Build must only be enabled for one OpenGL version only.
#endif

#if defined RC_CUDA_TRACER && defined RC_OPENGL_ES_20
	#error The Cuda tracer relies on OpenGL 2.0 for rendering, and may \
therefore only coexist with that version of OpenGL.
#endif

#ifdef RC_OPENGL_ES_20
	#define RC_OPENGL
	#define RC_OPENGL_ES

	#ifdef RC_WIN32
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
	#elif defined RC_IPHONE
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
	#endif /* RC_WIN32 / RC_IPHONE */

#elif defined RC_OPENGL_20

	#define RC_OPENGL

	#ifdef RC_WIN32
		#include "GLee.h"
		#include <GL/gl.h>
		#include <GL/glu.h>

		#if defined RC_SDL
			#include <SDL/SDL.h>
			#include <SDL/SDL_syswm.h>
		#elif defined RC_GLUT
			//#include <GL/glut.h>
		#endif /* RC_SDL / RC_GLUT */
		
		#include <SDL/SDL_image.h>

	#endif /* RC_WIN32 */

	#ifdef RC_MACOSX
		#include "GLee.h"
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#endif /* RC_MACOSX */
#endif /* RC_OPENGL_ES_20 / RC_OPENGL_20 */

#ifdef RC_CUDA_TRACER

	#ifdef RC_WIN32
		#include "GLee.h"
		#include <GL/gl.h>
		#include <GL/glu.h>

	#endif /* RC_WIN32 */

#endif



typedef enum {

	RENDERER_UNDEFINED = 0,

#ifdef RC_OPENGL_ES_20
	RENDERER_OPENGL_ES_20,
#elif defined RC_OPENGL_20
	RENDERER_OPENGL_20,
#endif /* OpenGL */

#ifdef RC_CUDA_TRACER
	RENDERER_CUDA_TRACER,
#endif

	RENDERER_LAST

} RendererID;




/* Standard libraries */
#ifdef RC_WIN32
	//#include <ctype>
	#include <vector>
	#include <memory>
	#include <cstdio>
	#include <cstdlib>
	#include <cstring>
	#include <cstdarg>
#elif defined RC_MACOSX
//	#include <vector.h>
//	#include <memory.h>
	#include "ctype.h"
	#include "stdio.h"
	#include "stdlib.h"
	#include "string.h"
	#include "stdarg.h"
#endif

#ifdef RC_APPLE

	#ifdef min
		#undef min
	#endif

	#ifdef max
		#undef max
	#endif

	#define min(a, b)		((a) < (b) ? (a) : (b))
	#define max(a, b)		((a) > (b) ? (a) : (b))
#endif

/*---------------------------------------------------------[ Tweakables ]----*/

#define RC_SCENE_GRAPH_MAX_DEPTH				64
#define RC_MAX_RENDER_TARGET_TEXTURES			8
#define RC_GEOMETRY_MAX_PASSES					8

/*===========================================================================*/





/* Basic typedefs */
#include "Types.h"

/* Memory hijacker */
#include "Memory.h"

/* Error and warning system */
#include "Result.h"

/* Vector and matrix math */
#include "VectorMath.h"

/* Misc. helper functions */
#include "Misc.h"


/* Platform include */
#include "Keys.h"
#include "Platform.h"

/* TODO: Fix! */
#include "SceneGraphAux.h"
#include "SceneGraph.h"


#endif /* RC_RESOLVE_H */

