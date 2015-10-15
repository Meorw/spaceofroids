
#ifndef RC_SHADERPROGRAM_H
#define RC_SHADERPROGRAM_H

#define RC_MAX_SHADERS				4

/**
 * ShaderProgram.
 */
class ShaderProgram : public Resource {

	friend class SceneGraph;
	friend class Geometry;

	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		ShaderProgram(
				const char			*nidentifier,

				const char			*description,
				char				*prefix
			);

		~ShaderProgram();

	public:

		static void registerCommands();

		/* Uniform stuff */

		void setScalar(
				const char			*name,
				f32					value,
				UniformType_t		type
			);

		void setScalar(
				const char			*name,
				i32					value,
				UniformType_t		type
			);

		void setScalarArray(
				const char			*name,
				void				*list,
				u32					n_values,
				UniformType_t		type
			);


		void setVector(
				const char			*name,
				void				*vecptr,	/* Pointer to vec#$ */
				u32					dimension,	/* # = dimension */
				UniformType_t		type		/* $ = i or f */
			);

		void setVectorArray(
				const char			*name,
				void				*list,
				u32					n_values,
				u32					dimension,
				UniformType_t		type
			);

		void setMatrix(
				const char			*name,
				void				*matptr,	/* Pointer to mat#f */
				u32					dimension	/* # = dimension */
			);

		void setMatrixArray(
				const char			*name,
				void				*list,
				u32					n_values,
				u32					dimension
			);
		/* Note: Integer matrices are not supported. Floats only! */



		void setTexture(
				const char			*name,
				Texture				*tex
			);

		void setCubeMap(
				const char			*name,
				CubeMap				*cubemap
			);


		RenderState *getRenderState();



		Uniform *getUniform();



		bool hasUniform(
				const char			*name
			);

		void recompile();


	//protected:

		void *getHandle();
	protected:
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

	public:
		RenderState		renderState;

	private:
		char			*shader_description;
		char			*shader_prefix;

		Uniform			uniform;

		void			*handle;

};

#endif /* RC_SHADERPROGRAM_H */

