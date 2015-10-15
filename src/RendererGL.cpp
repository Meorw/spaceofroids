#include "RenderChimp.h"

#ifdef RC_OPENGL

#include "BmpWriter.h"
#include "Renderer.h"
#include "RendererGL.h"
#include "SDL.h"

#define CHECK_FOR_ERRORS

ShaderProgram		*Renderer::active_shader_program;

RenderState			*Renderer::active_render_state;

/* Matrices */

/* ViewProjection matrix */
mat4f				Renderer::world;
mat4f				Renderer::worldInverse;

/* View matrix */
mat4f				Renderer::view;
mat4f				Renderer::viewInverse;

/* Projection matrix */
mat4f				Renderer::projection;
mat4f				Renderer::projectionInverse;

/* ViewProjection matrix */
mat4f				Renderer::viewProjection;
mat4f				Renderer::viewProjectionInverse;

i32					Renderer::viewport_offset[2];
u32					Renderer::viewport_size[2];

bool				Renderer::take_screenshot;
char				Renderer::screenshot_filename[FILENAME_MAX - 1];

RendererLimits_t	Renderer::limits;


/* Local stuff */

static u32				curr_tex_slot = 0;
static char				tex_slot[RC_MAX_TEXTURE_SLOTS][RC_MAX_TEXTURE_NAME];

static u32				n_vbos;
static u32				n_ibos;
static u32				n_rbos;
static u32				n_fbos;
static u32				n_textures;
static u32				n_cubemaps;
static u32				n_shaders;
static u32				n_shaderprogs;

//static ShaderProgram	*default_shader_program;

static RenderState		*default_render_state;

static vec4f			frustum[6];



#ifdef RC_WIN32

	#include <windows.h>

#endif

#define RC_RENDERER_ERR_BUF_LEN				8192

/*---------------------------------------------------------------------------*/

#define RC_N_TRANSFORMATION_MATRICES		24

static const char *gl_matrix_transform[RC_N_TRANSFORMATION_MATRICES] = {
	"View",					"ViewInverse",					"ViewTranspose",					"ViewInverseTranspose",
	"Projection",			"ProjectionInverse",			"ProjectionTranspose",				"ProjectionInverseTranspose",
	"ViewProjection",		"ViewProjectionInverse",		"ViewProjectionTranspose",			"ViewProjectionInverseTranspose",
	"World",				"WorldInverse",					"WorldTranspose",					"WorldInverseTranspose",
	"WorldView",			"WorldViewInverse",				"WorldViewTranspose",				"WorldViewInverseTranspose",
	"WorldViewProjection",	"WorldViewProjectionInverse",	"WorldViewProjectionTranspose",		"WorldViewProjectionInverseTranspose"
};

#define			MAT_VIEW				0
#define			MAT_PROJ				4
#define			MAT_VIEW_PROJ			8
#define			MAT_WORLD				12
#define			MAT_WORLD_VIEW			16
#define			MAT_WORLD_VIEW_PROJ		20

#define MATRIX_INDEX(base, i, t)			(((base) << 2) + i + ((t) << 1));

/*---------------------------------------------------------------------------*/

typedef struct {

	GLuint			id;

	u32				refs;

} RBO_t;

/*---------------------------------------------------------------------------*/

typedef struct {

	GLuint			tex_id;

} GLTextureHandle_t;

typedef struct {

	GLuint			tex_id[RC_MAX_RENDER_TARGET_TEXTURES];

	u32				n_targets;

	GLuint			fbo;

	RBO_t			*rbo;

} GLRenderTargetHandle_t;

typedef struct {

	GLuint			ctex_id;

} GLCubeMapHandle_t;

typedef struct {

	GLuint			vbo;
	GLuint			vbo_type;

	GLuint			ibo;
	GLuint			ibo_type;

	bool			has_ibo;

} GLVertexArrayHandle_t;

typedef struct {

	GLuint			sp_id;
	GLuint			vs_id;
	GLuint			fs_id;

	GLint			loc[RC_N_TRANSFORMATION_MATRICES + 1];

#ifdef RC_OPENGL_20
	GLuint			gs_id;
	bool			has_gs;
#endif

} GLShaderProgramHandle_t;


/*---------------------------------------------------------------------------*/

/* State translations to GL */

const u32 gl_src_blend_map[8] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

const u32 gl_dst_blend_map[8] = {
	GL_ZERO,
	GL_ONE,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA_SATURATE,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

const u32 gl_cmp_map[8] = {
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

const u32 gl_cull_map[3] = {
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

const u32 gl_usage_map[3] = {
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW
};



#ifndef RC_IPHONE
#ifdef RC_OPENGL_ES_20
	static EGLDisplay eglDisplay;
	static EGLConfig eglConfig;
	static EGLSurface eglSurface;
	static EGLContext eglContext;
#endif /* RC_OPENGL_ES_20 */
#endif

/*---------------------------------------------------------------------------*/

bool gl_is_broken = false;
i32 error_line = -1;

/* Ensures that GL hasn't generated any errors */
bool check_errors(
		const char			*file,
		const char			*function,
		i32					line
	)
{
	GLenum result;
	result = glGetError();

	if (result) {
		char str[4096];

		if (error_line != line) {
			sprintf(str, "GL error caught in %s, %s on line %d with error code %x", file, function, line, result);

			gl_is_broken = true;
			REPORT_WARNING(str);
			error_line = line;

			printf("%x\n", result);
		}

		return false;

	} else {
		error_line = -1;
	}

	return true;
}

#ifdef CHECK_FOR_ERRORS

	#define GL_CHECK_ERRORS()				check_errors(__FILE__, __FUNCTION__, __LINE__)

#else /* DEBUG */

	#define GL_CHECK_ERRORS()

#endif /* DEBUG */


#ifdef RC_OPENGL_ES_20

	static char *filename_prefix[] = {"gles20/", "gles20/"};
	static char *filename_suffix[] = {".vs", ".fs"};
	static ShaderType_t shader_type[2] = {SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT};
	static bool shader_required[2] = {true, true};

#elif defined RC_OPENGL_20

/*
	static char *filename_suffix[] = {".gl20.vs", ".gl20.gs", ".gl20.fs"};
	static ShaderType_t shader_type[3] = {SHADER_TYPE_VERTEX, SHADER_TYPE_GEOMETRY, SHADER_TYPE_FRAGMENT};
	static bool shader_required[3] = {true, false, true};
*/

#ifdef RC_WIN32
    static char *filename_prefix[] = {"gl3/", "gl3/"};
#else
	static char *filename_prefix[] = {"gl2/", "gl2/"};
#endif
	static char *filename_suffix[] = {".vs", ".fs"};
	static ShaderType_t shader_type[2] = {SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT};
//	static bool shader_required[2] = {true, true};

#endif

/*---------------------------------------------------------------------------*/

void Renderer::init()
{
	u32 i;

	take_screenshot = false;

	curr_tex_slot = 0;

	for (i = 0; i < RC_MAX_TEXTURE_SLOTS; i++)
		sprintf(tex_slot[i], "");

	view.identity();
	viewInverse.identity();

	projection.identity();
	projectionInverse.identity();

	viewProjection.identity();
	viewProjectionInverse.identity();


	n_vbos = 0;
	n_ibos = 0;
	n_rbos = 0;
	n_fbos = 0;
	n_textures = 0;
	n_cubemaps = 0;
	n_shaders = 0;
	n_shaderprogs = 0;

#ifndef RC_IPHONE
#ifdef RC_OPENGL_ES_20
	{
		i32 iConfigs;
		EGLint iMajorVersion, iMinorVersion;

		NativeWindowType eglWindow = 0;

		/* Init OpenGL ES 2.0 */
		eglDisplay = 0;
		eglConfig = 0;
		eglSurface = 0;
		eglContext = 0;

	#ifdef RC_WIN32
		{
			HDC hDC;
			HWND hWnd = (HWND) Platform::getWindowHandle();

			hDC = GetDC(hWnd);
			eglWindow = hWnd;
			hDC = GetDC(hWnd);

			if (!hDC) {
				exit(1);
			}

			eglDisplay = eglGetDisplay((NativeDisplayType) hDC);
		}
	#endif /* RC_WIN32 */


		if (eglDisplay == EGL_NO_DISPLAY)
			 eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);

		if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
			exit(1);
		}

		const EGLint pi32ConfigAttribs[] =
		{
			EGL_LEVEL,				0,
			EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
			EGL_NATIVE_RENDERABLE,	EGL_FALSE,
			EGL_DEPTH_SIZE,			16,
			EGL_NONE
		};

		if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs)) {
			exit(1);
		}

		if (iConfigs != 1) {
			exit(1);
		}

		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

		if (eglSurface == EGL_NO_SURFACE) {
			eglGetError(); // Clear error
			eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
		}

		eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	}
