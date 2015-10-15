#include "RenderChimp.h"
#include "SceneParser.h"
#include "Geometry.h"

/*---------------------------------------------------------------------------*/

char			*Geometry::render_pass[RC_GEOMETRY_MAX_PASSES];
u32				Geometry::curr_render_pass;

/*---------------------------------------------------------------------------*/

void Geometry::init()
{
	memset(render_pass, 0, RC_GEOMETRY_MAX_PASSES * sizeof(char *));
	addRenderPass("Default");
	setRenderPass("Default");
}

/*---------------------------------------------------------------------------*/

void Geometry::setRenderPass(
		char		*pass_name
	)
{
	u32 i;

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {
		if (render_pass[i] && (strcmp(pass_name, render_pass[i]) == 0)) {
			curr_render_pass = i;
			return;
		}
	}
}

/*---------------------------------------------------------------------------*/

void Geometry::addRenderPass(
		char		*pass_name
	)
{
	u32 i;

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {
		if (render_pass[i] && (strcmp(pass_name, render_pass[i]) == 0)) {
			return;
		} else if (!render_pass[i]) {
			render_pass[i] = duplicateString(pass_name);
			return;
		}
	}

	REPORT_WARNING("No render pass slots available (maximum is %d)", RC_GEOMETRY_MAX_PASSES);
}

/*---------------------------------------------------------------------------*/

void Geometry::removeRenderPass(
		char		*path_name
	)
{
	u32 i;

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {
		if (render_pass[i] && (strcmp(path_name, render_pass[i]) == 0)) {
			free(render_pass[i]);
			render_pass[i] = 0;
			return;
		}
	}
}

/*---------------------------------------------------------------------------*/

u32 Geometry::getCurrentRenderPass()
{
	return curr_render_pass;
}

/*---------------------------------------------------------------------------*/

u32 Geometry::getRenderPassIndex(
		char		*pass_name
	)
{
	u32 i;

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {

		if (render_pass[i] && (strcmp(pass_name, render_pass[i]) == 0))
			return i;

	}

	REPORT_WARNING("No render pass name \"%s\". Returning 0", pass_name);

	return 0;
}

/*---------------------------------------------------------------------------*/

Geometry::Geometry(
		const char		*nname
	) : Transformable(nname, NODE_GEOMETRY)
{
	vertexArray = 0;
	memset(shaderProgram, 0, sizeof(ShaderProgram *) * RC_GEOMETRY_MAX_PASSES);
}

/*---------------------------------------------------------------------------*/

Geometry::Geometry(
		const char		*nname,

		VertexArray		*vertex_array,

		bool			instantiate_resources
	) : Transformable(nname, NODE_GEOMETRY)
{
	memset(shaderProgram, 0, sizeof(ShaderProgram *) * RC_GEOMETRY_MAX_PASSES);

	vertexArray = vertex_array;
}

/*---------------------------------------------------------------------------*/

Geometry::Geometry(
		const char		*nname,
		const char		*filename
	) : Transformable(nname, NODE_GEOMETRY)
{
	char fnbuf[FILENAME_MAX];
	const char *fnprefix = RC_RESOURCE_PATH;

	vertexArray = 0;
	memset(shaderProgram, 0, sizeof(ShaderProgram *) * RC_GEOMETRY_MAX_PASSES);

	if (strlen(filename) + strlen(fnprefix) >= (FILENAME_MAX - 1)) {
		REPORT_WARNING("Scene filename too long (%s)", filename);
		broken = true;
		return;
	}

	strcpy(fnbuf, fnprefix);
	strcat(fnbuf, filename);

	ParseScene(this, fnbuf);
}

/*---------------------------------------------------------------------------*/

Geometry::~Geometry()
{
	u32 i;

	if (vertexArray)
		SceneGraph::dereferenceResource(vertexArray);

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {

		if (shaderProgram[i])
			SceneGraph::dereferenceResource(shaderProgram[i]);

	}

}

/*---------------------------------------------------------------------------*/

void Geometry::render()
{
	drawSelf();
}

/*---------------------------------------------------------------------------*/

void Geometry::setVertexArray(
		VertexArray		*vertex_array,

		bool			instantiate_resource
	)
{
	if (vertexArray)
		SceneGraph::dereferenceResource(vertexArray);

	if (instantiate_resource) {
		vertexArray = (VertexArray *) vertex_array->instantiate();
	} else {
		vertexArray = vertex_array;
	}

//	VertexArray::setVertexArray(&vertexArray[getCurrentRenderPath()], vertex_array,
//		instantiate_resource);
}

/*---------------------------------------------------------------------------*/

void Geometry::setShaderProgram(
		ShaderProgram	*shader_program,

		bool			instantiate_resource
	)
{
	u32 rp;
	rp = getCurrentRenderPass();

	if (shaderProgram[rp])
		SceneGraph::dereferenceResource(shaderProgram[rp]);

	if (instantiate_resource) {
		shaderProgram[rp] = (ShaderProgram *) shader_program->instantiate();
	} else {
		shaderProgram[rp] = shader_program;
	}
}

/*---------------------------------------------------------------------------*/

void Geometry::setShaderProgram(
		ShaderProgram	*shader_program,

		char			*render_pass,

		bool			instantiate_resource
	)
{
	u32 rp;

	rp = getRenderPassIndex(render_pass);

	if (shaderProgram[rp])
		SceneGraph::dereferenceResource(shaderProgram[rp]);

	if (instantiate_resource) {
		shaderProgram[rp] = (ShaderProgram *) shader_program->instantiate();
	} else {
		shaderProgram[rp] = shader_program;
	}
}

/*---------------------------------------------------------------------------*/

VertexArray *Geometry::getVertexArray()
{
	return vertexArray;
}

