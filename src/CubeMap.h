
#ifndef RC_CUBEMAP_H
#define RC_CUBEMAP_H

/**
 * CubeMap.
 * Inherits from Resource.
 *
 * This is a cube environment map that can be used for nice reflections in your
 * shaders. It uses 6 faces organized in a cube, each with its own texture.
 */
class CubeMap : public Resource {

	friend class SceneGraph;
	friend class Texture;

	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

	friend class Uniform;

/*----------------------------------------------------------[ Functions ]----*/

	private:
		CubeMap(
				const char			*nidentifier,

				/* Texture filenames */
				const char			*nleft,
				const char			*nright,
				const char			*nup,
				const char			*ndown,
				const char			*nfront,
				const char			*nback
			);

		~CubeMap();

	public:

	/* Static helper functions */

		static void setCubeMap(
				CubeMap				**target_map,
				CubeMap				*new_map,
				bool				instantiate_resource = true
			);

	protected:

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
		void				*handle;

};

#endif /* RC_CUBEMAP_H */

