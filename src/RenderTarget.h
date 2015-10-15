
#ifndef RC_RENDER_TARGET_H
#define RC_RENDER_TARGET_H

/*---------------------------------------------------------------------------*/

/**
 * RenderTarget.
 */
class RenderTarget : public Resource {

	friend class SceneGraph;
	
	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

	friend class Uniform;

/*----------------------------------------------------------[ Functions ]----*/

	private:
		RenderTarget(
				const char			*nidentifier,

				u32					nwidth,
				u32					nheight,

				u32					n_targets,

				TextureFilter_t		filter,
				TextureWrap_t		wrap,

				bool				include_depth_buffer,
				bool				include_stencil_buffer
			);

		~RenderTarget();

	public:

		Texture *getTexture(
				u32					target_index
			);

		u32 getWidth() { return width; }

		u32 getHeight() { return height; }

		TextureFormat_t getFormat() { return format; }

		void shareDepthBuffer(
				RenderTarget		*target
			);

	public:

		void *getHandle();

		void setHandle(
				void				*nhandle
			);

/*------------------------------------------------------------[ Members ]----*/

	private:
		Texture				*textures[RC_MAX_RENDER_TARGET_TEXTURES];
		u32					n_textures;

		TextureFormat_t		format;
		u32					width;
		u32					height;
	
		void				*handle;


};

#endif /* RC_RENDER_TARGET_H */

