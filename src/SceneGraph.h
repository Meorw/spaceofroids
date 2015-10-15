
/* TODO: Fult! */
#include "DefaultObjects.h"

#ifndef RC_SCENEGRAPH_H
#define RC_SCENEGRAPH_H

/*---------------------------------------------------------------------------*/

/**
 * Node and resource handling
 *
 * Use the create- and delete-functions from this function to handle all node
 * and resource allocations and deallocations.
 *
 */
class SceneGraph {

/*----------------------------------------------------------[ Functions ]----*/

	public:
		/**
		 * SceneGraph initializer
		 *
		 * This is called by RC during setup.
		 *
		 */
		static void init();

		/**
		 * SceneGraph destroyer
		 *
		 * This is called by RC on tear down.
		 *
		 */
		static void destroy();

		static void registerCommands();

/*==============================================================[ Nodes ]====*/

/*----------------------------------------------------[ Node destroyers ]----*/

		/**
		 * Common destroyer
		 *
		 * Use this function when destroying a node, no matter what type it
		 * has. The children of this node will also be deleted recursively, so
		 * make sure that the node doesn't have any children that shouldn't be
		 * deleted.
		 *
		 * @param node
		 *   The root node to be destroyed
		 *
		 */
		static void deleteNode(
				Node			*node
			);

/*------------------------------------------------------[ Node creators ]----*/

		/**
		 * Create World node
		 *
		 * @return
		 *   A World node on success, otherwise NULL
		 *
		 */
		static World *createWorld(
				const char		*nname
			);


		/* Transformable */

		/**
		 * Create Group node
		 *
		 * @return
		 *   A Group node on success, otherwise NULL
		 *
		 */
		static Group *createGroup(
				const char		*nname
			);

		/**
		 * Create Group node from file
		 *
		 * This creates a Group node with the entire scene structure of the
		 * specified file.
		 *
		 * @param filename
		 *   The file to be loaded. Supported formats are .pwn and .obj. For
		 * .obj files there will only be a single child Geometry node to this
		 * Group node. .pwn files, however, may contain entire scenes.
		 *
		 * @return
		 *   A Group node on success, otherwise NULL
		 *
		 */
		static Group *createGroup(
				const char		*group_name,
				const char		*filename	/* .pwn/.obj file */
			);


		/**
		 * Create Camera node
		 *
		 * @return
		 *   A Camera node on success, otherwise NULL
		 *
		 */
		static Camera *createCamera(
				const char		*nname
			);


		/**
		 * Create Light node
		 *
		 * @return
		 *   A Light node on success, otherwise NULL
		 *
		 */
		static Light *createLight(
				const char		*nname
			);


		/**
		 * Create empty Geometry node
		 *
		 * @return
		 *   A Geometry node on success, otherwise NULL
		 *
		 */
		static Geometry *createGeometry(
				const char		*nname
			);

		/**
		 * Create Geometry node
		 *
		 * @param instantiate_resources
		 *   false if the ownership of the vertex array should be yielded to
		 * this Geometry node; true if the caller should retain its reference
		 * to the vertex array
		 *
		 * @return
		 *   A Geometry node on success, otherwise NULL
		 *
		 */
		static Geometry *createGeometry(
				const char		*nname,

				VertexArray		*vertex_array,

				bool			instantiate_resources = true
			);

		/**
		 * Create Geometry node from file
		 *
		 * This creates a Geometry node from a file that contains one, and only
		 * one mesh. This bypasses the need to load simple meshes through the
		 * Group node object.
		 *
		 * @param filename
		 *   The file to be loaded. Supported formats are .pwn and .obj. The
		 * file loaded must contain exactly one mesh.
		 *
		 * @param instantiate_resources
		 *   false if the ownership of the resources should be yielded to
		 * this Geometry node; true if the caller should retain its reference
		 * to the resources loaded.
		 *
		 * @return
		 *   A Group node on success, otherwise NULL
		 *
		 */
		static Geometry *createGeometry(
				const char		*nname,
				const char		*filename,

				bool			instantiate_resources = true
			);

/*-------------------------------------------------------[ Node getters ]----*/

