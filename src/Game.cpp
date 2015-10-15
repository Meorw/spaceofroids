#include "program.h"

#ifdef GAME

#include "RenderChimp.h"
#include "Command.h"
#include "ParametricShapes.h"
#include <math.h>
#include "Interpolation.h"

/* Scene graph root node */
World *world;
Camera *camera;
Light *light;
Geometry *ship;
Text *text;

f32 activateNext = 1.0f;
f32 starttime;

/* These are used to store the previous and current frame camera rotation */
vec2f mouse_prev_pos;
vec2f camera_rotation;

struct Asteroid {
	vec3f velocity;
	float spin;
	Geometry *ast;
	float radius;
	int pos;
};

#define N_ASTEROIDS 200
Asteroid *asteroids[N_ASTEROIDS];
int active = 0;

void initializeAsteroid(Asteroid *roid) {
	float difficultySpeed = pow((Platform::getCurrentTime() - starttime) / 15.0f, 1.3f);
	roid->velocity = vec3f(0, 0, 25) + vec3f(0, 0, 1) * (rand() % 25) * difficultySpeed;
	roid->spin = roid->velocity.length() * 0.1f;

	vec3f pos = vec3f(rand() % 40 - 20, rand() % 40 - 20, -150 + (rand() % 20));
	roid->ast->setTranslate(pos);
	roid->ast->setActive(true);
}

ShaderProgram *shader;
Asteroid* createAsteroid (int i) {
	Asteroid *roid = new Asteroid();
	
	roid->pos = i;
	
	int size = rand() % 25 + 5;
	roid->radius = 0.1f * size;
	roid->ast = SceneGraph::createGeometry("Asteroid", createSphere(size/10 + 4, size/10 + 4, 0.1f * size));

	roid->ast->setShaderProgram(shader, true);
	roid->ast->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	roid->ast->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	
	world->attachChild(roid->ast);
	roid->ast->setActive(false);

	return roid;
}
void deactiveAsteroid(Asteroid *ast) {
	ast->ast->setActive(false);

	/* Do some nice fluffy array magic */
	active--;
	Asteroid *tmp = asteroids[active];
	asteroids[ast->pos] = tmp;
	tmp->pos = ast->pos;
	asteroids[active] = ast;
	ast->pos = active;
}

int gameState = 0;
int points;
void startGame() {
	if (gameState != 0) {
		printf("wtf");
		return;
	}

	gameState = 1;
	activateNext = 1;
	points = 0;
	starttime = Platform::getCurrentTime();
	srand((unsigned int)starttime);

	active = 0;
	/*for (int i = 0; i < active; i++) {
		initializeAsteroid(asteroids[i]);
		asteroids[i]->ast->setActive(true);
	}*/
	for (int i = active; i < N_ASTEROIDS; i++) {
		asteroids[i]->ast->setActive(false);
	}
}

