#include "RenderChimp.h"
#include "Group.h"
#include "SceneParser.h"

/*---------------------------------------------------------------------------*/

Group::Group(
		const char		*nname,
		const char		*filename
	) : Transformable(nname, NODE_GROUP)
{
	char fnbuf[FILENAME_MAX];
	const char *fnprefix = RC_RESOURCE_PATH;

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

Group::~Group()
{
	
}

/*---------------------------------------------------------------------------*/

Node *Group::duplicateSelf()
{
	Group *c = SceneGraph::createGroup(name);

	c->R = R;
	c->T = T;
	c->S = S;

	return c;
}

/*---------------------------------------------------------------------------*/

void Group::logInfo()
{
	Console::log("-=-=-=-");

	logNodeInfo();
	logTransformableInfo();

	Console::log("-=-=-=-");
}

/*---------------------------------------------------------------------------*/