#elif defined RC_OPENGL_20

	{
		char *glver;

		glver = (char *) glGetString(GL_VERSION);
		printf("Using GL version: %s\n", glver);

		glver = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
		printf("Shading language version is: %s\n", glver);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClearDepth(1.0);

//	#if defined RC_WIN32 && defined RC_SDL
	#if defined RC_SDL
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	#endif

		if (GLEE_ARB_shader_objects == GL_FALSE) {
			printf("Critical error - no shader support!\n");
			REPORT_ERROR("Shaders are not supported! Exiting...");
		}

		printf("Shader support OK\n");

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		GL_CHECK_ERRORS();

		glDisable(GL_ALPHA_TEST);
		GL_CHECK_ERRORS();

		glDisable(GL_DITHER);
		GL_CHECK_ERRORS();

		glDisable(GL_FOG);
		GL_CHECK_ERRORS();

		glDisable(GL_LINE_SMOOTH);
		GL_CHECK_ERRORS();

#ifndef RC_OPENGL_ES_20
		glEnable(GL_TEXTURE_2D);
		GL_CHECK_ERRORS();
#endif

		glEnable(GL_DEPTH_TEST);
		GL_CHECK_ERRORS();

		glDepthFunc(GL_LEQUAL);
		GL_CHECK_ERRORS();
 	}


#endif /* RC_OPENGL_ES_20 / RC_OPENGL_20 */
#endif

	
	/* Extract limits */
	{
		u32 max_uniform_frag, max_uniform_vert;
		
		
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *) &limits.max_texture_size);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, (GLint *) &limits.max_cubemap_size);
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (GLint *) limits.max_viewport_size);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint *) &limits.max_vertex_attributes);
		
#ifdef RC_IPHONE
		limits.max_index_type_size = 2;
		
		limits.max_render_targets = 1; // HACK - FIXME
		limits.max_indices = 32000;     // HACK - FIXME
		limits.max_vertices = 32000;// HACK - FIXME
		limits.max_texture_units = 8;// HACK - FIXME
		max_uniform_vert = 1024;// HACK - FIXME
		max_uniform_frag = 1024;// HACK - FIXME
#else
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint *) &limits.max_render_targets);
//		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, (GLint *) &limits.max_indices);
//		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, (GLint *) &limits.max_vertices);
		limits.max_indices = 0x7FFFFFFF;
		limits.max_vertices = 0x7FFFFFFF;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, (GLint *) &max_uniform_vert);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, (GLint *) &max_uniform_frag);
		limits.max_index_type_size = 4;
#endif
		limits.max_uniforms = smin2<i32>(max_uniform_frag, max_uniform_vert);
		
		printf("  -- OpenGL limits --\n\n");
		printf("Max texture dimensions: %d x %d\n", limits.max_texture_size, limits.max_texture_size);
		printf("Max cubemap dimensions: %d x %d\n\n", limits.max_cubemap_size, limits.max_cubemap_size);
		printf("Max indices: %d\n", limits.max_indices);
		printf("Max vertices: %d\n", limits.max_vertices);
		printf("Max vertex attributes: %d\n\n", limits.max_vertex_attributes);
		printf("Max render target color attachments: %d\n\n", limits.max_render_targets);
		printf("Max viewport dimensions: %d x %d\n\n", limits.max_viewport_size[0], limits.max_viewport_size[1]);
		printf("Max vertex shader uniforms %d\n", max_uniform_vert);
		printf("Max fragment shader uniforms %d\n\n", max_uniform_frag);
		printf("  -------------------\n");
	}	

	u32 w, h;
	
//	default_shader_program = SceneGraph::createShaderProgram("DefaultSP", "Default", 0);
	active_render_state = new RenderState();
	
	Platform::getDisplaySize(&w, &h);
	
	setViewport(0, 0, w, h);
}

/*---------------------------------------------------------------------------*/

void Renderer::destroy()
{
	if (n_vbos != 0)
		REPORT_WARNING("Renderer still holds %d vertex buffer objects", n_vbos);

	if (n_ibos != 0)
		REPORT_WARNING("Renderer still holds %d index buffers", n_ibos);

	if (n_rbos != 0)
		REPORT_WARNING("Renderer still holds %d render buffers", n_rbos);

	if (n_fbos != 0)
		REPORT_WARNING("Renderer still holds %d frame buffer objects", n_fbos);

	if (n_textures != 0)
		REPORT_WARNING("Renderer still holds %d textures", n_textures);

	if (n_cubemaps != 0)
		REPORT_WARNING("Renderer still holds %d cube maps", n_cubemaps);

	if (n_shaders != 0)
		REPORT_WARNING("Renderer still holds %d shaders", n_shaders);

	if (n_shaderprogs != 0)
		REPORT_WARNING("Renderer still holds %d shader programs", n_shaderprogs);

	delete active_render_state;
//	delete default_render_state;
}

/*---------------------------------------------------------------------------*/

void Renderer::swapBuffers()
{
#if defined RC_IPHONE
	// TODO need to add something!! FIX ME
#elif defined RC_OPENGL_ES_20
	eglSwapBuffers(eglDisplay, eglSurface);
#elif defined RC_OPENGL_20
	
	#ifdef RC_SDL
		SDL_GL_SwapBuffers();
	#elif defined RC_GLUT
		glutSwapBuffers();
	#endif

#endif

}

/*---------------------------------------------------------------------------*/

inline static bool frustum_test(
		vec4f				fr[6],
		AABox				*box
	)
{
	u32 i;

	for (i = 0; i < 6; i++) {
		vec3f f;
		f32 dt;

		if (fr[i].x > 0.0f) {
			f.x = box->maxCorner.x;
		} else {
			f.x = box->minCorner.x;
		}

		if (fr[i].y > 0.0f) {
			f.y = box->maxCorner.y;
		} else {
			f.y = box->minCorner.y;
		}

		if (fr[i].z > 0.0f) {
			f.z = box->maxCorner.z;
		} else {
			f.z = box->minCorner.z;
		}

		dt = f.dot(fr[i].xyz());
		if ((dt + fr[i].w) < 0.0f)
			return false;
	}

	return true;
}

