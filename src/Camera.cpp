#include "RenderChimp.h"
#include "Transformable.h"
#include "Camera.h"
#include "Command.h"
#include <math.h>

/*---------------------------------------------------------------------------*/

Camera::Camera(
		const char	*nname
	) : Transformable(nname, NODE_CAMERA)
{
	projectionMatrix = mat4f(1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0,
							 0, 0, 0, 1);
	nearPlane = 1;
	farPlane = 100;
	projectionType = RC_PROJECTION_TYPE_FREE_TRANSFORM;

	projectionChanged = true;

	indicatorVisible = false;
}

/*---------------------------------------------------------------------------*/

Camera::~Camera()
{

}

/*---------------------------------------------------------------------------*/

void Camera::setOrthogonalProjection(
		f32		left,
		f32		right,
		f32		bottom,
		f32		top,
		f32		nnear,
		f32		nfar
	)
{
	f32 arl, srl_inv;
	f32 atb, stb_inv;
	f32 afn, sfn_inv;

	if (left == right || top == bottom || nfar == nnear) {
		REPORT_WARNING("Camera values are not valid");
		return;
	}

	arl = right + left;
	srl_inv = 1.0f / (right - left);
	atb = top + bottom;
	stb_inv = 1.0f / (top - bottom);
	afn = nfar + nnear;
	sfn_inv = 1.0f / (nfar - nnear);

	projectionMatrix.identity();

	projectionMatrix.R00 = 2.0f * srl_inv;
	projectionMatrix.R11 = 2.0f * stb_inv;
	projectionMatrix.R22 = - 2.0f * sfn_inv;

	projectionMatrix.T0 = - arl * srl_inv;
	projectionMatrix.T1 = - atb * stb_inv;
	projectionMatrix.T2 = - afn * sfn_inv;

	projectionType = RC_PROJECTION_TYPE_ORTHOGONAL;

	projectionChanged = true;
}

/*---------------------------------------------------------------------------*/

void Camera::setPerspectiveProjection(
		f32		fovy,
		f32		aspect,
		f32		nnear,
		f32		nfar
	)
{
	f32 f, t;

	if (fovy > 3.14f) {
		REPORT_WARNING("Very large FOV (%f). The FOV value should be given in radians [0, PI], and not in degrees.", fovy);
		return;
	}

	if (nnear >= nfar || nnear < 0.01f || nfar < 0.0f) {
		REPORT_WARNING("Camera values are not valid");
		return;
	}

	/* TODO: * 0.5 ? */
	f = 1.0f / tanf(fovy * 0.5f);
	t = 1.0f / (nnear - nfar);

	projectionMatrix.identity();

	projectionMatrix.R00 = f / aspect;
	projectionMatrix.R11 = f;
	projectionMatrix.R22 = (nnear + nfar) * t;
	projectionMatrix.T2 = 2.0f * nnear * nfar * t;
	projectionMatrix.N2 = -1.0f;
	projectionMatrix.N3 = 0.0f;

	projectionType = RC_PROJECTION_TYPE_PERSPECTIVE;

	projectionChanged = true;
}

/*---------------------------------------------------------------------------*/

void Camera::setProjection(
		mat4f	&matrix
	)
{
	projectionMatrix = matrix;
	projectionType = RC_PROJECTION_TYPE_FREE_TRANSFORM;

	projectionChanged = true;
}


/*---------------------------------------------------------------------------*/

Node *Camera::duplicateSelf()
{
	Camera *c = SceneGraph::createCamera(name);

	c->nearPlane = nearPlane;
	c->farPlane = farPlane;
	c->projectionMatrix = projectionMatrix;
	c->projectionType = projectionType;

	c->R = R;
	c->T = T;
	c->S = S;

	return c;
}

/*---------------------------------------------------------------------------*/