		/**
		 * Get first node with name
		 *
		 * This retrieves the first node with the specified name. There may be
		 * multiple nodes with the same name, and it's not specified which one
		 * of them you will recieve in that case.
		 *   If you want to retrieve a node that is a child of a certain other
		 * node, then use Node::getChild(char, u32) instead.
		 *
		 * @return
		 *   A Node on success, otherwise NULL
		 *
		 */
		static Node *getNode(
				const char		*name
			);


		static u32 getNodeCount();

		/**
		 * Get all nodes
		 *
		 * Retrieves all existing nodes. Use getNodeCount() to find out how
		 * many nodes that is.
		 *   This method does not allocate any memory. It's up to the caller to
		 * provide an appropriately sized array pointer.
		 *
		 * @param node_list
		 *   Send in an array big enough to hold all Node pointers, and this
		 * method will populate it.
		 *
		 */
		static void getNodes(
				Node			**node_list
			);

		/**
		 * Get nodes with name
		 *
		 * Retrieves a maximum number of nodes with a specified name.
		 *   This method does not allocate any memory. It's up to the caller to
		 * provide an appropriately sized array pointer, though an upper limit
		 * can be set.
		 *
		 * @param name
		 *   Name of the nodes to be fetched
		 *
		 * @param max_nodes
		 *   Only the first max_nodes number of nodes will be fetched. It's not
		 * speficied which these are if there exists more than max_nodes number
		 * of nodes with this name
		 *
		 * @param node_list
		 *   Send in an array big enough to hold max_nodes number of Node
		 * pointers, and this method will populate it.
		 *
		 * @param n
		 *   The number of nodes found and inserted into node_list
		 *
		 */
		static void getNodes(
				const char		*name,
				u32				max_nodes,
				Node			**node_list,
				u32				*n
			);


		/**
		 * Get node count of a specific type
		 *
		 */
		static u32 getNodeCount(
				NodeType_t		type
			);


		/**
		 * Get all nodes of a specific type
		 *
		 * Retrieves all existing nodes of a certain type. Use
		 * getNodeCount(NodeType_t) to find out how many nodes that is.
		 *   This method does not allocate any memory. It's up to the caller to
		 * provide an appropriately sized array pointer.
		 *
		 * @param node_list
		 *   Send in an array big enough to hold all Node pointers, and this
		 * method will populate it.
		 *
		 * @param type
		 *   The type of node that should populate the list
		 *
		 */
		static void getNodes(
				Node			**node_list,
				NodeType_t		type
			);

		/**
		 * Get nodes with name and type
		 *
		 * Retrieves a maximum number of nodes with a specified name and type.
		 *   This method does not allocate any memory. It's up to the caller to
		 * provide an appropriately sized array pointer, though an upper limit
		 * can be set.
		 *
		 * @param name
		 *   Name of the nodes to be fetched
		 *
		 * @param max_nodes
		 *   Only the first max_nodes number of nodes will be fetched. It's not
		 * speficied which these are if there exists more than max_nodes number
		 * of nodes with this name
		 *
		 * @param node_list
		 *   Send in an array big enough to hold max_nodes number of Node
		 * pointers, and this method will populate it.
		 *
		 * @param n
		 *   The number of nodes found and inserted into node_list
		 *
		 * @param type
		 *   The type of node that should populate the list
		 *
		 */
		static void getNodes(
				const char		*name,
				u32				max_nodes,
				Node			**node_list,
				u32				*n,
				NodeType_t		type
			);


/*==========================================================[ Resources ]====*/


/*------------------------------------------------[ Resource destroyers ]----*/


		/**
		 * Common dereferencer.
		 *
		 * Use this to dereference and potentially delete resources of any type.
		 * When a resource is dereferenced its reference counter is decreased.
		 * When there are no more references to a resource, it's actually
		 * deallocated from memory, if the purge level is set to 0.
		 *
		 * @return
		 *   true if the resource was actually deleted from memory, false
		 * otherwise.
		 *
		 */
		static bool dereferenceResource(
				Resource		*resource
			);