void Renderer::render(
		Geometry			*mesh
	)
{
	curr_tex_slot = 0; /* IMPORTANT: Must reset texture binds before rendering!! FIXME: This is a perfomance decrease, fix! */

	GLShaderProgramHandle_t *handle;
	RenderState *render_state;
	ShaderProgram *shader_program;
	GLuint programId;
	VertexArray *va;
	GLVertexArrayHandle_t *vahandle;
	PrimitiveLayout_t primitiveLayout;
	u32 i;
	u32 type_size = GL_UNSIGNED_INT;
	i32 layout;

	va = mesh->getVertexArray();

	if (!va) {
		REPORT_WARNING("No vertex array");
		return;
	}

	if (!mesh->getShaderProgram()) {
		//REPORT_WARNING("No shader program for render pass");
		return;
	}

//	if (va->getIndexLength() == 0)
//		return;


	shader_program = mesh->getShaderProgram();

	render_state = shader_program->getRenderState();

	GL_CHECK_ERRORS();
	
	if (render_state->flags & STATE_FRUSTUM_CULL) {
		AABox bbox;
		mesh->getWorldBoundingBox(&bbox);

		if (!frustum_test(frustum, &bbox))
			return;

	}
	

	mat4f worldMatrix = mesh->getWorldMatrix();

	mat4f worldMatrixInverse = mesh->getWorldMatrixInverse();

	mat4f tmp = worldMatrixInverse;

	handle = (GLShaderProgramHandle_t *) shader_program->getHandle();
	programId = handle->sp_id;
	glUseProgram(programId);
	GL_CHECK_ERRORS();

	/* Activate the correct shader */
	active_shader_program = shader_program;

	setRenderState(render_state);
	GL_CHECK_ERRORS();

		/* Set uniforms */

	/* Look for transformation matrices. */
	for (i = 0; i < RC_N_TRANSFORMATION_MATRICES; i++) {
		GLint loc = handle->loc[i];
		GL_CHECK_ERRORS();

		if (loc != -1) {
			/* The transformation keyword exists */

			u32 which_matrix;
			u32 transpose, inverse;

			mat4f mat;

			which_matrix = i & (~3);
			inverse = i & 1;
			transpose = i & 2;

			switch (which_matrix) {

				case MAT_VIEW:
					mat = inverse ? viewInverse : view;
					break;
				case MAT_PROJ:
					mat = inverse ? projectionInverse : projection;
					break;
				case MAT_VIEW_PROJ:
					mat = inverse ? viewProjectionInverse : viewProjection;
					break;
				case MAT_WORLD:
					mat = inverse ? worldMatrixInverse : worldMatrix;
					break;
				case MAT_WORLD_VIEW:
					mat = inverse ? worldMatrixInverse * viewInverse : view * worldMatrix;
					break;
				case MAT_WORLD_VIEW_PROJ:
					mat = inverse ? worldMatrixInverse * viewProjectionInverse : viewProjection * worldMatrix;
					break;
			}

			/* IMPORTANT NODE: RC uses transposed matrices, and the the transpose-parameter doesn't seem to work! */
			if (!transpose)
				mat.transpose();

			glUniformMatrix4fv(loc, 1, GL_FALSE, mat.vec);
//			glUniformMatrix4fv(loc, 1, GL_TRUE, mat.s);
			GL_CHECK_ERRORS();

		}
	}
	GL_CHECK_ERRORS();

	/* View position */
	{
		GLint loc = handle->loc[RC_N_TRANSFORMATION_MATRICES];

		if (loc != -1) {
			vec3f pos;
			pos.x = viewInverse.T0;
			pos.y = viewInverse.T1;
			pos.z = viewInverse.T2;

			glUniform3f(loc, pos.x, pos.y, pos.z);
		}
	}
	GL_CHECK_ERRORS();


	/* Set uniforms from shader program and then mesh */
	setUniform(active_shader_program->getUniform());
	GL_CHECK_ERRORS();

	setUniform(mesh->getUniform());
	GL_CHECK_ERRORS();



	vahandle = (GLVertexArrayHandle_t *) va->getHandle();

	glBindBufferARB(GL_ARRAY_BUFFER, vahandle->vbo);
	GL_CHECK_ERRORS();

	if (vahandle->has_ibo) {
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, vahandle->ibo);
		GL_CHECK_ERRORS();
	} else {
		/* LOWPRIO: This shouldn't be necessary */
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
		GL_CHECK_ERRORS();
	}


	/* Set up attributes */
	setAttributes(va);
	GL_CHECK_ERRORS();

	primitiveLayout = va->getPrimitiveLayout();


	switch (va->getPrimitiveLayout()) {

		default:
		case TRIANGLES:
			layout = GL_TRIANGLES;
			break;
		case TRIANGLE_STRIP:
			layout = GL_TRIANGLE_STRIP;
			break;
		case TRIANGLE_FAN:
			layout = GL_TRIANGLE_FAN;
			break;
		case LINES:
			layout = GL_LINES;
			break;
		case LINE_LOOP:
			layout = GL_LINE_LOOP;
			break;
		case LINE_STRIP:
			layout = GL_LINE_STRIP;
			break;

	};

	if (!vahandle->has_ibo) {

		glDrawArrays(layout, 0, va->getLength());
		GL_CHECK_ERRORS();

	} else {

		if (va->getIndexTypeSize() > limits.max_index_type_size)
			REPORT_WARNING("Too large index type size for GL to handle. Behaviour from this point on is undefined");

		switch (va->getIndexTypeSize()) {
			case 1:
				type_size = GL_UNSIGNED_BYTE;
				break;
			case 2:
				type_size = GL_UNSIGNED_SHORT;
				break;
			case 4:
				type_size = GL_UNSIGNED_INT;
				break;
			default:
				REPORT_WARNING("Index array type size must be 1, 2 or 4");
				break;
		};
		GL_CHECK_ERRORS();

#ifdef CHECK_FOR_ERRORS
		/* LOWPRIO: This shouldn't be necessary */
		GLenum result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
			REPORT_WARNING("Framebuffer is incomplete - program crash likely");
#endif /* CHECK_FOR_ERRORS */
		
		glDrawElements(layout, va->getIndexLength(), type_size, 0);
		GL_CHECK_ERRORS();

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
		GL_CHECK_ERRORS();
	}

	/* Clear attributes */
	clearAttributes(va);
	GL_CHECK_ERRORS();

	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERRORS();

	glUseProgram(0);
	GL_CHECK_ERRORS();

	glActiveTexture(GL_TEXTURE0);
	GL_CHECK_ERRORS();

	curr_tex_slot = 0;

}

/*---------------------------------------------------------------------------*/

void Renderer::enqueue(
		Geometry			*mesh
	)
{
	REPORT_ERROR("This function is not yet implemented...");
}

/*---------------------------------------------------------------------------*/

void Renderer::renderQueued()
{
	REPORT_ERROR("This function is not yet implemented...");
}


/*---------------------------------------------------------------------------*/

//void Renderer::setShaderProgram(
//		ShaderProgram		*shader_program
//	)
//{
//	if (!shader_program) {
//		active_shader_program = default_shader_program;
//	} else {
//		active_shader_program = shader_program;
//	}
//}

/*---------------------------------------------------------------------------*/

GLuint Renderer::loadShader(
		const char			*parent,
		char				*prefix,
		char				*code,
		ShaderType_t		type
	)
{
	GLint id = 0, status;

	/* Compile shader */

	switch (type) {
		case SHADER_TYPE_VERTEX:
#if defined RC_OPENGL_ES_20
			id = glCreateShader(GL_VERTEX_SHADER);
#elif defined RC_OPENGL_20
			id = (GLint) glCreateShaderObjectARB(GL_VERTEX_SHADER);
#endif
			GL_CHECK_ERRORS();
			break;
		case SHADER_TYPE_FRAGMENT:
#if defined RC_OPENGL_ES_20
			id = glCreateShader(GL_FRAGMENT_SHADER);
#elif defined RC_OPENGL_20
			id = (GLint) glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
#endif
			GL_CHECK_ERRORS();
			break;
		default:
			REPORT_WARNING("The shader type is not supported by the renderer");
			return 0;
			break;
	}

	if (prefix && *prefix) {
		char *src[2];
		src[0] = prefix;
		src[1] = code;
		glShaderSource(id, 2, (const char **) src, NULL);
	} else {
		glShaderSource(id, 1, (const char **) &code, NULL);
	}
	GL_CHECK_ERRORS();

	glCompileShader(id);
	GL_CHECK_ERRORS();

	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	GL_CHECK_ERRORS();

	if (status == GL_FALSE) {
		GLsizei length;
		char result[RC_RENDERER_ERR_BUF_LEN];

		glGetShaderInfoLog(id, RC_RENDERER_ERR_BUF_LEN, &length, result);

		glDeleteShader(id);

		if (length)
			REPORT_WARNING(result);
        REPORT_WARNING("Couldn't compile %s shader for \"%s\"", type == SHADER_TYPE_VERTEX ? "vertex" : "fragment", parent);
        return 0;
	} else {
		GLsizei length;
		char result[RC_RENDERER_ERR_BUF_LEN];

		glGetShaderInfoLog(id, RC_RENDERER_ERR_BUF_LEN, &length, result);

		if (length) {
			REPORT("%s shader compiled with a message (for \"%s\")", type == SHADER_TYPE_VERTEX ? "Vertex" : "Fragment", parent);

			REPORT("%s\n --- ", result);
		}
	}

	n_shaders++;

	return id;
}