void Camera::drawSelf()
{
	if (indicatorVisible) {
		Geometry *cam;
		mat4f world;
		mat3f rot;
		vec3f pos;

		cam = (Geometry *) SceneGraph::getNode("CameraModel");

		pos = getWorldPosition();
		cam->setVector("worldpos", &pos, 3, UNIFORM_FLOAT32);

		world = getWorldMatrix();
		rot = world.rotationMatrix();
		cam->setMatrix("rotation", &rot, 3);

		cam->render();
	}
}

/*---------------------------------------------------------------------------*/

vec3f Camera::unproject(
		vec3f	screen_coord
	)
{
	mat4f vpi;
	vec4f v;
	f32 wi;
	
	vpi = getViewProjectionMatrixInverse();
	v = vec4f(screen_coord.x, screen_coord.y, screen_coord.z, 1.0f);
	v = vpi * v;
	wi = 1.0f / v.w;

	v.x *= wi;
	v.y *= wi;
	v.z *= wi;

	return vec3f(v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getProjectionMatrixInverse()
{
	if (projectionChanged) {
		float c, d;

		switch (projectionType) {
			case RC_PROJECTION_TYPE_PERSPECTIVE:
				d = 1.0f / projectionMatrix.T2;

				projectionMatrixInverse.identity();

				projectionMatrixInverse.R00 = 1.0f / projectionMatrix.R00;
				projectionMatrixInverse.R11 = 1.0f / projectionMatrix.R11;
				projectionMatrixInverse.R22 = 0.0f;

				projectionMatrixInverse.T2 = -1.0f;
				projectionMatrixInverse.N2 = d;
				projectionMatrixInverse.N3 = d * projectionMatrix.R22;

				break;
			case RC_PROJECTION_TYPE_ORTHOGONAL:
				c = 1.0f / projectionMatrix.R22;

				projectionMatrixInverse.identity();

				projectionMatrixInverse.R00 = 1.0f / projectionMatrix.R00;
				projectionMatrixInverse.R11 = 1.0f / projectionMatrix.R11;
				projectionMatrixInverse.R22 = c;

				projectionMatrixInverse.T2 = -c * projectionMatrix.T2;
				projectionMatrixInverse.N3 = 1.0f;

				break;
			default:
			case RC_PROJECTION_TYPE_FREE_TRANSFORM:

				/* Note: This is really expensive! */

				projectionMatrixInverse = projectionMatrix;
				projectionMatrixInverse.inv();

				break;
		}

		projectionChanged = false;
	}

	return projectionMatrixInverse;
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getViewMatrix()
{
	/* Yes, it's that simple! */
	return getWorldMatrixInverse();
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getViewMatrixInverse()
{
	/* Again - that simple! */
	return getWorldMatrix();
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getViewProjectionMatrix()
{
	/* LOWPRIO: Optimize */
	return getProjectionMatrix() * getViewMatrix();
}

/*---------------------------------------------------------------------------*/

mat4f Camera::getViewProjectionMatrixInverse()
{
	/* LOWPRIO: Optimize */
	return getViewMatrixInverse() * getProjectionMatrixInverse();
}

/*---------------------------------------------------------------------------*/

/* LOWPRIO: This is kind of untested! */
void Camera::calculateFrustum(
		vec4f	frustum[6]
	)
{
	u32 i;
	mat4f vp;

	vp = getViewProjectionMatrix();

	/* Left */
	frustum[0] = vp.getRow(0) + vp.getRow(3);

	/* Right */
	frustum[1] = - vp.getRow(0) + vp.getRow(3);

	/* Top */
	frustum[2] = - vp.getRow(1) + vp.getRow(3);

	/* Bottom */
	frustum[3] = vp.getRow(1) + vp.getRow(3);

	/* Near */							   
	frustum[4] = vp.getRow(2) + vp.getRow(3);

	/* Far */							   
	frustum[5] = - vp.getRow(2) + vp.getRow(3);

	for (i = 0; i < 6; i++) {
		f32 f;

		f = 1.0f / sqrtf(frustum[i].x * frustum[i].x + frustum[i].y * frustum[i].y + frustum[i].z * frustum[i].z);
		frustum[i].x *= f;
		frustum[i].y *= f;
		frustum[i].z *= f;
		frustum[i].w *= f;
	}
}

/*---------------------------------------------------------------------------*/

void Camera::toggleIndicator(
		bool		visible
	)
{
	indicatorVisible = visible;
}

/*---------------------------------------------------------------------------*/

bool Camera::getIndicator()
{
	return indicatorVisible;
}

/*---------------------------------------------------------------------------*/

void Camera::logInfo()
{
	mat4f m;

	Console::log("-=-=-=-");

	logNodeInfo();
	logTransformableInfo();

	Console::log("Projection (%s):", projectionType == RC_PROJECTION_TYPE_FREE_TRANSFORM ? "Free" :
		projectionType == RC_PROJECTION_TYPE_PERSPECTIVE ? "Perspective" : "Orthogonal");
	m = getProjectionMatrix();
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.a, m.b, m.c, m.d);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.e, m.f, m.g, m.h);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.i, m.j, m.k, m.l);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.m, m.n, m.o, m.p);
	Console::log(" ");
	Console::log("View:");
	m = getViewMatrix();
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.a, m.b, m.c, m.d);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.e, m.f, m.g, m.h);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.i, m.j, m.k, m.l);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.m, m.n, m.o, m.p);
	Console::log(" ");
	Console::log("Near / far: %f / %f", nearPlane, farPlane);
	Console::log("-=-=-=-");
}

