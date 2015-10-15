
#include "program.h"
#include "ParametricShapes.h"

#if defined ASSIGNMENT_1

/*
	EDA221 Introduction to Computer Graphics, 2011

	Assignment 1 - Solar System
*/

#include "RenderChimp.h"
#include "Command.h"

World	*world;				/* Scene graph root node */
Camera	*camera;			/* Scene camera */
Group	*camera_pivot, *saturn, *ug;		/* A group is an empty, transformable node */
Light	*light;				/* A light is a point in the scene which can be used for lighting calculations */
Group	*mep, *vp, *ep, *map, *jp, *sp, *up, *np, *pp;

Geometry		*sun, *mercury, *venus, *earth, *mars, *jupiter, *saturnus, *uranus, *neptune, *pluto;		/* Sun node */

vec3f			sun_ambient(1.0f, 1.0f, 1.0f);	/* Ambient color for the sun (never gets dark!) */

ShaderProgram	*sun_shader;		/* To add textures we need a shader program (vertex + fragment shader) */
Texture			*sun_texture, *met, *vt, *et, *mat, *jt, *st, *ut, *nt, *pt, *rt, *urt;		/* Sun texture */

float			sun_spin = 0.4f;	/* Angular velocity (spin) of the sun (radians/s) */


/* variables used for mouse-camera control */
vec2f	camera_rotation(0.0f, 0.0f),	/* current rotation of camera */
		mouse_prev_pos(0.0f, 0.0f);		/* previous mouse position */

