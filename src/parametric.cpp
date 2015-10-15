
#include "program.h"

#if defined ASSIGNMENT_23

/*
	EDA221 Introduction to Computer Graphics, 2011

	Assignment 2 - Parametric Shapes
*/

#include "RenderChimp.h"
#include "Command.h"
#include "ParametricShapes.h"
#include "Interpolation.h"

World	*world;				/* Scene graph root node */
Camera	*camera;			/* Scene camera */
Group	*camera_pivot;		/* A group is an empty, transformable node */
Light	*light;				/* A light is a point in the scene which can be used for lighting calculations */

Geometry		*shape;	/* */
VertexArray		*shape_va;

vec3f			*path = new vec3f[10];
i32				p=0;
f32				t=0;

vec3f			ambient(0.3f, 0.3f, 0.3f);	/* Ambient color for the sun (never gets dark) */

ShaderProgram	*shape_shader;		/* To add textures we need a shader program (vertex + fragment shader) */
Texture			*shape_texture;		/* Sun texture */

float			spin = 0.2f;	/* Angular velocity of the sun (radians/s) */


/* variables used for mouse-camera control */
vec2f	camera_rotation(0.0f, 0.0f),	/* current rotation of camera */
		mouse_prev_pos(0.0f, 0.0f);		/* previous mouse position */

