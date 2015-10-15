
#ifndef __RC_SETTINGS_H__
#define __RC_SETTINGS_H__

/* Get rid of dirty Mirco$oft warnings */
#ifdef WIN32
	#pragma warning( disable : 4996 )
	#pragma warning( disable : 4201 )
	#pragma warning( disable : 4100 )
#endif

/*================================================[ Target architecture ]====*/

/* 
 * Target platform
 * 
 * Only uncomment one of the possible platforms listed below.
 * RC_WIN32		- 32-bit Windows build flag
 * RC_MACOSX	- 32-bit MacOS X build flag
 * RC_IPHONE	- iPhone build flag
 */
#define RC_WIN32
/* #define RC_MACOSX */
/* #define RC_IPHONE */

/*
 * This determines the OpenGL version target. RC can only be built for one
 * OpenGL target at a time, so only uncomment one of the defines listed below.
 *
 */
#define RC_OPENGL_ES20
/* #define RC_OPENGL_20 */


#define RC_SSE_VERSION							0

/*============================================[ Debugging and profiling ]====*/

/* Memory debug
 * 
 * Enable definition to debug memory.
 * This will enable tracking and validation of all memory allocated on the
 * heap (at enormous expense of performance). Not fool proof, but pretty good.
 */
/* #define MEMORY_DEBUG */

/* Assert
 * 
 * Enable definition to run ASSERT-statements.
 * Disable this for final release for a small boost in performance. Your code
 * must be correct if this is disabled, or errors and warnings will produce
 * much nastier crashes if they are not caught.
 */
#define RC_ENABLE_ASSERT

/* Log to file
 *
 * If this definition is enabled warnings, errors and memory debug info will
 * be written to log.txt and mem.txt respectively. If this is disabled only
 * standard output will be used to log this information.
 */
#define RC_LOG_TO_FILE



/*=========================================================[ Tweakables ]====*/

#define RC_MAX_ATTRIBS							16

/*==============================================================[ Paths ]====*/

#define RC_TEXTURE_PATH							"../res/textures/"
#define RC_MODEL_PATH							"../res/models/"
#define RC_INFO_PATH							"../res/info/"

/*=========================================================[ Tweakables ]====*/

/*====================================================[ DO NOT TOUCH!!! ]====*/

/*
 * The following code sets up additional defines and makes sure that the
 * selected combination of settings is valid. DO NOT MODIFY THIS CODE (unless
 * you're absolutely sure what you're doing...)
 *
 */

/* TODO: Complete */

#ifdef RC_OPENGL_ES20
	#define RC_OPENGL

	#ifdef RC_WIN32
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
	#endif /* RC_WIN32 */
#elif defined RC_OPENGL_20
	#define RC_OPENGL

#endif

#if defined RC_MAC || defined RC_IPHONE
	#define RC_APPLE
#endif

#if defined RC_IPHONE && defined RC_LOG_TO_FILE
	#error iPhone build doesn't allow logging to file. Either disable \
	RC_LOG_TO_FILE or build for another architecture.
#endif


/*============================================================[ Include ]====*/

#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "Types.h"

#include "Memory.h"

#include "VectorMath.h"
#include "Quaternions.h"
#include "Result.h"
#include "Misc.h"

#include "SceneGraph.h"

/*===========================================================================*/

#endif /* __RC_SETTINGS_H__ */

/*===========================================================================*/