void RCInit()
{
	/* Create world node */
	world = SceneGraph::createWorld("solar_system");

	/* TODO: Create camera */
	camera = SceneGraph::createCamera("MyCamera");
	
	/* Set camera perspective projection (field-of-view, aspect ratio, near-plane. far-plane) */
	camera->setPerspectiveProjection(1.2f, 4.0f / 3.0f, 1.0f, 1000.0f);
	/* TODO: Translate the camera to (0, 1, 6) */
	camera->translate(0.0f, 1.0f, 6.0f);
    /* ... */
	/* Make this camera the active camera */
	world->setActiveCamera(camera);


	/* Create camera pivot group */
	//camera_pivot = SceneGraph::createGroup("camera_pivot");
	/* Attach camera */
	//camera_pivot->attachChild(camera);
	/* Attach to world */
	world->attachChild(camera);

	/* TODO: Create light source */
	light = SceneGraph::createLight("MyLight");
	
	
    /* ... */
	/* TODO: Set the light color to (1, 1, 1) */
	light->setColor(1.0f, 1.0f, 1.0f);
    /* ... */
	/* TODO: Translate the light to (10, 10, 10) */
	light->setTranslate(10.0f, 10.0f, 10.0f);
    /* ... */
	/* TODO: Attach the light to the world */
	world->attachChild(light);
    /* ... */

	/* Create sun geometry from a sphere object-file and attach it to world */
	sun = SceneGraph::createGeometry("sun", "scenes/sphere.obj");
	world->attachChild(sun);

	/* TODO: Build & run (F5 in Visual Studio) */

	/* Create a shader, set its attributes and attach to sun (uncomment when reaching this point) */
	sun_shader = SceneGraph::createShaderProgram("sun_shader", "LambertTexture", 0);
	sun_shader->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	sun_shader->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	sun_shader->setVector("Ambient", sun_ambient.vec, 3, UNIFORM_FLOAT32);
    sun->setShaderProgram(sun_shader);
    
	/* Create a texture from file and assign to the sun (uncomment when reaching this point) */
	sun_texture = SceneGraph::createTexture("sun_texture", "textures/sunmap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	sun->setTexture("DiffuseTex", sun_texture);

    
	/* TODO: Build & run (F5 in Visual Studio) */
	mep = SceneGraph::createGroup("mep pivot");
	sun->attachChild(mep);

	vp = SceneGraph::createGroup("vp pivot");
	sun->attachChild(vp);

	ep = SceneGraph::createGroup("ep pivot");
	sun->attachChild(ep);

	map = SceneGraph::createGroup("map pivot");
	sun->attachChild(map);

	jp = SceneGraph::createGroup("jp pivot");
	sun->attachChild(jp);

	sp = SceneGraph::createGroup("sp pivot");
	sun->attachChild(sp);

	up = SceneGraph::createGroup("up pivot");
	sun->attachChild(up);

	np = SceneGraph::createGroup("np pivot");
	sun->attachChild(np);

	pp = SceneGraph::createGroup("pp pivot");
	sun->attachChild(pp);
	
	
	/* TODO: Create rest of the solar system... */
	mercury = SceneGraph::createGeometry("mercury", "scenes/sphere.obj");
	mep->attachChild(mercury);
	mercury->setShaderProgram(sun_shader);
	met = SceneGraph::createTexture("mercury_texture", "textures/mercurymap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	mercury->setTexture("DiffuseTex", met);
	mercury->translate(2.0f, 0.0f, 0.0f);
	mercury->scale(0.1f);

	venus = SceneGraph::createGeometry("venus", "scenes/sphere.obj");
	vp->attachChild(venus);
	venus->setShaderProgram(sun_shader);
	vt = SceneGraph::createTexture("venus_texture", "textures/venusmap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	venus->setTexture("DiffuseTex", vt);
	venus->translate(3.0f, 0.0f, 0.0f);
	venus->scale(0.25f);

	earth = SceneGraph::createGeometry("earth", "scenes/sphere.obj");
	ep->attachChild(earth);
	earth->setShaderProgram(sun_shader);
	et = SceneGraph::createTexture("earth_texture", "textures/earthmap1k.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	earth->setTexture("DiffuseTex", et);
	earth->translate(4.0f, 0.0f, 0.0f);
	earth->scale(0.25f);

	mars = SceneGraph::createGeometry("mars", "scenes/sphere.obj");
	map->attachChild(mars);
	mars->setShaderProgram(sun_shader);
	mat = SceneGraph::createTexture("mars_texture", "textures/marsmap1k.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	mars->setTexture("DiffuseTex", mat);
	mars->translate(5.0f, 0.0f, 0.0f);
	mars->scale(0.2f);

	jupiter = SceneGraph::createGeometry("jupiter", "scenes/sphere.obj");
	jp->attachChild(jupiter);
	jupiter->setShaderProgram(sun_shader);
	jt = SceneGraph::createTexture("jupiter_texture", "textures/jupitermap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	jupiter->setTexture("DiffuseTex", jt);
	jupiter->translate(7.0f, 0.0f, 0.0f);
	jupiter->scale(0.7f);

	saturn = SceneGraph::createGroup("saturn");
	sp->attachChild(saturn);

	saturnus = SceneGraph::createGeometry("saturnus", "scenes/sphere.obj");
	saturn->attachChild(saturnus);
	saturnus->setShaderProgram(sun_shader);
	st = SceneGraph::createTexture("saturnus_texture", "textures/saturnmap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	saturnus->setTexture("DiffuseTex", st);
	saturnus->scale(0.5f);

	Group *srg = SceneGraph::createGroup("saturn rings");
	VertexArray *ring_va = createCircleRing(100, 100, 0.6f, 1.0f);
	Geometry *ring = SceneGraph::createGeometry("ring1", ring_va);
	ring->setShaderProgram(sun_shader);
	rt = SceneGraph::createTexture("saturnusring_texture", "textures/saturnringcolor.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	ring->setTexture("DiffuseTex", rt);
	srg->attachChild(ring);

	ring = SceneGraph::createGeometry("ring2", ring_va);
	ring->setShaderProgram(sun_shader);
	ring->setTexture("DiffuseTex", rt);
	ring->rotateX(3.1415f);
	srg->attachChild(ring);

	srg->rotateX(3.1415f * 0.25f);
	srg->rotateY(3.1415f * 0.25f);
	sp->attachChild(srg);

	saturn->translate(9.0f, 0.0f, 0.0f);
	srg->translate(saturn->getTranslation());
	
	ug = SceneGraph::createGroup("uranusgroup");
	up->attachChild(ug);

	uranus = SceneGraph::createGeometry("uranus", "scenes/sphere.obj");
	ug->attachChild(uranus);
	uranus->setShaderProgram(sun_shader);
	ut = SceneGraph::createTexture("uranus_texture", "textures/uranusmap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	uranus->setTexture("DiffuseTex", ut);
	uranus->scale(0.5f);

	Group *urg = SceneGraph::createGroup("uranus rings");
	ring = SceneGraph::createGeometry("urings1", ring_va);
	ring->setShaderProgram(sun_shader);
	urt = SceneGraph::createTexture("uranusring_texture", "textures/uranusringcolour.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	ring->setTexture("DiffuseTex", urt);
	urg->attachChild(ring);

	ring = SceneGraph::createGeometry("urings2", ring_va);
	ring->setShaderProgram(sun_shader);
	ring->setTexture("DiffuseTex", urt);
	ring->rotateX(3.1415f);
	urg->attachChild(ring);
	up->attachChild(urg);

	ug->translate(11.0f, 0.0f, 0.0f);
	urg->translate(11.0f, 0.0f, 0.0f);

	neptune = SceneGraph::createGeometry("neptune", "scenes/sphere.obj");
	np->attachChild(neptune);
	neptune->setShaderProgram(sun_shader);
	nt = SceneGraph::createTexture("neptune_texture", "textures/neptunemap.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	neptune->setTexture("DiffuseTex", nt);
	neptune->translate(13.0f, 0.0f, 0.0f);
	neptune->scale(0.5f);

	pluto = SceneGraph::createGeometry("pluto", "scenes/sphere.obj");
	pp->attachChild(pluto);
	pluto->setShaderProgram(sun_shader);
	pt = SceneGraph::createTexture("pluto_texture", "textures/plutomap1k.jpg", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	pluto->setTexture("DiffuseTex", pt);
	pluto->translate(14.0f, 0.0f, 0.0f);
	pluto->scale(0.1f);

	/* Set clear color and depth */
	Renderer::setClearColor(vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	Renderer::setClearDepth(1.0f);

    /* Enable XZ-grid and world axes */
    Command::run("/grid 1");
    Command::run("/axis_origin 1");
}

u32 RCUpdate()
{
	float dt = Platform::getFrameTimeStep();
	/* Apply rotation to the sun */
	sun->rotateY( sun_spin * Platform::getFrameTimeStep() );

    
	/* TODO: Perform animations for the rest of the solar system */
	mercury->rotateY( sun_spin * dt );
	venus->rotateY( sun_spin * dt );
	earth->rotateY( sun_spin * dt );
	mars->rotateY( sun_spin * dt );
	jupiter->rotateY( sun_spin * dt );
	saturnus->rotateY( sun_spin * dt );
	uranus->rotateY( sun_spin * dt );
	neptune->rotateY( sun_spin * dt );
	pluto->rotateY( sun_spin * dt );

	mep->rotateY(-1.5f * dt);
	vp->rotateY(0.25f * dt);
    ep->rotateY(-0.9f * dt);
	map->rotateY(0.15f * dt);
	jp->rotateY(-0.7f * dt);
	sp->rotateY(0.10f * dt);
	up->rotateY(-0.6f * dt);
	np->rotateY(0.05f * dt);
	pp->rotateY(-1 * dt);
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

	/* Perform camera rotations (pivot style) */
	camera->setRotateX(camera_rotation.y);
	camera->rotateY(camera_rotation.x);

	/* Store previous mouse screen position */
	mouse_prev_pos = mouse_pos;


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

	/* tell RenderChimp to render the scenegraph */
	world->renderAll();


	return 0;
}

void RCDestroy()
{
	/* release memory allocated by the scenegraph */
	SceneGraph::deleteNode(world);
}

#endif /* ASSIGNMENT_1 */
