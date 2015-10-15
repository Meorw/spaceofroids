#ifndef RC_VERTEXARRAY_H
#define RC_VERTEXARRAY_H

typedef enum {

	TRIANGLES = 0,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,

	LINES,
	LINE_LOOP,
	LINE_STRIP,

	INDEX_ARRAY_LAST

} PrimitiveLayout_t;

typedef struct {

	char			*name;
	u32				length;
	u32				offset;

	AttributeType_t	type;
	bool			normalized;

} Attribute_t;


/**
 * Vertex array.
 * Inherits from Resource.
 *
 * This is an array of vertices that is used in conjunction with an index
 * array. The index array points to vertex attributes from this array, and
 * these will be used to construct triangles with accompanying data.
 *
 * A vertex array should at least contain an attribute that specifies the
 * position of a vertex. Other common properties are normals, color and texture
 * coordinates.
 *
 * The vertex array is organized in a array of structs-manner, as opposed to a
 * struct of arrays. The difference is illustrated below with a vertex array
 * containing position, normal and color data.
 *
 * \verbatim
 *                       Vertex attributes
 *
 *   Struct of arrays (not used)       Array of structs
 *   (SOA)                             (AOS)
 *
 *    Vertex[0] Position.x              Vertex[0] Position.x       ^
 *    Vertex[0] Position.y              Vertex[0] Position.y       | S
 *    Vertex[0] Position.z              Vertex[0] Position.z       | t
 *    Vertex[1] Position.x              Vertex[0] Normal.x         | r
 *    Vertex[1] Position.y              Vertex[0] Normal.y         | i
 *    Vertex[1] Position.z              Vertex[0] Normal.z         | d
 *     ...                              Vertex[0] Color.r          | e
 *    Vertex[0] Normal.x                Vertex[0] Color.g          |
 *    Vertex[0] Normal.y                Vertex[0] Color.b          |
 *    Vertex[0] Normal.z                Vertex[0] Color.a          v
 *    Vertex[1] Normal.x                Vertex[1] Position.x
 *    Vertex[1] Normal.y				Vertex[1] Position.y
 *    Vertex[1] Normal.z				Vertex[1] Position.z
 *     ...								Vertex[1] Normal.x
 *     ...								Vertex[1] Normal.y
 *    Vertex[0] Color.r					Vertex[1] Normal.z
 *    Vertex[0] Color.g					Vertex[1] Color.r
 *    Vertex[0] Color.b					Vertex[1] Color.g
 *    Vertex[0] Color.a					Vertex[1] Color.b
 *    Vertex[1] Color.r					Vertex[1] Color.a
 *    Vertex[1] Color.g                  ...
 *    Vertex[1] Color.b
 *    Vertex[1] Color.a
 *     ...
 *
 * \endverbatim
 *
 * In this example, for the AOS the stride = 10 * size(f32), since the data
 * type repeats after every 10 entries and (in this example) we assume that
 * each struct member is a 32-bit float.
 *
 * The offset for the "Normal" attribute in this example is 3, since it's three
 * entries in from the beginning of the array. It's length is 3, since it has
 * three components (x, y, z).
 */
class VertexArray : public Resource {

	friend class SceneGraph;
	friend class Geometry;

/*----------------------------------------------------------[ Functions ]----*/

	private:
		/**
		 * Note: This copies the incoming array, so it's fine to deallocate the
		 * application side memory after the vertex array is constructed.
		 *
		 * @param nstride
		 *   Length of each struct in bytes
		 *
		 * @param n
		 *   Number of structs
		 *
		 * @param layout
		 *   The primitive and layout to use. If an index array is attached,
		 * the layout will apply for that instead of directly on the vertex
		 * attribute data.
		 *
		 */
		VertexArray(
				const char				*nidentifier,

				void					*new_array,
				u32						nstride,
				u32						n,

				PrimitiveLayout_t		layout,

				AttributeUsage_t		nusage
			);

		/**
		 * Create empty VertexArray
		 */
		VertexArray(
				const char				*nidentifier,

				u32						nstride,

				PrimitiveLayout_t		layout,

				AttributeUsage_t		nusage
			);

		virtual ~VertexArray();

	public:

		/**
		 * @param name
		 *   Name of the attribute. This must comply with the vertex attribute
		 * names expected further down in the rendering pipeline (as defined by
		 * the renderer and shader program.)
		 *
		 * @param offset
		 *   Offset, in bytes, to the first entry in the struct/array.
		 *
		 * @param length
		 *   Number of components in this attribute. This is _NOT_ given in
		 * bytes, as the type-argument multiplied with the length reveals the
		 * total byte length.
		 *
		 * @param normalize_int
		 *   This is only valid for int#-variants. If this is true values will
		 * be scaled to the interval [0, 1] in the renderer, otherwise no
		 * scaling will occur.
		 *
		 */
		void setAttribute(
				const char				*name,
				u32						offset,
				u32						length,
				AttributeType_t			type,

				bool					normalize_int = false
			);

		/**
		 * @param n
		 *   The number of indices in the index array. (_NOT_ the number of
		 * triangles you expect to be created). The layout argument specified
		 * in the constructor will be used by this index array. It's indices
		 * point to entries in the attached vertex attribute array.
		 *   It's safe to delete the incoming index array after calling this
		 * function.
		 */
		void setIndexArray(
				void					*new_array,
				u32						type_size,
				u32						n
			);

		void setArray(
				void					*new_array,
				u32						n
			);

		u32 getLength();

		u32 getStride();

		u32 getAttributeCount();

		Attribute_t *getAttribute(
				u32						i
			);

		Attribute_t *getAttribute(
				char					*name
			);

		PrimitiveLayout_t getPrimitiveLayout();

		u32 getIndexTypeSize();

		u32 getIndexLength();

		AttributeUsage_t getUsage();

		AttributeUsage_t getIndexUsage();

		/* If there are no normals in the vertex array, they can be generated
		 * by calling this function. For a more exact result, however, it is
		 * better to provide normals with the mesh, if possible.
		 *
		 * Normals are generated with regard to their adjacent triangles, so an
		 * index array is needed to specify which those triangles are.
		 */
//		void generateNormals();

		void *getHandle();

		void setHandle(
				void				*nhandle
			);

	protected:

		void getBoundingBox(
				AABox				*target
			);

	private:

		/* Static helper functions */
		static void setVertexArray(
				VertexArray				**target_vertex_array,
				VertexArray				*new_vertex_array,

				bool					instantiate_resource = true
			);

#ifdef DEBUG
	protected:
		void print(
				FILE	*f
			);
#endif /* DEBUG */

/*------------------------------------------------------------[ Members ]----*/

	private:

		bool				has_bounds;
		AABox				bounds;

		void				*varray;
		u32					n_elements;
		u32					stride;

		AttributeUsage_t	usage;
		bool				holds_array;

		/* Attributes */
		u32					n_attribs;
		Attribute_t			attrib[RC_MAX_ATTRIBS];

		/* Index array */
		PrimitiveLayout_t	primitiveLayout;
		void				*iarray;
		u32					indexArrayTypeSize; /* Should be either 1, 2 or 4 */
		u32					n_indices;
		bool				holds_index_array;

		AttributeUsage_t	index_usage;

		void				*handle;

};

#endif /* RC_VERTEXARRAY_H */

