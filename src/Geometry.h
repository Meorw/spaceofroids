
#ifndef RC_GEOMETRY_H
#define RC_GEOMETRY_H

/**
 * Defines a Geometry node.
 */
class Geometry : public Transformable {
	friend class SceneGraph;

	friend class Renderer;
#ifdef RC_OPENGL
	friend class RendererGL;
#endif

/*----------------------------------------------------------[ Functions ]----*/

	protected:
		Geometry(
				const char		*nname
			);

		Geometry(
				const char		*nname,

				VertexArray		*vertex_array,

				bool			instantiate_resources = true
			);

		Geometry(
				const char		*nname,

				const char		*filename
			);

		~Geometry();

	public:

		static void registerCommands();

		/**
		 * Render this node only 
		 *
		 */
		void render();

		void setVertexArray(
				VertexArray		*vertex_array,

				bool			instantiate_resource = true
			);

		VertexArray *getVertexArray();


		void setShaderProgram(
				ShaderProgram	*shader_program,

				bool			instantiate_resource = true
			);

		ShaderProgram *getShaderProgram();

		void setShaderProgram(
				ShaderProgram	*shader_program,

				char			*render_pass,

				bool			instantiate_resource = true
			);

		ShaderProgram *getShaderProgram(
				char			*render_pass
			);


		void getWorldBoundingBox(
				AABox			*target
			);

		void getLocalBoundingBox(
				AABox			*target
			);



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





		Uniform *getUniform();


		void logInfo();


		static void init();

	/* Render pass functions */

		static void setRenderPass(
				char		*pass_name
			);

		static void addRenderPass(
				char		*pass_name
			);

		static void removeRenderPass(
				char		*pass_name
			);

		static u32 getCurrentRenderPass();

		static u32 getRenderPassIndex(
				char		*pass_name
			);

	protected:

		void drawSelf();

		Node *duplicateSelf();

/*------------------------------------------------------------[ Members ]----*/

	protected:
		VertexArray				*vertexArray;

		ShaderProgram			*shaderProgram[RC_GEOMETRY_MAX_PASSES];

		/* Note: This applies to ALL paths */
		Uniform					uniform;

		static char				*render_pass[RC_GEOMETRY_MAX_PASSES];
		static u32				curr_render_pass;


};


#endif /* RC_GEOMETRY_H */

