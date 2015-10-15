#include "RenderChimp.h"
#include "DefaultObjects.h"

/*---------------------------------------------------------------------------*/

ShaderProgram	*DefaultObjects::failureShaderProgram;
VertexArray		*DefaultObjects::failureVertexArray;
Texture			*DefaultObjects::failureTexture;
CubeMap			*DefaultObjects::failureCubeMap;

Geometry		*DefaultObjects::bulbMesh;
ShaderProgram	*DefaultObjects::bulbShader;

Geometry		*DefaultObjects::camMesh;
ShaderProgram	*DefaultObjects::camShader;

Geometry		*DefaultObjects::arrowMesh;
ShaderProgram	*DefaultObjects::arrowShader;
Group			*DefaultObjects::axisGroup;

Geometry		*DefaultObjects::gridMesh;
ShaderProgram	*DefaultObjects::gridShader;

Group			*DefaultObjects::helperAxis;
Geometry		*DefaultObjects::helperGrid;

/*---------------------------------------------------------------------------*/

void DefaultObjects::init()
{
	RenderState *state;
	Geometry *axis[3];
	vec3f red, green, blue;

	red = vec3f(1.0f, 0.0f, 0.0f);
	green = vec3f(0.0f, 1.0f, 0.0f);
	blue = vec3f(0.0f, 0.0f, 1.0f);

	static const f32 vertex[4 * 3] = {
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f
	};

	static const i32 index[4 * 3] = {
		0, 1, 2,
		1, 2, 3,
		0, 1, 3,
		2, 0, 3
	};

	failureShaderProgram = 0;
	failureTexture = 0;
	failureCubeMap = 0;
	failureVertexArray = 0;

	printf("    Setting up failure shader program\n");
	failureShaderProgram = SceneGraph::createShaderProgram("FailureShaderProgram", "Default", 0);
	if (!failureShaderProgram)
		REPORT_ERROR("Couldn't load default shader program");

	printf("    Setting up failure texture\n");
	failureTexture = SceneGraph::createTexture("FailureTexture", "textures/notex.png", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	if (!failureTexture)
		REPORT_ERROR("Couldn't load default texture");

	printf("    Setting up failure cube map\n");
	failureCubeMap = SceneGraph::createCubeMap("FailureCubeMap", "textures/notex.png", "textures/notex.png", "textures/notex.png", "textures/notex.png", "textures/notex.png", "textures/notex.png");
	if (!failureCubeMap)
		REPORT_ERROR("Couldn't load default cube map");

	printf("    Setting up failure vertex array\n");
	failureVertexArray = SceneGraph::createVertexArray("FailureVertexArray", (void *) vertex, 4 * 3, 4, TRIANGLES, USAGE_STATIC);
	printf("      Attribute\n");
	failureVertexArray->setAttribute("Vertex", 0, 3, ATTRIB_FLOAT32, false);
	printf("      Index array\n");
	failureVertexArray->setIndexArray((void *) index, 4, 12);

	printf("    Setting up bulb mesh\n");
	bulbMesh = SceneGraph::createGeometry("LightBulb", "scenes/light.obj", false);
	bulbShader = SceneGraph::createShaderProgram("LightShader", "Light", 0);
	bulbMesh->setShaderProgram(bulbShader, false);

	printf("    Setting up camera mesh\n");
	camMesh = SceneGraph::createGeometry("CameraModel", "scenes/camera.obj", false);
	camShader = SceneGraph::createShaderProgram("CameraShader", "Camera", 0);
	camMesh->setShaderProgram(camShader, false);

	printf("    Setting up arrow mesh\n");
	arrowMesh = SceneGraph::createGeometry("ArrowModel", "scenes/arrow.obj", false);
	arrowShader = SceneGraph::createShaderProgram("ArrowShader", "Axis", 0);
	arrowMesh->setShaderProgram(arrowShader, false);

	state = arrowShader->getRenderState();

	axis[0] = (Geometry *) arrowMesh->duplicate("AxisX", false);
	axis[1] = (Geometry *) arrowMesh->duplicate("AxisY", false);
	axis[2] = (Geometry *) arrowMesh->duplicate("AxisZ", false);

	axis[0]->scale(2.0f, 2.0f, 2.0f);
	axis[1]->scale(2.0f, 2.0f, 2.0f);
	axis[2]->scale(2.0f, 2.0f, 2.0f);

	axis[0]->setRotate(-fPI * 0.5f, 0.0f, 0.0f, 1.0f);
	axis[2]->setRotate(fPI * 0.5f, 1.0f, 0.0f, 0.0f);

	axis[0]->setVector("col", red.vec, 3, UNIFORM_FLOAT32);
	axis[1]->setVector("col", green.vec, 3, UNIFORM_FLOAT32);
	axis[2]->setVector("col", blue.vec, 3, UNIFORM_FLOAT32);

	axisGroup = SceneGraph::createGroup("AxisGroup");
	axisGroup->attachChild(axis[0]);
	axisGroup->attachChild(axis[1]);
	axisGroup->attachChild(axis[2]);

	printf("    Setting up grid\n");
	/* Grid */
	{
		u32 i, o;
		VertexArray *gridva;
		f32 gridv[2 * 21 * 2 * 2];
		vec3f color;

		color = vec3f(0.4f, 0.7f, 0.8f);

		for (i = 0; i < 21; i++) {
			gridv[i * 4] = ((f32) i) - 10.0f;
			gridv[i * 4 + 1] = -10.0f;
			gridv[i * 4 + 2] = ((f32) i) - 10.0f;
			gridv[i * 4 + 3] = 10.0f;
		}

		o = 21 * 4;

		for (i = 0; i < 21; i++) {
			gridv[o + i * 4] = -10.0f;
			gridv[o + i * 4 + 1] = ((f32) i) - 10.0f;
			gridv[o + i * 4 + 2] = 10.0f;
			gridv[o + i * 4 + 3] = ((f32) i) - 10.0f;
		}

		gridva = SceneGraph::createVertexArray("Grid", gridv, 2 * 4, 84, LINES, USAGE_STATIC);
		gridva->setAttribute("Vertex", 0, 2, ATTRIB_FLOAT32, false);
		gridva->setPurgeLevel(0);

		gridShader = SceneGraph::createShaderProgram("GridShader", "Grid", 0);
		gridShader->setPurgeLevel(0);

		state = gridShader->getRenderState();

		gridMesh = SceneGraph::createGeometry("GridMesh", gridva, false);
		gridMesh->setShaderProgram(gridShader, false);

		gridShader->setVector("Color", color.vec, 3, UNIFORM_FLOAT32);
		gridShader->renderState.disableCulling();

	}

	printf("    Default object setup complete\n");
}

/*---------------------------------------------------------------------------*/

void DefaultObjects::destroy()
{
	SceneGraph::deleteNode(bulbMesh);
	SceneGraph::deleteNode(camMesh);
	SceneGraph::deleteNode(arrowMesh);
	SceneGraph::deleteNode(gridMesh);
	SceneGraph::deleteNode(axisGroup);

	if (helperGrid)
		SceneGraph::deleteNode(helperAxis);

	if (helperAxis)
		SceneGraph::deleteNode(helperGrid);

	SceneGraph::dereferenceResource(failureShaderProgram);
	SceneGraph::dereferenceResource(failureTexture);
	SceneGraph::dereferenceResource(failureCubeMap);
	SceneGraph::dereferenceResource(failureVertexArray);
}

/*---------------------------------------------------------------------------*/

void DefaultObjects::drawHelpers()
{
//	if (helperAxis || helperGrid) {
//		Renderer::setRenderTarget(0);
//		Renderer::setClearDepth(1.0f);
//		Renderer::clearDepth();
//	}

	if (helperGrid)
		helperGrid->render();

	if (helperAxis)
		helperAxis->renderAll();
}

/*---------------------------------------------------------------------------*/

