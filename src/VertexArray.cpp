#include "RenderChimp.h"
#include "VertexArray.h"

/*---------------------------------------------------------------------------*/

VertexArray::VertexArray(
		const char				*nidentifier,

		void					*new_array,
		u32						nstride,
		u32						n,

		PrimitiveLayout_t		layout,

		AttributeUsage_t		nusage

	) : Resource(nidentifier, NO_PURGE, RESOURCE_VERTEX_ARRAY)
{
	handle = 0;
	n_attribs = 0;
	stride = nstride;

	indexArrayTypeSize = 0;

	primitiveLayout = layout;

	usage = nusage;
	n_elements = n;

	holds_index_array = false;
	holds_array = false;

	iarray = 0;
	varray = 0;

	has_bounds = false;

	setArray(new_array, n);
}

/*---------------------------------------------------------------------------*/

VertexArray::VertexArray(
		const char				*nidentifier,
		u32						nstride,
		PrimitiveLayout_t		layout,
		AttributeUsage_t		nusage

	) : Resource(nidentifier, NO_PURGE, RESOURCE_VERTEX_ARRAY)
{
	handle = 0;
	n_attribs = 0;
	stride = nstride;

	indexArrayTypeSize = 0;

	primitiveLayout = layout;

	usage = nusage;
	n_elements = 0;

	holds_index_array = false;
	holds_array = false;

	has_bounds = false;

	iarray = 0;
	varray = 0;
}

/*---------------------------------------------------------------------------*/

VertexArray::~VertexArray()
{
	u32 i;

	for (i = 0; i < n_attribs; i++)
		free(attrib[i].name);

	Renderer::unloadVertexArray(this);

	if (iarray)
		free(iarray);

	if (varray)
		free(varray);
}

/*---------------------------------------------------------------------------*/

