
#ifndef RC_TEXTURE_H
#define RC_TEXTURE_H

/*---------------------------------------------------------------------------*/

/**
 * Texture.
 */
class Texture : public Resource {

	friend class SceneGraph;
	
	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

	friend class Uniform;

/*----------------------------------------------------------[ Functions ]----*/

	private:
		Texture(
				const char			*nidentifier,

				TextureFormat_t		nformat,

				u32					nwidth,
				u32					nheight,

				TextureFilter_t		filter,
				TextureWrap_t		wrap
			);

		Texture(
				const char			*nidentifier,
				const char			*filename,

				bool				generate_mipmaps,
				TextureFilter_t		filter,
				TextureWrap_t		wrap
			);

		/**
		 * @param pixels
		 *   Color values [0, 255] if 8-bit type, [0, 65535] if 16-bit type.
		 *
		 */
		Texture(
				const char			*nidentifier,
				u8					*pixels,

				TextureFormat_t		nformat,

				u32					nwidth,
				u32					nheight,

				bool				generate_mipmaps,

				TextureFilter_t		filter,
				TextureWrap_t		wrap
			);

		~Texture();

	public:
		bool hasMipMaps() { return hasMipmaps; }
	
		/** WARNING: This is untested! */
		void generateMipMaps();

		u32 getWidth() { return width; }

		u32 getHeight() { return height; }

		TextureFormat_t getFormat() { return format; }



	/* Static helper functions */

		static void setTexture(
				Texture				**target_tex,
				Texture				*new_tex,
				bool				instantiate_resource = true
			);

		static void setTexture(
				Texture				**target_tex,
				const char			*nidentifier,
				const char			*filename
			);

	public:

		void *getHandle();

		void setHandle(
				void				*nhandle
			);

#ifdef DEBUG
	protected:
		void print(
				FILE	*f
			);
#endif /* DEBUG */

/*------------------------------------------------------------[ Members ]----*/

	private:
		TextureFormat_t		format;
		u32					width;
		u32					height;

		bool				hasMipmaps;

		void				*handle;


};

#endif /* RC_TEXTURE_H */