void RCInit()
{
	world = SceneGraph::createWorld("solar_system");

	/* Camera */
	camera = SceneGraph::createCamera("cam1");
	camera->setPerspectiveProjection(1.2f, 4.0f / 3.0f, 1.0f, 1000.0f);
	camera->translate(0.0f, 1.0f, 6.0f);
	world->setActiveCamera(camera);

	/* Create pivot */
	camera_pivot = SceneGraph::createGroup("camera_pivot");
	camera_pivot->attachChild(camera);
	world->attachChild(camera_pivot);

	/* light */
	light = SceneGraph::createLight("light1");
	light->setColor(1.0f, 1.0f, 1.0f);
	light->setTranslate(15.0f, 15.0f, 15.0f);
	world->attachChild(light);	

	/* Create vertex array for circle ring */
	//shape_va = createCircleRing(4, 60, 1, 2);
    
    // TODO: implement createSphere(...) in ParametricShapes.cpp
	shape_va = createSphere(100, 100, 2);
    
    // TODO: implement createTorus(...) in ParametricShapes.cpp
	//shape_va = createTorus(100, 100, 2, 1);

	shape = SceneGraph::createGeometry("saturn_rings", shape_va, false);
	world->attachChild(shape);

	vec3f specular = vec3f(1, 0, 0);
	/* Create a shader and set its attributes */
	/*shape_shader = SceneGraph::createShaderProgram("phong shader", "Phong", 0);
	shape_shader->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("Ambient", ambient.vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("SpecularColor", specular.vec, 3, UNIFORM_FLOAT32);
	shape_shader->setScalar("Shininess", 50, UNIFORM_FLOAT32);*/

	CubeMap *m = SceneGraph::createCubeMap("reflection", "cubemaps/sky_left.png", "cubemaps/sky_right.png", "cubemaps/sky_up.png", "cubemaps/sky_down.png", "cubemaps/sky_front.png", "cubemaps/sky_back.png");
	shape_shader = SceneGraph::createShaderProgram("cube shader", "CubeMap", 0);
	shape_shader->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setCubeMap("CubeTex", m);
	/*Texture *tex = SceneGraph::createTexture("tex", "textures/Fieldstone_diffuse.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	Texture *bumpTex = SceneGraph::createTexture("alpha", "textures/Fieldstone_bump.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	shape_shader = SceneGraph::createShaderProgram("bump map shader", "BumpMap", 0);
	shape_shader->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	shape_shader->setVector("Ambient", ambient.vec, 3, UNIFORM_FLOAT32);
	shape_shader->setTexture("BumpMap", bumpTex);
	shape_shader->setTexture("DiffuseTex", tex);*/

	//shape_shader->getRenderState()->enableCulling(CULL_FRONT);
	//shape_shader->getRenderState()->disableCulling();

	/* Assign shader and texture to the sun (uncomment when reaching this point) */
	shape->setShaderProgram(shape_shader);

	/* Set clear color and depth */
	Renderer::setClearColor(vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	Renderer::setClearDepth(1.0f);
    
    /* Enable XZ-grid and world axes */
    Command::run("/grid 1");
    Command::run("/axis_origin 1");

	/* path gen */
	for (i32 i = 0; i < 10; i++) {
		f32 theta = i * (2*fPI/10);
		path[i] = vec3f(cosf(theta) * 5, 0, sinf(theta) * 5);
	}
}


int circle_mod(int i, int o) {
	int x = i;
	x += o;
	if (x < 0)
		return x += 10;
	return x % 10;
}

u32 RCUpdate()
{
	//shape->rotateY( spin * Platform::getFrameTimeStep() );

	/* Mouse & keyboard controls --> */

	/* Get the current mouse button state */
	bool *mouse = Platform::getMouseButtonState();

	/* Get mouse screen coordinates [-1, 1] */
	vec2f mouse_pos = Platform::getMousePosition();

	/* Translate mouse position and button state to camera rotations */
	if (mouse[RC_MOUSE_LEFT]) {

		/* Absolute position */

		camera_rotation.x = -mouse_pos.x * 2.0f;
		camera_rotation.y = mouse_pos.y * 2.0f;

	} else if (mouse[RC_MOUSE_RIGHT]) {

		/* Relative position */

		vec2f mouse_diff = mouse_pos - mouse_prev_pos;
		camera_rotation.x -= mouse_diff.x * 2.0f;
		camera_rotation.y += mouse_diff.y * 2.0f;
	}

	/* Perform camera rotations */
	camera_pivot->setRotateX(camera_rotation.y);
	camera_pivot->rotateY(camera_rotation.x);

	/* Store previous mouse screen position */
	mouse_prev_pos = mouse_pos;

	t += Platform::getFrameTimeStep();
	
	int p0 = circle_mod(p, -1),
		p1 = p,
		p2 = circle_mod(p, 1),
		p3 = circle_mod(p2, 1);

	//printf("%i %i %i %i", p0, p1, p2, p3);
	//vec3f old = shape->getTranslation();
	////shape->setTranslate(evalLERP(path[p], path[(p+1)%10], t));
	//shape->setTranslate(evalCatmullRom(path[p0], path[p1], path[p2], path[p3], 0.5f, t));
	//vec3f newp = shape->getTranslation();
	//vec3f dp = newp-old;
	//vec3f vel = dp * ( 1 / Platform::getFrameTimeStep());
	//shape->lookTowards(vel.normalize());
	//if (t >= 1) {
	//	p = (p + 1) % 10;
	//	t -= 1;
	//}


	/* Get the current key state */
	bool *keys = Platform::getKeyState();

	/* Map keys to bilateral camera movement */
	f32 move = 0.0f, strafe = 0.0f;
	if (keys[RC_KEY_W])	move += 0.1f;
	if (keys[RC_KEY_S])	move -= 0.1f;
	if (keys[RC_KEY_A])	strafe -= 0.1f;
	if (keys[RC_KEY_D])	strafe += 0.1f;

	/* Apply movement to camera */
	camera->translate(camera->getLocalFront() * move);
	camera->translate(camera->getLocalRight() * strafe);

	/* Clear color and depth buffers */
	Renderer::clearColor();
	Renderer::clearDepth();

	/* Tell RenderChimp to render the scenegraph */
	world->renderAll();


	return 0;
}

void RCDestroy()
{
	// release memory allocated by the scenegraph
	SceneGraph::deleteNode(world);
}

#endif /* ASSIGNMENT_2 */