void VertexArray::setIndexArray(
		void					*new_array,
		u32						type_size,
		u32						n
	)
{
//#ifdef RC_IPHONE
	u16 *small_array = 0;
//#endif
	
	if (n == 0 || !new_array || type_size == 0) {
		/* TODO: Remove index array */

		holds_index_array = false;

		return;
	}

	ASSERT(type_size == 1 || type_size == 2 || type_size == 4);

	n_indices = n;

	index_usage = USAGE_STATIC;
	
#ifdef RC_IPHONE
	if (type_size == 4) {
#else
	if (type_size == 4 && n < 65535) {
#endif
		u32 *ii = (u32 *) new_array;
		u32 i;
		small_array = (u16 *) malloc(sizeof(u16) * n);
		for (i = 0; i < n; i++) {
			small_array[i] = (u16) ii[i];
		}
		
		type_size = 2;
		new_array = small_array;
	}
	
	indexArrayTypeSize = type_size;

	if (!holds_index_array) {
		Renderer::loadIndexArray(this, new_array, n, index_usage);
	} else {
		Renderer::reloadIndexArray(this, new_array, n);
	}

	if (iarray)
		free(iarray);

	iarray = malloc(n * type_size);
	memcpy(iarray, new_array, n * type_size);

#ifdef RC_IPHONE
	if (small_array)
		free(small_array);
#endif
	
	holds_index_array = true;
}

/*---------------------------------------------------------------------------*/

void VertexArray::setAttribute(
		const char				*name,
		u32						offset,
		u32						length,
		AttributeType_t			type,
		bool					normalized
	)
{
	i32 i;

	if (length == 0)
		return;

	if (n_attribs >= RC_MAX_ATTRIBS) {
		REPORT_WARNING("Too many attributes added for vertex array resource %s", identifier);
		return;
	}

	if (offset >= stride) {
		REPORT_WARNING("The offset is greater or equal to stride (%s)", identifier);
		return;
	}

	for (i = 0; i < (i32) n_attribs; i++) {
		if (strcmp(name, attrib[i].name) == 0) {
			REPORT_WARNING("Attribute %s already exists", name);
			return;
		}
	}

	i = n_attribs - 1;

	while (i >= 0 && offset < attrib[i].offset) {
		attrib[i + 1].offset = attrib[i].offset;
		attrib[i + 1].length = attrib[i].length;
		attrib[i + 1].name = attrib[i].name;
		attrib[i + 1].type = attrib[i].type;
		attrib[i + 1].normalized = attrib[i].normalized;
		i--;
	}

	i++;

	attrib[i].offset = offset;
	attrib[i].length = length;
	attrib[i].name = duplicateString(name);
	attrib[i].type = type;
	attrib[i].normalized = normalized;

	n_attribs++;
}

/*---------------------------------------------------------------------------*/

void VertexArray::setArray(
		void					*new_array,
		u32						n				/* Number of structs */
	)
{
	if (n == 0)
		REPORT_WARNING("Number of structs set to 0");

	n_elements = n;

	if (holds_array) {
		Renderer::reloadVertexArray(this, new_array, n);
	} else {
		Renderer::loadVertexArray(this, new_array, n, usage);
		holds_array = true;
	}

	has_bounds = false;

	if (varray)
		free(varray);

	varray = malloc(n * stride);
	memcpy(varray, new_array, n * stride);
}

/*---------------------------------------------------------------------------*/

u32 VertexArray::getLength()
{
	return n_elements;
}

/*---------------------------------------------------------------------------*/

u32 VertexArray::getStride()
{
	return stride;
}

/*---------------------------------------------------------------------------*/

PrimitiveLayout_t VertexArray::getPrimitiveLayout()
{
	return primitiveLayout;
}

/*---------------------------------------------------------------------------*/

void VertexArray::setVertexArray(
		VertexArray				**target_vertex_array,
		VertexArray				*new_vertex_array,

		bool					instantiate_resource
	)
{
	(*target_vertex_array)->has_bounds = false;

	if (*target_vertex_array)
		SceneGraph::dereferenceResource(*target_vertex_array);

	if (instantiate_resource) {
		*target_vertex_array = (VertexArray *) new_vertex_array->instantiate();
	} else {
		*target_vertex_array = new_vertex_array;
	}
}

/*---------------------------------------------------------------------------*/

#ifdef DEBUG

void VertexArray::print(
		FILE		*f
	)
{

#ifdef RC_LOG_TO_FILE
	#define prnt0(a)		fprintf(f, a)
	#define prnt1(a, b)		fprintf(f, a, b)
#else /* RC_LOG_TO_FILE */
	#define prnt0(a)		printf(a)
	#define prnt1(a, b)		printf(a, b)
#endif /* RC_LOG_TO_FILE */

	prnt1("    [ N ELEMS  ][ %u ]\n", n_elements);
	prnt1("    [ STRIDE   ][ %u ]\n", stride);
	prnt1("    [ N ATTR.  ][ %u ]\n", n_attribs);

}

#endif /* DEBUG */

/*---------------------------------------------------------------------------*/

u32 VertexArray::getAttributeCount()
{
	return n_attribs;
}

/*---------------------------------------------------------------------------*/

Attribute_t *VertexArray::getAttribute(
		u32						i
	)
{
	return &attrib[i];
}

/*---------------------------------------------------------------------------*/

Attribute_t *VertexArray::getAttribute(
		char					*name
	)
{
	u32 i;

	for (i = 0; i < n_attribs; i++) {

		if (strcmp(attrib[i].name, name) == 0)
			return &attrib[i];

	}

	REPORT_WARNING("No attribute by the name \"%s\" found", name);

	return 0;
}

/*---------------------------------------------------------------------------*/

u32 VertexArray::getIndexTypeSize()
{
	return indexArrayTypeSize;
}

/*---------------------------------------------------------------------------*/

u32 VertexArray::getIndexLength()
{
	return n_indices;
}

/*---------------------------------------------------------------------------*/

AttributeUsage_t VertexArray::getUsage()
{
	return usage;
}

/*---------------------------------------------------------------------------*/

AttributeUsage_t VertexArray::getIndexUsage()
{
	return index_usage;
}

/*---------------------------------------------------------------------------*/

void *VertexArray::getHandle()
{
	return handle;
}

/*---------------------------------------------------------------------------*/

void VertexArray::setHandle(
		void				*nhandle
	)
{
	handle = nhandle;
}

/*---------------------------------------------------------------------------*/

void VertexArray::getBoundingBox(
		AABox				*target
	)
{
	u8 *va;
	u32 i;
	Attribute_t *a;
	AABox box;

	if (has_bounds) {
		*target = bounds;
		return;
	}

	box.minCorner = vec3f(0.0f, 0.0f, 0.0f);
	box.maxCorner = vec3f(0.0f, 0.0f, 0.0f);

	a = getAttribute("Vertex");

	if (!a) {
		REPORT_WARNING("No \"Vertex\" attribute to extract bounding box from");
		*target = box;
		return;
	}

	if (a->type != ATTRIB_FLOAT32) {
		REPORT_WARNING("The \"Vertex\" attribute must be a 32 bit float");
		*target = box;
		return;
	}

	if (!varray) {
		REPORT_WARNING("No vertex array attached");
		*target = box;
		return;
	}

	if (n_elements == 0) {
		REPORT_WARNING("Vertex array contains no elements");
		*target = box;
		return;
	}

	bounds.minCorner = fMax;
	bounds.maxCorner = -fMax;

	va = (u8 *) varray;
	va += a->offset;

	for (i = 0; i < n_elements; i++) {
		f32 *vtx;
		vec3f pt;

		vtx = (f32 *) va;
		pt = vec3f(vtx[0], vtx[1], vtx[2]);
		bounds.include(pt);

		va += stride;
	}

	has_bounds = true;

	*target = bounds;
}

/*---------------------------------------------------------------------------*/