/*---------------------------------------------------------------------------*/

bool Renderer::loadShaderProgram(
		ShaderProgram		*shader_program,
		char				*desc,
		char				*prefix
	)
{
	char *buf = 0;
	u32 buf_len;

	char fnbuf[2][FILENAME_MAX];
	u32 i;
	GLint loc;
	GLShaderProgramHandle_t *hdl;
	GLint status;
	GLuint id;

	//GLuint vsid, fsid;
	GLuint shader_id[2];

	buf_len = 0;

	for (i = 0; i < 2; i++) {
		FILE *file;
		u32 file_len;
		const char *fnpath = RC_SHADER_PATH;

		if (strlen(desc) + strlen(filename_suffix[i]) + strlen(fnpath) + strlen(filename_prefix[i]) >= (FILENAME_MAX - 1)) {
			REPORT_WARNING("Shader filename too long \"%s%s%s%s\"", fnpath, filename_prefix[i], desc, filename_suffix[i]);
			return false;
		}

		strcpy(fnbuf[i], fnpath);
		strcat(fnbuf[i], filename_prefix[i]);
		strcat(fnbuf[i], desc);
		strcat(fnbuf[i], filename_suffix[i]);


		/* Read source file */
		file = fopen(fnbuf[i], "rb");

		if (!file) {
			REPORT_WARNING("Couldn't load shader source for \"%s\"", fnbuf[i]);

			if (buf)
				free(buf);

			return false;
		}

		fseek(file, 0, SEEK_END);
		file_len = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (buf_len == 0) {
			buf = (char *) malloc(sizeof(char) * (file_len + 1));
			buf_len = (file_len + 1);
		} else if (buf_len < (file_len + 2)) {
			buf = (char *) realloc(buf, sizeof(char) * (file_len + 1));
			buf_len = (file_len + 1);
		}

		if (!fread(buf, file_len, 1, file)) {
			REPORT_WARNING("Couldn't read shader source for \"%s\"", fnbuf[i]);

			if (buf)
				free(buf);

			fclose(file);

			return false;
		}

		buf[file_len] = 0;

		fclose(file);

		if (!(shader_id[i] = loadShader(shader_program->getIdentifier(), prefix, buf, shader_type[i]))) {

			u32 j;

			GL_CHECK_ERRORS();

			for (j = 0; j < i; j++) {

				if (shader_id[j])
					glDeleteShader(shader_id[j]);

				n_shaders--;

			}

			if (buf)
				free(buf);

			return false;
		}


	}

	if (buf)
		free(buf);

	id = glCreateProgram();
	GL_CHECK_ERRORS();

	glAttachShader(id, shader_id[0]);
	glAttachShader(id, shader_id[1]);
	GL_CHECK_ERRORS();

	glLinkProgram(id);
	GL_CHECK_ERRORS();

	glGetProgramiv(id, GL_LINK_STATUS, &status);
	GL_CHECK_ERRORS();

	if (status == GL_FALSE) {
		GLsizei length;
//		char result[SHADER_PROGRAM_ERROR_BUFFER_LENGTH];
		char result[8192];

		REPORT_WARNING("Couldn't link program %s", shader_program->getIdentifier());

		glGetProgramInfoLog(id, 8192, &length, result);

		GL_CHECK_ERRORS();

		glDeleteProgram(id);

		if (shader_id[0])
			glDeleteShader(shader_id[0]);

		if (shader_id[1])
			glDeleteShader(shader_id[1]);

		if (length) {
			REPORT_WARNING(result);

			return false;
		}
	} else {
		GLsizei length;
//		char result[SHADER_PROGRAM_ERROR_BUFFER_LENGTH];
		char result[8192];

		glGetProgramInfoLog(id, 8192, &length, result);

		GL_CHECK_ERRORS();

		if (length) {
			REPORT("Shader linking of \"%s\" returned a message", shader_program->getIdentifier());

			REPORT("%s\n --- ", result);

		}
	}

	GL_CHECK_ERRORS();


	/* Set up handle */
	hdl = new GLShaderProgramHandle_t;

	hdl->sp_id = id;
	hdl->vs_id = shader_id[0];
	hdl->fs_id = shader_id[1];

	shader_program->setHandle((void *) hdl);

	/* Check for default uniform names */

	for (i = 0; i < RC_N_TRANSFORMATION_MATRICES; i++) {
		loc = glGetUniformLocation(id, gl_matrix_transform[i]);
		hdl->loc[i] = loc;
		GL_CHECK_ERRORS();
	}

	loc = glGetUniformLocation(id, "ViewPosition");
	GL_CHECK_ERRORS();
	hdl->loc[i] = loc;

	n_shaderprogs++;

	return true;
}

/*---------------------------------------------------------------------------*/

void Renderer::unloadShaderProgram(
		ShaderProgram		*shader_program
	)
{
	GLShaderProgramHandle_t *hdl = (GLShaderProgramHandle_t *) shader_program->getHandle();

	if (hdl) {
		glDeleteShader(hdl->fs_id);
		GL_CHECK_ERRORS();

		glDeleteShader(hdl->vs_id);
		GL_CHECK_ERRORS();

		glDeleteProgram(hdl->sp_id);
		GL_CHECK_ERRORS();

		delete hdl;

		n_shaders -= 2;
		n_shaderprogs--;
	}

}

/*---------------------------------------------------------------------------*/

bool Renderer::getShaderHasUniform(
		ShaderProgram		*shader_program,
		const char			*name
	)
{
	GLint loc;
	GLuint id;
	GLShaderProgramHandle_t *handle;

	handle = (GLShaderProgramHandle_t *) active_shader_program->getHandle();
	id = handle->sp_id;

	loc = glGetUniformLocation(id, name);

	if (loc == -1)
		return false;

	return true;
}

/*---------------------------------------------------------------------------*/

void Renderer::setAttributes(
		VertexArray			*vertex_array
	)
{
	static const GLenum attrib_to_gl[] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
		GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, 0, GL_FLOAT };

	GLuint id;
	GLShaderProgramHandle_t *handle;

	u32 stride;

	u32 i;
	u32 n = vertex_array->getAttributeCount();

	if (n > limits.max_vertex_attributes)
		REPORT_WARNING("Too many vertex attributes for GL to handle. Behaviour from this point on is undefined");

	handle = (GLShaderProgramHandle_t *) active_shader_program->getHandle();
	id = handle->sp_id;

	stride = vertex_array->getStride();

	for (i = 0; i < n; i++) {
		GLenum gltype;
		Attribute_t *attrib;
		GLint loc;

		attrib = vertex_array->getAttribute(i);

		loc = glGetAttribLocation(id, attrib->name);
		GL_CHECK_ERRORS();

		if (loc == -1)
			continue;

		gltype = attrib_to_gl[(u32) attrib->type];

		glVertexAttribPointer(loc, attrib->length, gltype,
			attrib->normalized ? GL_TRUE : GL_FALSE, stride,
			(const void *) attrib->offset);

		GL_CHECK_ERRORS();

		glEnableVertexAttribArray(loc);
		GL_CHECK_ERRORS();
	}

}

/*---------------------------------------------------------------------------*/

