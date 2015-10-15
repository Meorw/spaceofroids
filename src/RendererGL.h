
#ifndef RC_RENDERER_GL_H
#define RC_RENDERER_GL_H

static void setCubeTex(
				u8					*pixels,
				u32					w,
				u32					h,
				u32					bpp,
				u32					side
				);

static GLuint loadShader(
				  const char			*parent,
				  char				*prefix,
				  char				*code,
				  ShaderType_t		type
				  );

static void unloadShader(
				  void				*handle
				  );

static void clearAttributes(
							VertexArray			*vertex_array
						);

#endif /* RC_RENDERER_GL_H */

