#include <cstdio>
#include "RenderChimp.h"
#include "Node.h"
#include "Transformable.h"
#include "Camera.h"
#include "World.h"

/*---------------------------------------------------------------------------*/

World::World(
		const char	*nname
	) : Node(nname, NODE_WORLD)
{
	activeCamera = 0;
}

/*---------------------------------------------------------------------------*/

World::~World()
{
}

/*---------------------------------------------------------------------------*/

void World::drawSelf()
{

	if (activeCamera)
		Renderer::setMatrices(activeCamera);

}

/*---------------------------------------------------------------------------*/

void World::setActiveCamera(
		Camera *camera
	)
{
	activeCamera = camera;
}

/*---------------------------------------------------------------------------*/

Camera *World::getActiveCamera()
{
	return activeCamera;
}

/*---------------------------------------------------------------------------*/

Node *World::duplicateSelf()
{
	World *c = SceneGraph::createWorld("WorldCopy");

	c->setActiveCamera(activeCamera);

	return c;
}

/*---------------------------------------------------------------------------*/

void World::logInfo()
{
	Console::log("-=-=-=-");

	logNodeInfo();

	Console::log("Active camera: %s", activeCamera ? activeCamera->getName() : "None");

	Console::log("-=-=-=-");
}

/*---------------------------------------------------------------------------*/