void Renderer::clearAttributes(
		VertexArray			*vertex_array
	)
{
	/*
	static const GLenum attrib_to_gl[] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
		GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, 0, GL_FLOAT };
	 */

	GLuint id;
	GLShaderProgramHandle_t *handle;

	u32 stride;

	u32 i;
	u32 n = vertex_array->getAttributeCount();

	handle = (GLShaderProgramHandle_t *) active_shader_program->getHandle();
	id = handle->sp_id;

	stride = vertex_array->getStride();

	for (i = 0; i < n; i++) {
		Attribute_t *attrib;
		GLint loc;

		attrib = vertex_array->getAttribute(i);

		loc = glGetAttribLocation(id, attrib->name);
		GL_CHECK_ERRORS();

		if (loc == -1)
			continue;

		glDisableVertexAttribArray(loc);
		GL_CHECK_ERRORS();
	}

}

/*---------------------------------------------------------------------------*/

void Renderer::setUniform(
		Uniform				*uniform
	)
{
	GLuint id;

	u32 i, j;
	u32 n_entries;
	UniformEntry_t *entries;
	GLShaderProgramHandle_t *handle;

	handle = (GLShaderProgramHandle_t *) active_shader_program->getHandle();
	id = handle->sp_id;

	n_entries = uniform->getEntryCount();
	entries = uniform->getEntries();

	for (i = 0; i < n_entries; i++) {
		GLint loc;

		GL_CHECK_ERRORS();

		loc = glGetUniformLocation(id, entries[i].name);
		GL_CHECK_ERRORS();

		if (loc == -1)
			continue;


		if (entries[i].flags & (UNIFORM_FLAG_TEXTURE | UNIFORM_FLAG_CUBEMAP)) {

			/*
			 * Texture or Cube Map
			 *
			 */

			GLuint texId, slot;

			if (entries[i].flags & UNIFORM_FLAG_TEXTURE) {

				GLTextureHandle_t *thandle;
				thandle = (GLTextureHandle_t *) entries[i].handle;
				texId = thandle->tex_id;

			} else {

				GLCubeMapHandle_t *chandle;
				chandle = (GLCubeMapHandle_t *) entries[i].handle;
				texId = chandle->ctex_id;

			}

			
			slot = 0;

			if (strlen(entries[i].name) >= (RC_MAX_TEXTURE_NAME - 1)) {
				REPORT_WARNING("The texture name %s is too long. Ignoring incoming texture", entries[i].name);
				continue;
			}

			for (j = 0; j < curr_tex_slot; j++) {
				if (strcmp(tex_slot[j], entries[i].name) == 0)
					break; /* Already bound, but we will re-bind */
			}

			if (j == curr_tex_slot) {
				/* Not previously bound */
				if (curr_tex_slot >= RC_MAX_TEXTURE_SLOTS) {
					REPORT_WARNING("Using too many texture slots (%d). Ignoring incoming texture", curr_tex_slot);
					continue;
				}

				strcpy(tex_slot[j], entries[i].name);
				glActiveTexture(GL_TEXTURE0 + curr_tex_slot);
				slot = curr_tex_slot;

				curr_tex_slot++;
			} else {
				glActiveTexture(GL_TEXTURE0 + j);
				slot = j;
			}

			GL_CHECK_ERRORS();


			if (entries[i].flags & UNIFORM_FLAG_TEXTURE) {
				glBindTexture(GL_TEXTURE_2D, texId);
			} else { /* It's a cube map */
				glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
			}

			GL_CHECK_ERRORS();

			glUniform1i(loc, slot);
			GL_CHECK_ERRORS();

			{
				i32 valid;
				glValidateProgram(id);
				glGetProgramiv(id, GL_VALIDATE_STATUS, &valid);

				if (!valid)
					REPORT_WARNING("The shader is no longer valid");

			}

		} else if (entries[i].flags & UNIFORM_FLAG_INT) { /* int */

			switch (entries[i].flags & UNIFORM_FLAG_CLASS_MASK) {

				case (UNIFORM_FLAG_SCALAR): /* Scalar */

					if (entries[i].n == 1) {
						glUniform1i(loc, *((i32 *) &entries[i].value)); /* Single value */
					} else {
						glUniform1iv(loc, entries[i].n, (GLint *) entries[i].ptr); /* Array */
					}
					break;

				case (UNIFORM_FLAG_VECTOR2): /* Vec2 int */

					if (entries[i].n == 1) {
						i32 *v = (i32 *) entries[i].ptr;
						glUniform2i(loc, v[0], v[1]);
					} else {
						glUniform2iv(loc, entries[i].n, (GLint *) entries[i].ptr);
					}
					break;

				case (UNIFORM_FLAG_VECTOR3):

					if (entries[i].n == 1) {
						i32 *v = (i32 *) entries[i].ptr;
						glUniform3i(loc, v[0], v[1], v[2]);
					} else {
						glUniform3iv(loc, entries[i].n, (GLint *) entries[i].ptr);
					}
					break;

				case (UNIFORM_FLAG_VECTOR4):

					if (entries[i].n == 1) {
						i32 *v = (i32 *) entries[i].ptr;
						glUniform4i(loc, v[0], v[1], v[2], v[3]);
					} else {
						glUniform4iv(loc, entries[i].n, (GLint *) entries[i].ptr);
					}
					break;

				default:
					break;
			}

		} else { /* must be float */

			switch (entries[i].flags & UNIFORM_FLAG_CLASS_MASK) {
				case (UNIFORM_FLAG_SCALAR): /* Scalar */

					if (entries[i].n == 1) {
						glUniform1f(loc, *((f32 *) &entries[i].value)); /* Single value */
					} else {
						GL_CHECK_ERRORS();
						glUniform1fv(loc, entries[i].n, (GLfloat *) entries[i].ptr); /* Array */
						GL_CHECK_ERRORS();
					}
					break;

				case (UNIFORM_FLAG_VECTOR2): /* Vec2 float */

					if (entries[i].n == 1) {
						vec2f *v = (vec2f *) entries[i].ptr;
						glUniform2f(loc, v->x, v->y);
					} else {
						glUniform2fv(loc, entries[i].n, (GLfloat *) entries[i].ptr);
					}
					break;

				case (UNIFORM_FLAG_VECTOR3):

					if (entries[i].n == 1) {
						vec3f *v = (vec3f *) entries[i].ptr;
						glUniform3f(loc, v->x, v->y, v->z);
					} else {
						glUniform3fv(loc, entries[i].n, (GLfloat *) entries[i].ptr);
					}
					break;

				case (UNIFORM_FLAG_VECTOR4):

					if (entries[i].n == 1) {
						vec4f *v = (vec4f *) entries[i].ptr;
						glUniform4f(loc, v->x, v->y, v->z, v->w);
					} else {
						glUniform4fv(loc, entries[i].n, (GLfloat *) entries[i].ptr);
					}
					break;

				case (UNIFORM_FLAG_MATRIX2): /* Mat2 float */

					{
						mat2f mat = *((mat2f *) entries[i].ptr);
						mat.transpose();

						glUniformMatrix2fv(loc, entries[i].n, GL_FALSE, (GLfloat *) mat.vec);
					}
					break;

				case (UNIFORM_FLAG_MATRIX3):

					{
						mat3f mat = *((mat3f *) entries[i].ptr);
						mat.transpose();

						glUniformMatrix3fv(loc, entries[i].n, GL_FALSE, (GLfloat *) mat.vec);
					}
					break;

				case (UNIFORM_FLAG_MATRIX4):

					{
						mat4f mat = *((mat4f *) entries[i].ptr);
						mat.transpose();

						glUniformMatrix4fv(loc, entries[i].n, GL_FALSE, (GLfloat *) mat.vec);
					}
					break;

				default:
					break;
			}

		}

		GL_CHECK_ERRORS();
	}


	GL_CHECK_ERRORS();

}

/*---------------------------------------------------------------------------*/