		/**
		 * Purge resources.
		 *
		 * Purge all resources with purge level that is;
		 *   0 < purgeLevel <= max_purge_level
		 *
		 *   The resources are not actually deleted unless their reference
		 * counter has hit 0.
		 *
		 */
		static void purgeResources(
				u32				max_purge_level
			);

		/**
		 * Forced purge resources.
		 *
		 * Purge all resources with purge level that is;
		 *   0 < purgeLevel <= max_purge_level
		 *
		 *   The resources _are_ deleted even if their reference counter has
		 * not reached 0. It's important that you delete any Nodes using these
		 * resources _before_ calling this method, or your program is very
		 * likely to crash!
		 *
		 */
		static void forcePurgeResources(
				u32				max_purge_level
			);

/*--------------------------------------------------[ Resource creators ]----*/

		/**
		 * Create a Vertex Array resource
		 *
		 * @param new_array
		 *   Array of vertex attribute structs. It's safe to delete the
		 * incoming array after calling this function.
		 *
		 * @param nstride
		 *   The length of each struct in bytes
		 *
		 * @param n
		 *   Number of structs in the array
		 *
		 * @param usage
		 *   How often the contents will change;
		 *  - USAGE_STATIC		Never
		 *  - USAGE_DYNAMIC		Every few render calls
		 *  - USAGE_STREAM:		Used once and then changed
		 *
		 * @return
		 *   A VertexArray resource on success, otherwise NULL
		 *
		 */
		static VertexArray *createVertexArray(
				const char		*nidentifier,

				void			*new_array,
				u32				nstride,
				u32				n,

				PrimitiveLayout_t layout,

				AttributeUsage_t usage
			);

		/**
		 * Create an empty Vertex Array resource
		 *
		 * This creates an empty Vertex Array resource. This just defines the
		 * vertex array - the actual data is provided later on using
		 * VertexArray::setArray().
		 *
		 * @param nstride
		 *   The length of each struct
		 *
		 * @param usage
		 *   How often the contents will change;
		 *  - USAGE_STATIC		Never
		 *  - USAGE_DYNAMIC		Every few render calls
		 *  - USAGE_STREAM:		Used once and then changed
		 *
		 * @return
		 *   A VertexArray resource on success, otherwise NULL
		 *
		 */
		static VertexArray *createVertexArray(
				const char		*nidentifier,

				u32				nstride,		/* Length of each struct */

				PrimitiveLayout_t layout,

				AttributeUsage_t usage
			);

		/**
		 * Create a Texture resource from file
		 *
		 * This creates a Texture resource from an image file. Which formats
		 * are supported is up to the Platform layer. The safest bet is to use
		 * .png or .bmp images, as these formats are mostly supported.
		 *
		 * @param filename
		 *   The filename of the image (preferably .png or .bmp)
		 *
		 * @param filter
		 *   The filtering method used for magnification _and_ minification;
		 *  - TEXTURE_FILTER_NEAREST
		 *  - TEXTURE_FILTER_BILINEAR
		 *  - TEXTURE_FILTER_TRILINEAR
		 *
		 * @param wrap
		 *   How the texture should wrap
		 *  - TEXTURE_WRAP_CLAMP
		 *  - TEXTURE_WRAP_REPEAT
		 *  - TEXTURE_WRAP_MIRRORED_REPEAT
		 *
		 * @return
		 *   A Texture resource on success, otherwise NULL
		 *
		 */
		static Texture *createTexture(
				const char		*nidentifier,
				const char		*filename,
				bool			generate_mipmaps,
				TextureFilter_t	filter,
				TextureWrap_t	wrap
			);

