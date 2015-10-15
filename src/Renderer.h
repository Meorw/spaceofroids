#ifndef RC_RENDERER_H
#define RC_RENDERER_H

#define RC_MAX_TEXTURE_SLOTS			8
#define RC_MAX_TEXTURE_NAME				256

/*==================================================[ Renderer settings ]====*/

/*----------------------------------------------------[ Shader settings ]----*/

/** Available shader program uniform types */
typedef enum {

	UNIFORM_INT32 = 0,
	UNIFORM_FLOAT32,

	UNIFORM_TYPE_LAST

} UniformType_t;

/** Available vertex attribute types */
typedef enum {

	ATTRIB_INT8 = 0,
	ATTRIB_UINT8,
	ATTRIB_INT16,
	ATTRIB_UINT16,
	ATTRIB_INT32,
	ATTRIB_UINT32,
	ATTRIB_FLOAT32,
	ATTRIB_FLOAT64,

	ATTRIB_TYPE_LAST

} AttributeType_t;

/*---------------------------------------------------[ Vertex attribute ]----*/

/**
 * VertexArray usage modes
 *  - USAGE_STATIC - Never changed
 *  - USAGE_DYNAMIC -  Seldom changed (every few frames or so)
 *  - USAGE_STREAM - Constantly changed
 */
typedef enum {

	USAGE_STATIC = 0,
	USAGE_DYNAMIC,
	USAGE_STREAM,

	USAGE_LAST

} AttributeUsage_t;

/*---------------------------------------------------[ Texture settings ]----*/

/** Texture formats. Note: Some Renderers don't support 16-bit formats. */
typedef enum {

	TEXTURE_FORMAT_RGBA8 = 4,
	TEXTURE_FORMAT_RGB8 = 3,
	TEXTURE_FORMAT_GRAY8 = 1,
	TEXTURE_FORMAT_RGBA16 = 8,
	TEXTURE_FORMAT_RGB16 = 6,
	TEXTURE_FORMAT_GRAY16 = 2,
	TEXTURE_FORMAT_DEPTH24 = 10,

	TEXTURE_FORMAT_LAST

} TextureFormat_t;

/** Texture filtering method */
typedef enum {

	TEXTURE_FILTER_NEAREST = 0,
	TEXTURE_FILTER_BILINEAR,
	TEXTURE_FILTER_TRILINEAR,

	TEXTURE_FILTER_LAST

} TextureFilter_t;

/** Texture wrapping strategy */
typedef enum {

	TEXTURE_WRAP_CLAMP = 0,
	TEXTURE_WRAP_REPEAT,
	TEXTURE_WRAP_MIRRORED_REPEAT,

	TEXTURE_WRAP_LAST

} TextureWrap_t;

/*-----------------------------------------------------[ State settings ]----*/

/** Comparation strategy for depth and stencil */
typedef enum {

	CMP_NEVER = 0,
	CMP_LESS,
	CMP_EQUAL,
	CMP_LEQUAL,
	CMP_GREATER,
	CMP_NOTEQUAL,
	CMP_GEQUAL,
	CMP_ALWAYS,

	CMP_LAST

} StateCompare_t;

/** Source blending methods */
typedef enum {

	SB_ZERO = 0,
	SB_ONE,
	SB_SRC_COLOR,
	SB_ONE_MINUS_SRC_COLOR,
	SB_SRC_ALPHA,
	SB_ONE_MINUS_SRC_ALPHA,
	SB_DST_ALPHA,
	SB_ONE_MINUS_DST_ALPHA,

	SB_LAST

} StateSrcBlend_t;

/** Destination blending methods */
typedef enum {

	DB_ZERO = 0,
	DB_ONE,
	DB_DST_COLOR,
	DB_ONE_MINUS_DST_COLOR,
	DB_SRC_ALPHA_SATURATE,
	DB_ONE_MINUS_SRC_ALPHA,
	DB_DST_ALPHA,
	DB_ONE_MINUS_DST_ALPHA,

	DB_LAST

} StateDstBlend_t;


/** Culling modes */
typedef enum {

	CULL_FRONT = 0,
	CULL_BACK,
	CULL_FRONT_AND_BACK,

	CULL_LAST

} StateCulling_t;

/*---------------------------------------------------------------------------*/

typedef enum {
	
	SHADER_TYPE_VERTEX = 0,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_FRAGMENT,
	
	SHADER_TYPE_LAST
	
} ShaderType_t;


/** Renderer specific limits */
typedef struct {

	u32					max_render_targets;

	u32					max_vertices;
	u32					max_indices;

	u32					max_index_type_size;

	u32					max_texture_size;
	u32					max_cubemap_size;

	u32					max_viewport_size[2];

	u32					max_vertex_attributes;
	u32					max_uniforms;

} RendererLimits_t;

