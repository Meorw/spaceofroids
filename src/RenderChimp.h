/*

 Copyright (c) 2009-2010 Magnus Andersson

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/

/*
 *
 * Run "doxygen $RENDERCHIMP$/doc/doxy" to generate HTML documentation.
 *
 */

/*===========================================================================*/

/*
 *           _
 *          c "    -[ RenderChimp ]-
 *           (-
 *
 *
 */

/*===========================================================================*/


/*===========================================================================*/

#ifndef RC_RENDERCHIMP_H
#define RC_RENDERCHIMP_H


/*================================================[ Target architecture ]====*/

/* 
 * Target platform
 * 
 * Only uncomment one of the possible platforms listed below.
 * RC_WIN32		- 32-bit Windows build flag
 * RC_MACOSX	- 32-bit MacOS X build flag (NOT YET SUPPORTED!)
 * RC_IPHONE	- iPhone build flag (ONLY MARGINALLY SUPPORTED!)
 */
/* #define RC_WIN32 */
/* #define RC_MACOSX */
/* #define RC_IPHONE */

/*
 * This determines the OpenGL version target. RC can only be built for one
 * OpenGL target at a time, so only uncomment one of the defines listed below.
 *
 */
//#define RC_OPENGL_ES_20
//#define RC_OPENGL_20

/*
 * Either use GLUT or SDL
 *
 */
#define RC_SDL
//#define RC_GLUT

#define RC_SSE_VERSION							0

/*============================================[ Debugging and profiling ]====*/

/* Memory debug
 * 
 * Enable definition to debug memory.
 * This will enable tracking and validation of all memory allocated on the
 * heap (at enormous expense of performance). Not fool proof, but pretty good.
 */
//#define RC_MEMORY_DEBUG

/* Assert
 * 
 * Enable definition to run ASSERT-statements.
 * Disable this for final release for a small boost in performance. Your code
 * must be correct if this is disabled, or errors and warnings will produce
 * much nastier crashes if they are not caught.
 */
//#define RC_ENABLE_ASSERT

/* Log to file
 *
 * If this definition is enabled warnings, errors and memory debug info will
 * be written to log.txt and mem.txt respectively. If this is disabled only
 * standard output will be used to log this information.
 */
#define RC_LOG_TO_FILE

/* Log frenzy
 *
 * Log everything to everywhere...
 */
#define RC_LOG_FRENZY


/*===========================================================================*/

/* LOWPRIO: Hack! */
#define RC_MAX_ATTRIBS							16

#include "Resolve.h"

/*=========================================================[ Tweakables ]====*/

#ifdef RC_IPAD
	#define RC_DEFAULT_DISPLAY_WIDTH				1024
	#define RC_DEFAULT_DISPLAY_HEIGHT				768
#else
	#define RC_DEFAULT_DISPLAY_WIDTH				1024
	#define RC_DEFAULT_DISPLAY_HEIGHT				768
#endif

/*==============================================================[ Paths ]====*/

#define RC_RESOURCE_PATH						"res/"
#define RC_SHADER_PATH							"shaders/"

/*===========================================================================*/

i32 RCMain();
i32 RCRun();
void RCEnd();

#endif /* RC_RENDERCHIMP_H */

/*===========================================================================*/