/*====================================================[ Console commands ]====*/

static Camera *fetchCamera(
		char *nname
	)
{
	Node *node;

	node = SceneGraph::getNode(nname);

	if (!node || node->getNodeType() != NODE_CAMERA) {
		REPORT_WARNING("Camera \"%s\" not found", nname);
		return 0;
	}

	return (Camera *) node;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_camera_orthogonal = {
	"/camera_orthogonal <Camera> <left> <right> <bottom> <top> <near> <far>"
};

bool CMD_camera_orthogonal(
		u32		n_args,
		char	**args
	)
{
	Camera *cam;
	f32 f[6];

	if (!Command::checkArgCount(7, n_args, args))
		return false;

	if (!(cam = fetchCamera(args[0])))
		return false;

	if (!parseFloats(6, f, &args[1]))
		return false;

	cam->setOrthogonalProjection(f[0], f[1], f[2], f[3], f[4], f[5]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_camera_perspective = {
	"/camera_perspective <Camera> <fov> <aspect> <near> <far>"
};

bool CMD_camera_perspective(
		u32		n_args,
		char	**args
	)
{
	Camera *cam;
	f32 f[4];

	if (!Command::checkArgCount(5, n_args, args))
		return false;

	if (!(cam = fetchCamera(args[0])))
		return false;

	if (!parseFloats(4, f, &args[1]))
		return false;

	cam->setPerspectiveProjection(f[0], f[1], f[2], f[3]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_camera_indicator = {
	"/camera_indicator <Camera> <0/1>"
};

bool CMD_camera_indicator(
		u32		n_args,
		char	**args
	)
{
	Camera *camera;

	if (n_args < 1)
		return false;

	if (!(camera = fetchCamera(args[0])))
		return false;

	if (n_args == 1) {
		camera->toggleIndicator(!camera->getIndicator());
	} else {
		camera->toggleIndicator(!(args[1][0] == '0' && args[1][1] == 0));
	}

	return true;
}

/*---------------------------------------------------------------------------*/

void Camera::registerCommands()
{
	Command::add("camera_perspective", CMDHELP_camera_perspective, CMD_camera_perspective);
	Command::add("camera_orthogonal", CMDHELP_camera_orthogonal, CMD_camera_orthogonal);
	Command::add("camera_indicator", CMDHELP_camera_indicator, CMD_camera_indicator);
}

/*---------------------------------------------------------------------------*/

