#include "RenderChimp.h"
#include "Volume.h"
#include "Sphere.h"
#include "Light.h"
#include "Command.h"

/*---------------------------------------------------------------------------*/

Light::Light(
		const char	*nname
	) : Transformable(nname, NODE_LIGHT)
{
	color = vec3f(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;


	indicatorVisible = false;

}

/*---------------------------------------------------------------------------*/

Light::~Light()
{
}

/*---------------------------------------------------------------------------*/

void Light::setColor(
		vec3f		ncolor
	)
{
	color = ncolor;
}

/*---------------------------------------------------------------------------*/

void Light::setColor(
		f32			r,
		f32			g,
		f32			b
	)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

/*---------------------------------------------------------------------------*/

void Light::setIntensity(
		f32			nintensity
	)
{
	intensity = nintensity;
}

/*---------------------------------------------------------------------------*/

void Light::setFalloff(
		f32			nfalloff
	)
{
	falloff.setRadius(nfalloff);
}

/*---------------------------------------------------------------------------*/

vec3f Light::getColor()
{
	return color;
}

/*---------------------------------------------------------------------------*/

f32 Light::getIntensity()
{
	return intensity;
}

/*---------------------------------------------------------------------------*/

f32 Light::getFalloff()
{
	return falloff.radius;
}

/*---------------------------------------------------------------------------*/

Node *Light::duplicateSelf()
{
	Light *c = SceneGraph::createLight(name);

	c->color = color;
	c->intensity = intensity;
	c->falloff = falloff;

	c->R = R;
	c->T = T;
	c->S = S;

	return c;
}

/*---------------------------------------------------------------------------*/

void Light::drawSelf()
{
	if (indicatorVisible) {
		Geometry *bulb;
		vec3f pos;

		bulb = (Geometry *) SceneGraph::getNode("LightBulb");

		pos = getWorldPosition();
		bulb->setVector("worldpos", &pos, 3, UNIFORM_FLOAT32);

		bulb->setVector("lcolor", &color, 3, UNIFORM_FLOAT32);

		bulb->render();
	}
}

/*---------------------------------------------------------------------------*/

void Light::toggleIndicator(
		bool		visible
	)
{
	indicatorVisible = visible;
}

/*---------------------------------------------------------------------------*/

bool Light::getIndicator()
{
	return indicatorVisible;
}

/*---------------------------------------------------------------------------*/

void Light::logInfo()
{
	Console::log("-=-=-=-");

	logNodeInfo();
	logTransformableInfo();

	Console::log("Color: %f %f %f", color.r, color.g, color.b);
	Console::log("Intensity: %f", intensity);
	Console::log("Falloff: %f", falloff.radius);

	Console::log("-=-=-=-");
}

/*====================================================[ Console commands ]====*/

static Light *fetchLight(
		char *nname
	)
{
	Node *node;

	node = SceneGraph::getNode(nname);

	if (!node || node->getNodeType() != NODE_LIGHT) {
		REPORT_WARNING("Light \"%s\" not found", nname);
		return 0;
	}

	return (Light *) node;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_light_color = {
	"/light_color <Light> <r> <g> <b>"
};

bool CMD_light_color(
		u32		n_args,
		char	**args
	)
{
	Light *light;
	f32 f[3];

	if (!Command::checkArgCount(4, n_args, args))
		return false;

	if (!(light = fetchLight(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	light->setColor(f[0], f[1], f[2]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_light_intensity = {
	"/light_intensity <Light> <intensity>"
};

bool CMD_light_intensity(
		u32		n_args,
		char	**args
	)
{
	Light *light;
	f32 f;

	if (!Command::checkArgCount(2, n_args, args))
		return false;

	if (!(light = fetchLight(args[0])))
		return false;

	if (!parseFloat(&f, args[1]))
		return false;

	light->setIntensity(f);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_light_indicator = {
	"/light_indicator <Light> <0/1>"
};

bool CMD_light_indicator(
		u32		n_args,
		char	**args
	)
{
	Light *light;

	if (n_args < 1)
		return false;

	if (!(light = fetchLight(args[0])))
		return false;

	if (n_args == 1) {
		light->toggleIndicator(!light->getIndicator());
	} else {
		light->toggleIndicator(!(args[1][0] == '0' && args[1][1] == 0));
	}

	return true;
}

/*---------------------------------------------------------------------------*/

void Light::registerCommands()
{
	Command::add("light_color", CMDHELP_light_color, CMD_light_color);
	Command::add("light_intensity", CMDHELP_light_intensity, CMD_light_intensity);
	Command::add("light_indicator", CMDHELP_light_indicator, CMD_light_indicator);
}

/*---------------------------------------------------------------------------*/