bool Renderer::loadRenderTarget(
		RenderTarget		*target,
		u32					n_tex,
		bool				include_depth_buffer,
		bool				include_stencil_buffer
	)
{
	u32 i, j;

	/* LOWPRIO: Stencil buffer */

	GLRenderTargetHandle_t *handle;

	GLuint fbo;
	GLenum result;

	GLuint rbo_id;

	if (n_tex > limits.max_render_targets)
		REPORT_WARNING("Too many render target textures for GL to handle. Behaviour from this point on is undefined");

	for (j = 0; j < RC_MAX_TEXTURE_SLOTS; j++) {
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, 0);
		curr_tex_slot = 0;
	}

	handle = new GLRenderTargetHandle_t;

	/* Framebuffer */
	glGenFramebuffersEXT(1, &fbo);
	GL_CHECK_ERRORS();

	glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);
	GL_CHECK_ERRORS();

	for (i = 0; i < n_tex; i++) {
		Texture *tex;
		GLTextureHandle_t *th;

		tex = target->getTexture(i);

		if (!tex) {
			REPORT_WARNING("Unable to initiate render target - missing texture!");
			return false;
		}

		th = (GLTextureHandle_t *) tex->getHandle();

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, th->tex_id, 0);
		GL_CHECK_ERRORS();
	}

	/* Renderbuffer */ 
	if (include_depth_buffer) {
		glGenRenderbuffersEXT(1, &rbo_id);
		GL_CHECK_ERRORS();

		glBindRenderbufferEXT(GL_RENDERBUFFER, rbo_id);
		GL_CHECK_ERRORS();

#if defined RC_OPENGL_ES_20
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, target->getWidth(), target->getHeight());
#elif defined RC_OPENGL_20
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, target->getWidth(), target->getHeight());
#endif
		GL_CHECK_ERRORS();

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);
		GL_CHECK_ERRORS();

		handle->rbo = new RBO_t;
		handle->rbo->id = rbo_id;
		handle->rbo->refs = 1;

		n_rbos++;

		glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
		GL_CHECK_ERRORS();

//	} else if (include_depth_buffer && slot != 0) {
//		REPORT_WARNING("Depth buffer attachment is only 
//		thandle->has_rbo = false;
	} else {
		handle->rbo = 0;
	}

	result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if (result != GL_FRAMEBUFFER_COMPLETE) {
		REPORT_WARNING("Failed to create render target \"%s\"", target->getIdentifier());
		return false;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	GL_CHECK_ERRORS();

	handle->fbo = fbo;

	handle->n_targets = n_tex;

	target->setHandle(handle);

	n_fbos++;

	return true;
}

/*---------------------------------------------------------------------------*/

void Renderer::unloadRenderTarget(
		RenderTarget		*target
	)
{
	u32 j;

	GLuint fbo;
	GLRenderTargetHandle_t *handle;

	for (j = 0; j < RC_MAX_TEXTURE_SLOTS; j++) {
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, 0);
		curr_tex_slot = 0;
	}

	handle = (GLRenderTargetHandle_t *) target->getHandle();

	fbo = handle->fbo;

	glDeleteFramebuffersEXT(1, &handle->fbo);
	GL_CHECK_ERRORS();

	if (handle->rbo) {
		handle->rbo->refs--;

		if (handle->rbo->refs == 0) {
			handle->rbo->refs = 0;
			glDeleteRenderbuffersEXT(1, &handle->rbo->id);
			GL_CHECK_ERRORS();

			delete handle->rbo;

			n_rbos--;
		}
	}

	handle->fbo = 0;
	handle->rbo = 0;

	n_fbos--;
}

/*---------------------------------------------------------------------------*/

bool Renderer::shareDepthBuffer(
		RenderTarget		*dst,
		RenderTarget		*src
	)
{
	u32 j;
	/* LOWPRIO: Stencil buffer */

	GLRenderTargetHandle_t *shandle, *dhandle;

	GLenum result;

	for (j = 0; j < RC_MAX_TEXTURE_SLOTS; j++) {
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, 0);
		curr_tex_slot = 0;
	}

	shandle = (GLRenderTargetHandle_t *) src->getHandle();
	dhandle = (GLRenderTargetHandle_t *) dst->getHandle();

	if (!shandle->rbo) {
		REPORT_WARNING("Source render target \"%s\"doesn't have a depth buffer attached.", src->getIdentifier());
		return false;
	}

	/* Framebuffer */
	glBindFramebufferEXT(GL_FRAMEBUFFER, dhandle->fbo);
	GL_CHECK_ERRORS();

	/* Renderbuffer */
	glBindRenderbufferEXT(GL_RENDERBUFFER, shandle->rbo->id);
	GL_CHECK_ERRORS();

#if defined RC_OPENGL_ES_20
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, dst->getWidth(), dst->getHeight());
#elif defined RC_OPENGL_20
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dst->getWidth(), dst->getHeight());
#endif
	GL_CHECK_ERRORS();

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shandle->rbo->id);
	GL_CHECK_ERRORS();

	result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if (result != GL_FRAMEBUFFER_COMPLETE) {
		REPORT_WARNING("Failed to share depth buffer from \"%s\" to \"%s\"", src->getIdentifier(), dst->getIdentifier());
		return false;
	}

	dhandle->rbo = shandle->rbo;
	shandle->rbo->refs++;

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	GL_CHECK_ERRORS();

	glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
	GL_CHECK_ERRORS();

	return true;
}

/*---------------------------------------------------------------------------*/

void Renderer::setRenderTarget(
		RenderTarget		*target
	)
{
	GLenum buf[RC_MAX_RENDER_TARGET_TEXTURES];
	GLRenderTargetHandle_t *handle;
	GLenum result;
	u32 i, j;

	for (j = 0; j < RC_MAX_TEXTURE_SLOTS; j++) {
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, 0);
		curr_tex_slot = 0;
	}