/*===========================================================================*/



/* Forward declaration */
class Uniform;

/*===========================================================================*/

class Renderer {

	public:

		static void init();
	
		static void destroy();

	public:

/*----------------------------------------------------------[ Rendering ]----*/

		static void enqueue(
				Geometry			*mesh
			);

		static void renderQueued();

		/** This renders the geometry node. */
		static void render(
				Geometry			*mesh
			);

		/**
		 * Called at the end of each set of renderings to draw the resulting
		 * frame buffer.
		 */
		static void swapBuffers();

/*------------------------------------------------------------[ Shading ]----*/

		//static void setShaderProgram(
		//		ShaderProgram		*shader_program
		//	);

		static bool loadShaderProgram(
				ShaderProgram		*shader_program,
				char				*desc,
				char				*prefix
			);

		static void unloadShaderProgram(
				ShaderProgram		*shader_program
			);

		/* Shader attrib. */
		static void setAttributes(
				VertexArray			*vertex_array
			);

		/* Shader uniforms */
		static void setUniform(
				Uniform				*uniform
			);

		static bool getShaderHasUniform(
				ShaderProgram		*shader_program,
				const char			*name
			);

/*-----------------------------------------------------------[ Textures ]----*/

		static bool loadRenderTarget(
				RenderTarget		*target,
				u32					n_textures,
				bool				include_depth_buffer,
				bool				include_stencil_buffer
			);

		static void unloadRenderTarget(
				RenderTarget		*target
			);

		static bool shareDepthBuffer(
				RenderTarget		*dst,
				RenderTarget		*src
			);

		static void loadTexture(
				Texture				*texture,
				u8					*pixels,
				u32					width,
				u32					height,
				TextureFormat_t		format,
				bool				generate_mipmaps,
				TextureFilter_t		filter,
				TextureWrap_t		wrap
			);

		static void unloadTexture(
				Texture				*texture
			);

		static void generateMipmaps(
				Texture				*texture
			);
	
		static void loadCubeMap(
				CubeMap				*cubemap,
				u8					**pixels,
				u32					width,
				u32					height,
				u32					bpp
			);

		static void unloadCubeMap(
				CubeMap				*cubemap
			);

		static void loadVertexArray(
				VertexArray			*vertex_array,
				void				*new_array,
				u32					n,
				AttributeUsage_t	usage
			);

		static void unloadVertexArray(
				VertexArray			*vertex_array
			);

		static void reloadVertexArray(
				VertexArray			*vertex_array,
				void				*new_array,
				u32					n
			);

		static void loadIndexArray(
				VertexArray			*vertex_array,
				void				*new_array,
				u32					n,
				AttributeUsage_t	usage
			);

		static void unloadIndexArray(
				VertexArray			*vertex_array
			);

		static void reloadIndexArray(
				VertexArray			*vertex_array,
				void				*new_array,
				u32					n
			);

/*-------------------------------------------------------[ Render state ]----*/

		static void setRenderState(
				RenderState			*render_state
			);

		static void setViewport(
				i32					x,
				i32					y,
				u32					width,
				u32					height
			);

		static void getViewportDimensions(
				u32					*width,
				u32					*height
			);

		static void setRenderTarget(
				RenderTarget		*target
			);

		static void setClearColor(
				vec4f		color
			);

		static void setClearDepth(
				f32			depth
			);

		static void setClearStencil(
				i32			stencil
			);

		static void clearColor();

		static void clearDepth();

		static void clearStencil();

		static void flush();

		static void finish();

/*-----------------------------------------------------------[ Matrices ]----*/

		static void setMatrices(
				Camera				*camera
			);

/*---------------------------------------------------------------------------*/

		/** Returns a _copy_ of the Renderer limits */
		RendererLimits_t getLimits() { return limits; }


		static void endFrame();

		static void produceScreenshot(
				char				*filename
			);

		static void ensure();

	private:
	
#ifdef RC_OPENGL
#include "RendererGL.h"
#endif
	
	
/*---------------------------------------------------------------------------*/

	protected:

		static ShaderProgram	*active_shader_program;

		static RenderState		*active_render_state;

			/* Matrices */

		/* ViewProjection matrix */
		static mat4f			world;
		static mat4f			worldInverse;

		/* View matrix */
		static mat4f			view;
		static mat4f			viewInverse;

		/* Projection matrix */
		static mat4f			projection;
		static mat4f			projectionInverse;

		/* ViewProjection matrix */
		static mat4f			viewProjection;
		static mat4f			viewProjectionInverse;

		static i32				viewport_offset[2];
		static u32				viewport_size[2];

		static bool				take_screenshot;
		static char				screenshot_filename[FILENAME_MAX - 1];


		static RendererLimits_t	limits;

//		RenderQueue				*


};

#endif /* RC_RENDERER_H */