void RCInit()
{
	world = SceneGraph::createWorld("MyWorld");
	
	shader = SceneGraph::createShaderProgram("Rock-it-shade", "RockShader", 0);
	//Texture *rockTexture = SceneGraph::createTexture("tex", "textures/lunarrock_diffuse.png", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	//Texture *bumpTex = SceneGraph::createTexture("alpha", "textures/lunarrock_normal.png", true, TEXTURE_FILTER_TRILINEAR, TEXTURE_WRAP_REPEAT);
	//shader->setTexture("DiffuseTex", rockTexture);
	//shader->setTexture("BumpMap", bumpTex);

	camera = SceneGraph::createCamera("MyCamera");
	camera->setPerspectiveProjection(1.2f, 4.0f / 3.0f, 1.0f, 1000.0f);
	camera->translate(0.0f, 0.0f, 10.0f);
	camera->setRotateX(0);
	camera->rotateY(0);

	/* Camera rotation initiation */
	mouse_prev_pos = vec2f(0.0f, 0.0f);
	camera_rotation = vec2f(0.0f, 0.0f);

	world->attachChild(camera);
	world->setActiveCamera(camera);

	light = SceneGraph::createLight("MyLight");
	light->setColor(1.0f, 1.0f, 1.0f);
	light->setTranslate(100.0f, 100.0f, 100.0f);

	world->attachChild(light);

	for (int i = 0; i < N_ASTEROIDS; i++) {
		asteroids[i] = createAsteroid(i);
	}

	/* Set clear color and depth */
	Renderer::setClearColor(vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	Renderer::setClearDepth(1.0f);

	/* Skybox in space */
	VertexArray *va = createSphere(100, 100, 800);
	Geometry *spacesky = SceneGraph::createGeometry("Space", va);
	ShaderProgram *map = SceneGraph::createShaderProgram("Map", "CubeMap", 0);
	CubeMap *m = SceneGraph::createCubeMap("reflection", "cubemaps/pink_planet_front.png", "cubemaps/pink_planet_right.png", "cubemaps/pink_planet_up.png", "cubemaps/pink_planet_down.png", "cubemaps/pink_planet_left.png", "cubemaps/pink_planet_back.png");
	spacesky->setCubeMap("CubeTex", m);
	map->getRenderState()->disableCulling();
	spacesky->setShaderProgram(map, true);
	world->attachChild(spacesky);

	ship = SceneGraph::createGeometry("Spaceship", "scenes/spaceship.obj");
	ShaderProgram *shipit = SceneGraph::createShaderProgram("Ship Shader", "ColorLambert", 0);
	ship->setShaderProgram(shipit, true);
	ship->setVector("LightPosition", light->getWorldPosition().vec, 3, UNIFORM_FLOAT32);
	ship->setVector("LightColor", light->getColor().vec, 3, UNIFORM_FLOAT32);
	ship->setVector("Ambient", vec3f(1, 0, 0).vec, 3, UNIFORM_FLOAT32);

	text = new Text();
	text->setFont("fonts/console.png", false);
	text->lineSpacing = -((i32) text->getGlyphHeight() + 2);
	text->maxWidth = 200;
	text->maxLines = 1;
	u32 x, y;
	Platform::getDisplaySize(&x, &y);
	text->setXY(x/2, y/2);

	ship->scale(0.5f);
	ship->rotateY(3.14f);
	world->attachChild(ship);

	startGame();
}


float getAsteroidSpawnRate(){
	float time = (Platform::getCurrentTime() - starttime);
	int secs = (int)time;
	int steps = secs / 10;

	return 0.25f - min(steps * 0.1, 0.24f);
}

vec3f shipVel = vec3f(0,0,0);
void moveShip(){
	/* Get the current key state */
	bool *keys = Platform::getKeyState();
	float dt =  Platform::getFrameTimeStep();

	/* Map keys to bilateral camera movement */
	f32 move = 0.0f, strafe = 0.0f;
	if (keys[RC_KEY_W])	move += 0.1f;
	if (keys[RC_KEY_S])	move -= 0.1f;
	if (keys[RC_KEY_A])	strafe -= 0.1f;
	if (keys[RC_KEY_D])	strafe += 0.1f;

	/* Apply movement to camera */
	shipVel += 3500 * dt * (camera->getLocalUp() * move + camera->getLocalRight() * strafe);
	shipVel *= 0.95f;

	vec3f newPos = ship->getTranslation() + shipVel * dt;
	if (newPos.x > 8) {
		newPos.x = 8;
		shipVel.x = 0;
	}
	if (newPos.x < -8) {
		newPos.x = -8;
		shipVel.x = 0;
	}
	if (newPos.y > 6) {
		newPos.y = 6;
		shipVel.y = 0;
	}
	if (newPos.y < -6) {
		newPos.y = -6;
		shipVel.y = 0;
	}

	newPos.z = 0;
	ship->setTranslate(newPos);
}


void renderGameOver(){
	text->print("Game Over");

	char buf[128];
	sprintf(buf, "%d points", points);
	text->printLine(buf, 5);

	bool *keys = Platform::getKeyState();
	if (keys[RC_KEY_RETURN]) {
		startGame();
	}
}

void renderGame() {
	float dt =  Platform::getFrameTimeStep();

	moveShip();
	vec3f shipPos = ship->getTranslation();

	/* Update active asteroids */
	for (int i = 0; i < active; i++) {
		Asteroid *ast = asteroids[i];
		
		ast->ast->translate(ast->velocity * dt);
		ast->ast->rotateY(ast->spin * dt);

		if (ast->ast->getTranslation().z > 11) {
			deactiveAsteroid(ast);
			printf("active: %d\n", active);
			i--;
		} else if (ast->ast->getTranslation().z > -5) {
			/* Check collision on active asteroids */
			float dist2 = (ast->ast->getTranslation() - shipPos).lengthSquared();
			float radius = ast->radius + 0.15f;
			if (dist2 <= radius*radius) {
				points = (int)(Platform::getCurrentTime() - starttime);
				gameState = 0;
				break;
			}
		}
	}

	activateNext -= Platform::getFrameTimeStep();
	if (activateNext <= 0.0f) {
		activateNext += getAsteroidSpawnRate();
		printf("next in: %f\n", activateNext);

		if (active < N_ASTEROIDS) {
			initializeAsteroid(asteroids[active]);
			active++;
			printf("active %d\n", active);
		}
	}
}
u32 RCUpdate()
{
	if (gameState == 1) {
		renderGame();
	}

	/* Clear color and depth buffers */
	Renderer::clearColor();
	Renderer::clearDepth();

	/* Render the scene graph */
	world->renderAll();

	if (gameState == 0) {
		renderGameOver();
	}
	return 0;
}

void RCDestroy()
{
	/* Scene graph clean up */
	SceneGraph::deleteNode(world);
}
#endif // GAME