		/**
		 * Create a Texture resource from pixel array
		 *
		 * Note: Some platforms require width and height to be powers of 2.
		 *
		 * @param pixels
		 *   An array of color/intensity values. May be NULL (if the texture
		 * is intended to be used as a render target)
		 *
		 * @param nformat
		 *   The format of the incoming pixels;
		 *  - TEXTURE_FORMAT_RGBA8
		 *  - TEXTURE_FORMAT_RGB8
		 *  - TEXTURE_FORMAT_GRAY8
		 *
		 * @param filter
		 *   The filtering method used for magnification _and_ minification;
		 *  - TEXTURE_FILTER_NEAREST
		 *  - TEXTURE_FILTER_BILINEAR
		 *  - TEXTURE_FILTER_TRILINEAR
		 *
		 * @param wrap
		 *   How the texture should wrap
		 *  - TEXTURE_WRAP_CLAMP
		 *  - TEXTURE_WRAP_REPEAT
		 *  - TEXTURE_WRAP_MIRRORED_REPEAT
		 *
		 * @return
		 *   A Texture resource on success, otherwise NULL
		 *
		 */
		static Texture *createTexture(
				const char		*nidentifier,
				u8				*pixels,

				TextureFormat_t	nformat,

				u32				nwidth,				/* width in pixels */
				u32				nheight,			/* height in pixels */

				bool			generate_mipmaps,
				TextureFilter_t	filter,
				TextureWrap_t	wrap
			);


		/**
		 * Create a RenderTarget resource
		 *
		 * Note: Some platforms require width and height to be powers of 2.
		 *   This automatically choses to use RGBA8 textures with bilinear
		 * filtering. You're free to hack this if you feel like it.
		 *
		 *
		 * @return
		 *   A RenderTarget resource on success, otherwise NULL
		 *
		 */
		static RenderTarget *createRenderTarget(
				const char		*nidentifier,

				u32				width,
				u32				height,

				u32				n_target_textures,

				bool			depth_buffer,
				bool			stencil_buffer,
				TextureFilter_t	textureFilter
			);

		/**
		 * Create a Cube map
		 *
		 * This creates a Cube map from 6 image files
		 *
		 * @return
		 *   A Cube map resource on success, otherwise NULL
		 *
		 */
		static CubeMap *createCubeMap(
				const char		*nidentifier,

				const char		*nleft,
				const char		*nright,
				const char		*nup,
				const char		*ndown,
				const char		*nfront,
				const char		*nback
			);


		/**
		 * Create a Shader program resource
		 *
		 * @param shader_name
		 *   The name of the shader. It's up to the Renderer to interpret this
		 * name in a way it deems appropriate. The OpenGL ES 2.0 Renderer, for
		 * instance, will look for shader files named "name.gles20.vs" and
		 * "name.gles20.fs", whereas another Renderer may have hard coded code
		 * paths for specific names. Check the documentation for the Renderers
		 * you intend to use for more information on how shaders are handled.
		 *
		 * @param prefix
		 *   A prefix string (if applicable) that can contain things like extra
		 * #defines and such, that alter the behaviour of the shader. Not all
		 * Renderers utilize this parameter. The prefix may also be NULL.
		 *
		 * @return
		 *   A Shader program resource on success, otherwise NULL
		 *
		 */
		static ShaderProgram *createShaderProgram(
				const char		*nidentifier,

				const char		*shader_name,
				char			*prefix
			);


/*---------------------------------------------------[ Resource getters ]----*/

		/**
		 * Fetch resource
		 *
		 * @return
		 *   A Resource on success, otherwise NULL
		 *
		 */
		static Resource *getResource(
				const char		*identifier,
				ResourceType_t	type
			);


		static u32 getResourceCount(
				ResourceType_t	type
			);

		/**
		 * Get list of Resources
		 *
		 * @param list
		 *   An array of appropriate size that will be populated by this
		 * method. Use getResourceCount(ResourceType_t) to get the list size.
		 *
		 */
		static void getResourceList(
				Resource		**list,
				ResourceType_t	type
			);

/*===========================================================================*/

		/**
		 * Debug printout
		 *
		 */
		static void printDebugInfo();


/*------------------------------------------------------------[ Members ]----*/

	protected:
		static ResourceList		*vertexList;
		static ResourceList		*renderTargetList;
		static ResourceList		*textureList;
		static ResourceList		*cubeMapList;
		static ResourceList		*shaderProgramList;

		static NodeList			*nodeList;
};

#endif /* RC_SCENEGRAPH_H */

