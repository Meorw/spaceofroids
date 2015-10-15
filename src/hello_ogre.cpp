
#include "program.h"

#if defined OGRE

/*
 * Example 01 - Hello Ogre
 *
 * Lessons learned:
 *  - Model loading
 *  - Simple shader program usage
 *  - Catching mouse events
 *
 */

#include "RenderChimp.h"
#include "Command.h"

/* Scene graph root node */
World *world;

/* Scene camera */
Camera *camera;

/* This will contain a mesh loaded from an .obj file */
Geometry *ogre;

/* A group is an empty, transformable node */
Group *camera_pivot;

/* A light is a point in the scene which can be used for lighting calculations
 */
Light *light;

/* Shader programs are renderer specific pipeline steps */
ShaderProgram *shader;

/* A texture would make our mesh more interesting */
Texture *tex;

/* These are used to store the previous and current frame camera rotation */
vec2f mouse_prev_pos;
vec2f camera_rotation;

/* Program start time */
f32 starttime;

void RCInit()
{
	world = SceneGraph::createWorld("MyWorld");

	camera = SceneGraph::createCamera("MyCamera");
	camera->setPerspectiveProjection(1.2f, 4.0f / 3.0f, 1.0f, 1000.0f);
	camera->translate(0.0f, 0.0f, 10.0f);

	/* Transformations are stacked, so attaching the camera to a group node
	 * that is in the origin of the scene allows us to rotate around the origin
	 */
	camera_pivot = SceneGraph::createGroup("MyCameraTarget");
	camera_pivot->attachChild(camera);

	world->attachChild(camera_pivot);
	world->setActiveCamera(camera);

	/* The Lambert BRDF is a simple lighting model.
	 *   The string "Lambert" in this initialization is interpreted by the
	 * back end Renderer used. For example, the OpenGL 2.0 Renderer will look
	 * for the files "RC_SHADER_PATH/gl20/Lambert.gl20.vs" and
	 * "RC_SHADER_PATH/gl20/Lambert.gl20.fs" and compile a GL specific shader
	 * program with these two shader source files. Read more regarding how,
	 * exactly, each renderer interpret the incoming description string in
	 * the corresponding Renderer#.h files.
	 */
	shader = SceneGraph::createShaderProgram("OgreShader", "LambertTexture", 0);

	/* Load a texture from an image file */
	tex = SceneGraph::createTexture("Tex", "textures/boards.png", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);

	/* Load a mesh from an .obj file.
	 *   This sets up a VertexArray resource "behind the scenes" that contains
	 * the mesh.
	 */
	ogre = SceneGraph::createGeometry("Ogre", "scenes/ogre.obj", 0);
	ogre->setScale(2.0f, 2.0f, 2.0f);
	world->attachChild(ogre);

	/* Apply the Lambert shader to the ogre geometry node */
	ogre->setShaderProgram(shader, true);

	/* Set the texture called "DiffuseTex" to our previously loaded texture
	 *   The texture name should correspond to what your shader program
	 * requires. In the case of OpenGL 2.0, texture names should match
	 * "uniform sampler2D"-type variables. Other Renderers may have other
	 * requirements.
	 */
	ogre->setTexture("DiffuseTex", tex);

	/* Create and set up a point light source some distance away
	 *   It's important to note that the light's position, color, etc, are not
	 * in any way automatically utilized for shading later in the pipeline. If
	 * you want to relay information from a light source to your shader
	 * program, you must do so yourself (as we will see in just a moment). The
	 * Light node is merely a convenient node to have, since it's such a common
	 * entity in most scenes.
	 */
	light = SceneGraph::createLight("MyLight");
	light->setColor(1.0f, 1.0f, 1.0f);
	light->setTranslate(100.0f, 100.0f, 100.0f);

	world->attachChild(light);

	/* Relay the information of the light's position and color to the shader
	 * program. Again, these names and types must match the expectations of the
	 * shader program and renderer used. In OpenGL 2.0, the corresponding light
	 * position variable of the example code below would be declared as
	 * "uniform vec3 LightPosition".
	 */
	ogre->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	ogre->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);

	/* Camera rotation initiation */
	mouse_prev_pos = vec2f(0.0f, 0.0f);
	camera_rotation = vec2f(0.0f, 0.0f);

	/* Log a message to the console */
	Console::log("Example 01 - Hello ogre!");
	Console::log("Left click and drag to rotate to an absolute position");
	Console::log("Right click and drag to rotate to a relative position");
	Console::log("Press W-S and A-D to move/strafe");

	/* Set clear color and depth */
	Renderer::setClearColor(vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	Renderer::setClearDepth(1.0f);
    
    /* Enable XZ-grid and world axes */
    Command::run("/grid 1");
    Command::run("/axis_origin 1");
}



u32 RCUpdate()
{
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

		vec2f diff = mouse_pos - mouse_prev_pos;

		camera_rotation.x -= diff.x * 2.0f;
		camera_rotation.y += diff.y * 2.0f;

	}

	/* Perform camera rotations (pivot style) */
	camera_pivot->setRotateX(camera_rotation.y);
	camera_pivot->rotateY(camera_rotation.x);

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

	/* Render the scene graph */
	world->renderAll();

	return 0;

}

void RCDestroy()
{
	/* Scene graph clean up */
	SceneGraph::deleteNode(world);
}

#endif /* OGRE */