//	glBindTexture(GL_TEXTURE_2D, 0);

	if (!target) {

		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		GL_CHECK_ERRORS();

		glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
		GL_CHECK_ERRORS();

		glViewport(viewport_offset[0], viewport_offset[1],
			viewport_size[0], viewport_size[1]);

		GL_CHECK_ERRORS();

		return;
	}

	handle = (GLRenderTargetHandle_t *) target->getHandle();

	glBindFramebufferEXT(GL_FRAMEBUFFER, handle->fbo);
	GL_CHECK_ERRORS();

	if (handle->rbo) {
		glBindRenderbufferEXT(GL_RENDERBUFFER, handle->rbo->id);
		GL_CHECK_ERRORS();
	}

	for (i = 0; i < handle->n_targets; i++) {
		buf[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(handle->n_targets, buf);

	/* TODO: TMP */
//	glClearDepth(1.0f);
//	glClearColor(0.1f, 0.2f, 0.3f, 100.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

//		glBindFramebuffer(GL_FRAMEBUFFER, thandle->fbo);

	GL_CHECK_ERRORS();

	glViewport(0, 0, target->getWidth(), target->getHeight());

	result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if (result != GL_FRAMEBUFFER_COMPLETE) {
		REPORT_WARNING("Failed to bind render target for texture %s", target->getIdentifier());
		return;
	}
}

/*---------------------------------------------------------------------------*/

/* Textures */
void Renderer::loadTexture(
		Texture				*texture,
		u8					*pixels,
		u32					width,
		u32					height,
		TextureFormat_t		format,
		bool				generate_mipmaps,
		TextureFilter_t		filter,
		TextureWrap_t		wrap
	)
{
	GLTextureHandle_t *thandle;
	GLuint texId;
	u32 bpp;
	GLenum frm;

	if (width > limits.max_texture_size && height > limits.max_texture_size)
		REPORT_WARNING("Too large texture for GL to handle. Behaviour from this point on is undefined");

#ifdef RC_IPHONE
	if ((width & (width - 1) || (height & (height - 1)))
		REPORT_WARNING("Textures must have dimensions that are powers of 2");
#endif
	
	glGenTextures(1, &texId);
	GL_CHECK_ERRORS();

	glBindTexture(GL_TEXTURE_2D, texId);
	GL_CHECK_ERRORS();

	texture->width = width;
	texture->height = height;

//	width = nextPow2(width);
//	height = nextPow2(height);

	bpp = 0;
	frm = 0;

#ifndef RC_IPHONE // HACK -- FIX ME
	if (generate_mipmaps) {
		switch (format) {
			default:
			case TEXTURE_FORMAT_RGBA8:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				bpp = 4;
				frm = GL_RGBA;
				break;
			case TEXTURE_FORMAT_RGB8:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				bpp = 3;
				frm = GL_RGB;
				break;
			case TEXTURE_FORMAT_GRAY8:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
				bpp = 1;
				frm = GL_LUMINANCE;
				break;
			case TEXTURE_FORMAT_RGBA16:
	#if defined RC_OPENGL_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_SHORT, pixels);
	#elif defined RC_OPENGL_ES_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_SHORT, pixels);
	#endif
				break;
			case TEXTURE_FORMAT_RGB16:
	#if defined RC_OPENGL_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_SHORT, pixels);
	#elif defined RC_OPENGL_ES_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_SHORT, pixels);
	#endif
				break;
			case TEXTURE_FORMAT_GRAY16:
	#if defined RC_OPENGL_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, GL_UNSIGNED_SHORT, pixels);
	#elif defined RC_OPENGL_ES_20
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, GL_UNSIGNED_SHORT, pixels);
	#endif
				break;
		}
		GL_CHECK_ERRORS();
	} else {
#endif /* RC_IPHONE */
		switch (format) {
			default:
			case TEXTURE_FORMAT_RGBA8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				bpp = 4;
				frm = GL_RGBA;
				break;
			case TEXTURE_FORMAT_RGB8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				bpp = 3;
				frm = GL_RGB;
				break;
			case TEXTURE_FORMAT_GRAY8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
				bpp = 1;
				frm = GL_LUMINANCE;
				break;
			case TEXTURE_FORMAT_RGBA16:
#if defined RC_OPENGL_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, pixels);
#elif defined RC_OPENGL_ES_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, pixels);
#endif
				break;
			case TEXTURE_FORMAT_RGB16:
#if defined RC_OPENGL_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT, pixels);
#elif defined RC_OPENGL_ES_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT, pixels);
#endif
				break;
			case TEXTURE_FORMAT_GRAY16:
#if defined RC_OPENGL_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, pixels);
#elif defined RC_OPENGL_ES_20
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, pixels);
#endif
				break;
		}
		GL_CHECK_ERRORS();
#ifndef RC_IPHONE
	}
#endif /* RC_IPHONE */

	switch (filter) {
		case TEXTURE_FILTER_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		default:
		case TEXTURE_FILTER_BILINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case TEXTURE_FILTER_TRILINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
	}
	GL_CHECK_ERRORS();

	switch (wrap) {
		case TEXTURE_WRAP_CLAMP:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		default:
		case TEXTURE_WRAP_REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TEXTURE_WRAP_MIRRORED_REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
	}
	GL_CHECK_ERRORS();

//	if (generate_mipmaps)
//		glGenerateMipmap(GL_TEXTURE_2D);

//	if (generate_mipmaps)
//		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	/* TODO: This should definitly be glGenerateMipmap, but it doesn't work with OpenGL 2.1 :( */
#if defined RC_IPHONE // HACK -- FIX ME
	if (generate_mipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);
#else
//	if (generate_mipmaps)
//		gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, frm, GL_UNSIGNED_BYTE, pixels);
#endif
	
	GL_CHECK_ERRORS();

	texture->format = format;
	texture->hasMipmaps = generate_mipmaps;

	thandle = new GLTextureHandle_t;

	thandle->tex_id = texId;

	texture->setHandle((void *) thandle);

	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK_ERRORS();

	n_textures++;
}

/*---------------------------------------------------------------------------*/