/*---------------------------------------------------------------------------*/

ShaderProgram *Geometry::getShaderProgram()
{
	return shaderProgram[getCurrentRenderPass()];
}

/*---------------------------------------------------------------------------*/

ShaderProgram *Geometry::getShaderProgram(
		char			*render_pass
	)
{
	u32 rp;
	rp = getRenderPassIndex(render_pass);
	return shaderProgram[rp];
}

/*---------------------------------------------------------------------------*/

Uniform *Geometry::getUniform()
{
	return &uniform;
}

/*---------------------------------------------------------------------------*/

void Geometry::drawSelf()
{
	Renderer::render(this);
}

/*---------------------------------------------------------------------------*/

void Geometry::setScalar(
		const char			*name,
		f32					value,
		UniformType_t		type
	)
{
	uniform.setScalar(name, value, type);
}

/*---------------------------------------------------------------------------*/

void Geometry::setScalar(
		const char			*name,
		i32					value,
		UniformType_t		type
	)
{
	uniform.setScalar(name, value, type);
}

/*---------------------------------------------------------------------------*/

void Geometry::setScalarArray(
		const char			*name,
		void				*list,
		u32					n_values,
		UniformType_t		type
	)
{
	uniform.setScalarArray(name, list, n_values, type);
}

/*---------------------------------------------------------------------------*/

void Geometry::setVector(
		const char			*name,
		void				*vecptr,	/* Pointer to vec#$ */
		u32					dimension,	/* # = dimension */
		UniformType_t		type		/* $ = i or f */
	)
{
	uniform.setVector(name, vecptr, dimension, type);
}

/*---------------------------------------------------------------------------*/

void Geometry::setVectorArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension,
		UniformType_t		type
	)
{
	uniform.setVectorArray(name, list, n_values, dimension, type);
}

/*---------------------------------------------------------------------------*/

void Geometry::setMatrix(
		const char			*name,
		void				*matptr,	/* Pointer to mat#f */
		u32					dimension	/* # = dimension */
	)
{
	uniform.setMatrix(name, matptr, dimension);
}

/*---------------------------------------------------------------------------*/

void Geometry::setMatrixArray(
		const char			*name,
		void				*list,
		u32					n_values,
		u32					dimension
	)
{
	uniform.setMatrixArray(name, list, n_values, dimension);
}

/*---------------------------------------------------------------------------*/

void Geometry::setTexture(
		const char			*name,
		Texture				*tex
	)
{
	uniform.setTexture(name, tex);
}

/*---------------------------------------------------------------------------*/

void Geometry::setCubeMap(
		const char			*name,
		CubeMap				*cubemap
	)
{
	uniform.setCubeMap(name, cubemap);
}

/*---------------------------------------------------------------------------*/

Node *Geometry::duplicateSelf()
{
	u32 i;

	Geometry *dup;

	dup = (Geometry *) SceneGraph::createGeometry(name);

	dup->setVertexArray(vertexArray, true);

	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {

		if (shaderProgram[i])
			dup->shaderProgram[i] = (ShaderProgram *) shaderProgram[i]->instantiate();

	}

	dup->R = R;
	dup->T = T;
	dup->S = S;

	uniform.copyTo(dup->getUniform());

	return dup;
}

/*---------------------------------------------------------------------------*/

void Geometry::getLocalBoundingBox(
		AABox			*target
	)
{
	AABox box;

	if (!vertexArray) {
		AABox b;
		b.minCorner = 0.0f;
		b.maxCorner = 0.0f;
		REPORT_WARNING("Geometry \"%s\" has no vertex array attached", name);
		*target = b;
		return;
	}

	vertexArray->getBoundingBox(target);
}

/*---------------------------------------------------------------------------*/

/* LOWPRIO: Should we store this in the geometry node? */
void Geometry::getWorldBoundingBox(
		AABox			*target
	)
{
	u32 i;
	mat4f w;
	AABox box;
	AABox wbox;

	if (!vertexArray) {
		AABox b;
		b.minCorner = 0.0f;
		b.maxCorner = 0.0f;
		REPORT_WARNING("Geometry \"%s\" has no vertex array attached", name);
		*target = b;
		return;
	}

	vertexArray->getBoundingBox(&box);

	wbox.minCorner = fMax;
	wbox.maxCorner = -fMax;

	w = getWorldMatrix();

	for (i = 0; i < 8; i++) {
		vec3f wpt;
		vec3f pt = box.getCornerPoint(i);
		wpt = w.affineMul(pt);
		wbox.include(wpt);
	}

	*target = wbox;
}

/*---------------------------------------------------------------------------*/

void Geometry::logInfo()
{
	u32 i;
	mat4f m;

	Console::log("-=-=-=-");

	logNodeInfo();
	logTransformableInfo();

	Console::log("Vertex array: %s", vertexArray->getIdentifier());
	Console::log("Shader programs:");
	for (i = 0; i < RC_GEOMETRY_MAX_PASSES; i++) {
		if (shaderProgram[i] && render_pass[i]) {
			Console::log("  Path %s: %s", render_pass[i], shaderProgram[i]->getIdentifier());
		}
	}
	Console::log("Inherited shader uniforms:");
	if (shaderProgram[curr_render_pass]) {
		shaderProgram[curr_render_pass]->getUniform()->log();
	} else {
		Console::log("No shader program for shader path!");
	}
	Console::log(" ");

	Console::log("Prioritized shader uniforms:");
	uniform.log();
	Console::log(" ");
	Console::log("-=-=-=-");
}

/*---------------------------------------------------------------------------*/

void Geometry::registerCommands()
{
//	Command::add("camera_perspective", CMDHELP_camera_perspective, CMD_camera_perspective);
}

/*---------------------------------------------------------------------------*/