void Renderer::generateMipmaps(
		Texture				*texture
	)
{
	GLTextureHandle_t *thandle;
	
	thandle = (GLTextureHandle_t *) texture->getHandle();
	
	if (!thandle->tex_id) {
		REPORT_WARNING("No texture bound");
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, thandle->tex_id);
	
	if (thandle)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::unloadTexture(
		Texture				*texture
	)
{
	GLTextureHandle_t *thandle;
	GLuint texId;

	thandle = (GLTextureHandle_t *) texture->getHandle();

	if (thandle) {

		texId = thandle->tex_id;
		glDeleteTextures(1, &texId);

		delete thandle;

		n_textures--;
	}

	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::setCubeTex(
		u8					*pixels,
		u32					w,
		u32					h,
		u32					bpp,
		u32					side
	)
{
	if (bpp == 4) {
		glTexImage2D(side, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	} else if (bpp == 3) {
		glTexImage2D(side, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	} else if (bpp == 1) {
		glTexImage2D(side, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
	} else {
		REPORT("The texture doesn't have 1, 3 or 4 bytes per pixel");
		return;
	}

	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::loadCubeMap(
		CubeMap				*cubemap,
		u8					**pixels,
		u32					width,
		u32					height,
		u32					bpp
	)
{
	GLCubeMapHandle_t *chandle;
	GLuint texId;

	if (width > limits.max_cubemap_size && height > limits.max_cubemap_size)
		REPORT_WARNING("Too large cube map for GL to handle. Behaviour from this point on is undefined");

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	GL_CHECK_ERRORS();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GL_CHECK_ERRORS();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	GL_CHECK_ERRORS();

	setCubeTex(pixels[0], width, height, bpp, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	setCubeTex(pixels[1], width, height, bpp, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	setCubeTex(pixels[2], width, height, bpp, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	setCubeTex(pixels[3], width, height, bpp, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	setCubeTex(pixels[4], width, height, bpp, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	setCubeTex(pixels[5], width, height, bpp, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);

	GL_CHECK_ERRORS();

	chandle = new GLCubeMapHandle_t;

	chandle->ctex_id = texId;

	cubemap->handle = chandle;

	n_cubemaps++;
}

/*---------------------------------------------------------------------------*/

void Renderer::unloadCubeMap(
		CubeMap				*cubemap
	)
{
	GLCubeMapHandle_t *chandle;

	chandle = (GLCubeMapHandle_t *) cubemap->handle;
	glDeleteTextures(1, &chandle->ctex_id);

	delete ((GLCubeMapHandle_t *) cubemap->handle);

	n_cubemaps--;

	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::loadVertexArray(
		VertexArray			*vertex_array,
		void				*new_array,
		u32					n,
		AttributeUsage_t	usage
	)
{
	GLuint gl_usage;
	GLuint vbo;
	GLVertexArrayHandle_t *vahandle;

	GL_CHECK_ERRORS();

	if (vertex_array->getLength() > limits.max_vertices)
		REPORT_WARNING("Too many vertices for GL to handle. Behaviour from this point on is undefined");

	glGenBuffersARB(1, &vbo);
	GL_CHECK_ERRORS();

	glBindBufferARB(GL_ARRAY_BUFFER, vbo);
	GL_CHECK_ERRORS();

	gl_usage = gl_usage_map[vertex_array->getUsage()];
	glBufferDataARB(GL_ARRAY_BUFFER, vertex_array->getStride() * vertex_array->getLength(), new_array, gl_usage);
	GL_CHECK_ERRORS();

	vahandle = new GLVertexArrayHandle_t;

	vahandle->has_ibo = false;
	vahandle->vbo = vbo;

	vertex_array->setHandle(vahandle);

	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERRORS();

	n_vbos++;
}

/*---------------------------------------------------------------------------*/

void Renderer::unloadVertexArray(
		VertexArray			*vertex_array
	)
{
	GLVertexArrayHandle_t *vahandle;

	vahandle = (GLVertexArrayHandle_t *) vertex_array->getHandle();

	if (vahandle) {
		if (vahandle->vbo) {
			glDeleteBuffers(1, &vahandle->vbo);
			n_vbos--;
		}

		if (vahandle->has_ibo && vahandle->ibo) {
			glDeleteBuffers(1, &vahandle->ibo);
			n_ibos--;
		}

		delete vahandle;
	}

}

/*---------------------------------------------------------------------------*/

void Renderer::reloadVertexArray(
		VertexArray			*vertex_array,
		void				*new_array,
		u32					n
	)
{
	GLuint gl_usage;
	GLVertexArrayHandle_t *vahandle;

	vahandle = (GLVertexArrayHandle_t *) vertex_array->getHandle();

	glBindBufferARB(GL_ARRAY_BUFFER, vahandle->vbo);
	GL_CHECK_ERRORS();

	gl_usage = gl_usage_map[vertex_array->getUsage()];
	glBufferDataARB(GL_ARRAY_BUFFER, vertex_array->getStride() * vertex_array->getLength(), new_array, gl_usage);
	GL_CHECK_ERRORS();

	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::loadIndexArray(
		VertexArray			*vertex_array,
		void				*new_array,
		u32					n,
		AttributeUsage_t	usage
	)
{
	GLuint gl_usage;
	GLuint ibo;
	GLVertexArrayHandle_t *vahandle;

	if (vertex_array->getIndexLength() > limits.max_indices)
		REPORT_WARNING("Too many indices for GL to handle. Behaviour from this point on is undefined");

	vahandle = (GLVertexArrayHandle_t *) vertex_array->getHandle();

	glGenBuffersARB(1, &ibo);
	GL_CHECK_ERRORS();

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
	GL_CHECK_ERRORS();

	gl_usage = gl_usage_map[vertex_array->getIndexUsage()];
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, vertex_array->getIndexLength() * vertex_array->getIndexTypeSize(), new_array, gl_usage);
	GL_CHECK_ERRORS();

	vahandle->has_ibo = true;
	vahandle->ibo = ibo;

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_CHECK_ERRORS();

	n_ibos++;

}

/*---------------------------------------------------------------------------*/

void Renderer::unloadIndexArray(
		VertexArray			*vertex_array
	)
{
	/* TODO: Test me! */
	GLVertexArrayHandle_t *vahandle;
	
	vahandle = (GLVertexArrayHandle_t *) vertex_array->getHandle();
	
	if (vahandle) {
		if (vahandle->has_ibo && vahandle->ibo) {
			glDeleteBuffers(1, &vahandle->ibo);
			n_ibos--;
		}
		
		vahandle->ibo = 0;
	}
	
}

/*---------------------------------------------------------------------------*/

void Renderer::reloadIndexArray(
		VertexArray			*vertex_array,
		void				*new_array,
		u32					n
	)
{
	GLuint gl_usage;
	GLVertexArrayHandle_t *vahandle;

	vahandle = (GLVertexArrayHandle_t *) vertex_array->getHandle();

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, vahandle->ibo);
	GL_CHECK_ERRORS();

	gl_usage = gl_usage_map[vertex_array->getIndexUsage()];
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, vertex_array->getIndexLength() * vertex_array->getIndexTypeSize(), new_array, gl_usage);
	GL_CHECK_ERRORS();

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::setRenderState(
		RenderState			*render_state
	)
{
	RenderState *s;

	if (!render_state) {
		*active_render_state = *default_render_state;
	} else {
		*active_render_state = *render_state;
	}

	s = active_render_state;

	if (s->flags & STATE_DEPTH) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(gl_cmp_map[(u32) s->depthTest]);
		GL_CHECK_ERRORS();
	} else {
		glDisable(GL_DEPTH_TEST);
	}
	GL_CHECK_ERRORS();

	if (s->flags & STATE_DEPTH_WRITE) {
		glDepthMask(GL_TRUE);
	} else {
		glDepthMask(GL_FALSE);
	}
//	glDepthMask(GL_FALSE);

//	glDepthMask((s->flags & STATE_DEPTH_WRITE) ? GL_TRUE : GL_FALSE);

//	glDepthMask(s->flags & STATE_DEPTH_WRITE ? GL_FALSE : GL_TRUE);
	GL_CHECK_ERRORS();

	if (s->flags & STATE_STENCIL) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(gl_cmp_map[(u32) s->stencilTest], s->stencilRef, s->stencilMask);
	} else {
		glDisable(GL_STENCIL_TEST);
	}
	GL_CHECK_ERRORS();

	if (s->flags & STATE_BLEND) {
		glEnable(GL_BLEND);
		glBlendFunc(gl_src_blend_map[(u32) s->srcBlend], gl_dst_blend_map[(u32) s->dstBlend]);
	} else {
		glDisable(GL_BLEND);
	}
	GL_CHECK_ERRORS();

	if (s->flags & STATE_CULL) {
		glEnable(GL_CULL_FACE);
		glCullFace(gl_cull_map[(u32) s->cull]);
	} else {
		glDisable(GL_CULL_FACE);
	}
	GL_CHECK_ERRORS();

	if (s->flags & STATE_SCISSOR) {
		glEnable(GL_SCISSOR_TEST);
		glScissor(s->scissorX, s->scissorY, s->scissorW, s->scissorH);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
	GL_CHECK_ERRORS();

}

/*---------------------------------------------------------------------------*/

void Renderer::setViewport(
		i32					x,
		i32					y,
		u32					width,
		u32					height
	)
{
	if (width > limits.max_viewport_size[0] || height > limits.max_viewport_size[1])
		REPORT_WARNING("Too large viewport for GL to handle. Behaviour from this point on is undefined");

	viewport_offset[0] = x;
	viewport_offset[1] = y;
	viewport_size[0] = width;
	viewport_size[1] = height;

	glViewport(x, y, width, height);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::getViewportDimensions(
		u32					*width,
		u32					*height
	)
{
	*width = viewport_size[0];
	*height = viewport_size[1];
}

/*---------------------------------------------------------------------------*/

void Renderer::setMatrices(
		 Camera				*camera
	)
{
	ASSERT(camera);

	view = camera->getViewMatrix();
	viewInverse = camera->getViewMatrixInverse();

	projection = camera->getProjectionMatrix();
	projectionInverse = camera->getProjectionMatrixInverse();

	viewProjection = camera->getViewProjectionMatrix();
	viewProjectionInverse = camera->getViewProjectionMatrixInverse();

	camera->calculateFrustum(frustum);
}

/*---------------------------------------------------------------------------*/

void Renderer::setClearColor(
		vec4f		color
	)
{
	glClearColor(color.r, color.g, color.b, color.a);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::setClearDepth(
		f32			depth
	)
{
#if defined RC_OPENGL_ES_20
	glClearDepthf(depth);
#elif defined RC_OPENGL_20
	glClearDepth(depth);
#endif

	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::setClearStencil(
		i32			stencil
	)
{
	glClearStencil(stencil);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::clearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::clearDepth()
{
	glDepthMask(true);
	glClear(GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::clearStencil()
{
	glClear(GL_STENCIL_BUFFER_BIT);
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::endFrame()
{
	if (take_screenshot) {
		u32 w, h;
		u8 *pixels;

		Platform::getDisplaySize(&w, &h);
		pixels = (u8 *) malloc(sizeof(u8) * w * h * 3);

		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		BmpWriter::write(screenshot_filename, pixels, w, h, false);

		REPORT("Wrote screenshot \"%s\"", screenshot_filename);

		free(pixels);
	}

	take_screenshot = false;

	GL_CHECK_ERRORS();

}

/*---------------------------------------------------------------------------*/

void Renderer::produceScreenshot(
		char				*filename
	)
{
	if (strlen(filename) >= (FILENAME_MAX - 1)) {
		REPORT_WARNING("Screen shot file name too long");
		return;
	}

	take_screenshot = true;

	strcpy(screenshot_filename, filename);
}

/*---------------------------------------------------------------------------*/

void Renderer::flush()
{
	GL_CHECK_ERRORS();
	glFlush();
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::finish()
{
	GL_CHECK_ERRORS();
	glFinish();
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

void Renderer::ensure()
{
	GL_CHECK_ERRORS();
}

/*---------------------------------------------------------------------------*/

#endif /* RC_OPENGL */


