#include "RenderChimp.h"
#include <cmath>
#include "Command.h"
#include "DefaultObjects.h"
#include "Transformable.h"

/*---------------------------------------------------------------------------*/

Transformable::Transformable(
		const char		*nname,
		NodeType_t		ntype
	) : Node(nname, ntype)
{
	frozen = false;
	is_transformable = true;

	worldTime = 0;
	parentWorldTime = 0;

	worldInvTime = 0;
	parentWorldInvTime = 0;

	transformTime = 0;

	worldMatrix.identity();
	worldMatrixInverse.identity();

	reset();

	axis = 0;
}

/*---------------------------------------------------------------------------*/

Transformable::~Transformable()
{
}

/*---------------------------------------------------------------------------*/

void Transformable::reset()
{
	T.x = 0.0f;
	T.y = 0.0f;
	T.z = 0.0f;

	S.x = 1.0f;
	S.y = 1.0f;
	S.z = 1.0f;

	R.identity();

	frozen = false;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::translate(
		f32		x,
		f32		y,
		f32		z
	)
{
	ASSERT(!frozen);

	T.x += x;
	T.y += y;
	T.z += z;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::translate(
		vec3f	v
	)
{
	translate(v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::scale(
		f32		x,
		f32		y,
		f32		z
	)
{
	ASSERT(!frozen);

	S.x *= x;
	S.y *= y;
	S.z *= z;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::scale(
		vec3f	v
	)
{
	scale(v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::scale(
		f32		uniform
	)
{
	ASSERT(!frozen);

	S.x *= uniform;
	S.y *= uniform;
	S.z *= uniform;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::rotate(
		f32		angle,
		f32		x,
		f32		y,
		f32		z
	)
{
	vec3f v = vec3f(x, y, z);
	mat3f m;

	f32 xx, xy, xz, yy, yz, zz;
	f32 a, c, s;

	ASSERT(!frozen);

	if (v.length() != 1.0f) {
		v.normalize();
		x = v.x;
		y = v.y;
		z = v.z;
	}

	a = angle;

	c = cos(a);
	s = sin(a);

	xx = x * x;
	xy = x * y;
	xz = x * z;
	yy = y * y;
	yz = y * z;
	zz = z * z;

	m.a = xx + (1.0f - xx) * c;
	m.b = xy * (1.0f - c) - z * s;
	m.c = xz * (1.0f - c) + y * s;

	m.d = xy * (1.0f - c) + z * s;
	m.e = yy + (1.0f - yy) * c;
	m.f = yz * (1.0f - c) - x * s;

	m.g = xz * (1.0f - c) - y * s;
	m.h = yz * (1.0f - c) + x * s;
	m.i = zz + (1.0f - zz) * c;

	R = m * R;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::rotate(
		f32		angle,
		vec3f	v
	)
{
	rotate(angle, v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::rotateX(
		f32		angle
	)
{
	f32 d, e, f;
	f32 g, h, i;
	f32 cs, sn;

	ASSERT(!frozen);

	cs = cos(angle);
	sn = sin(angle);

	d = R.d * cs - R.g * sn;
	e = R.e * cs - R.h * sn;
	f = R.f * cs - R.i * sn;

	g = R.d * sn + R.g * cs;
	h = R.e * sn + R.h * cs;
	i = R.f * sn + R.i * cs;

	R.d = d;
	R.e = e;
	R.f = f;

	R.g = g;
	R.h = h;
	R.i = i;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::rotateY(
		f32		angle
	)
{
	f32 a, b, c;
	f32 g, h, i;
	f32 cs, sn;

	cs = cos(angle);
	sn = sin(angle);

	a = R.a * cs + R.g * sn;
	b = R.b * cs + R.h * sn;
	c = R.c * cs + R.i * sn;

	g = R.g * cs - R.a * sn;
	h = R.h * cs - R.b * sn;
	i = R.i * cs - R.c * sn;

	R.a = a;
	R.b = b;
	R.c = c;

	R.g = g;
	R.h = h;
	R.i = i;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::rotateZ(
		f32		angle
	)
{
	f32 a, b, c;
	f32 d, e, f;
	f32 cs, sn;

	ASSERT(!frozen);

	cs = cos(angle);
	sn = sin(angle);

	a = R.a * cs - R.d * sn;
	b = R.b * cs - R.e * sn;
	c = R.c * cs - R.f * sn;

	d = R.a * sn + R.d * cs;
	e = R.b * sn + R.e * cs;
	f = R.c * sn + R.f * cs;

	R.a = a;
	R.b = b;
	R.c = c;

	R.d = d;
	R.e = e;
	R.f = f;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setTranslate(
		f32		x,
		f32		y,
		f32		z
	)
{
	ASSERT(!frozen);

	T.x = x;
	T.y = y;
	T.z = z;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setTranslate(
		vec3f	v
	)
{
	setTranslate(v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::setScale(
		f32		x,
		f32		y,
		f32		z
	)
{
	ASSERT(!frozen);

	S.x = x;
	S.y = y;
	S.z = z;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setScale(
		vec3f	v
	)
{
	setScale(v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::setScale(
		f32		uniform
	)
{
	ASSERT(!frozen);

	S.x = uniform;
	S.y = uniform;
	S.z = uniform;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setRotate(
		f32		angle,
		f32		x,
		f32		y,
		f32		z
	)
{
	ASSERT(!frozen);

	R.identity();
	rotate(angle, x, y, z);
}

/*---------------------------------------------------------------------------*/

void Transformable::setRotate(
		f32		angle,
		vec3f	v
	)
{
	setRotate(angle, v.x, v.y, v.z);
}

/*---------------------------------------------------------------------------*/

void Transformable::setRotateX(
		f32		angle
	)
{
	f32 cs, sn;

	ASSERT(!frozen);

	cs = cos(angle);
	sn = sin(angle);

	R.a = 1;
	R.b = 0;
	R.c = 0;

	R.d = 0;
	R.e = cs;
	R.f = -sn;

	R.g = 0;
	R.h = sn;
	R.i = cs;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setRotateY(
		f32		angle
	)
{
	f32 cs, sn;

	ASSERT(!frozen);

	cs = cos(angle);
	sn = sin(angle);

	R.a = cs;
	R.b = 0;
	R.c = sn;

	R.d = 0;
	R.e = 1;
	R.f = 0;

	R.g = -sn;
	R.h = 0;
	R.i = cs;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

void Transformable::setRotateZ(
		f32		angle
	)
{
	f32 cs, sn;

	ASSERT(!frozen);

	cs = cos(angle);
	sn = sin(angle);

	R.a = cs;
	R.b = -sn;
	R.c = 0;

	R.d = sn;
	R.e = cs;
	R.f = 0;

	R.g = 0;
	R.h = 0;
	R.i = 1;

	transformTime++;
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getTranslationMatrix()
{
	return mat4f(1, 0, 0, T.x,
				 0, 1, 0, T.y,
				 0, 0, 1, T.z,
				 0, 0, 0, 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getRotationMatrix()
{
	return mat4f(R.a, R.b, R.c, 0,
				 R.d, R.e, R.f, 0,
				 R.g, R.h, R.i, 0,
				 0  , 0  , 0  , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getScaleMatrix()
{
	return mat4f(S.x, 0  , 0  , 0,
				 0  , S.y, 0  , 0,
				 0  , 0  , S.z, 0,
				 0  , 0  , 0  , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getTranslationMatrixInverse()
{
	return mat4f(1, 0, 0, -T.x,
				 0, 1, 0, -T.y,
				 0, 0, 1, -T.z,
				 0, 0, 0, 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getRotationMatrixInverse()
{
	return mat4f(R.a, R.d, R.g, 0,
				 R.b, R.e, R.h, 0,
				 R.c, R.f, R.i, 0,
				 0  , 0  , 0  , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getScaleMatrixInverse()
{
	return mat4f(1.0f/S.x, 0       , 0       , 0,
				 0       , 1.0f/S.y, 0       , 0,
				 0       , 0       , 1.0f/S.z, 0,
				 0       , 0       , 0       , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getTranslationRotationMatrix()
{
	return mat4f(R.a, R.b, R.c, T.x,
				 R.d, R.e, R.f, T.y,
				 R.g, R.h, R.i, T.z,
				 0	, 0	 , 0  , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getLocalMatrix()
{
	return mat4f(R.a*S.x, R.b*S.y, R.c*S.z, T.x,
				 R.d*S.x, R.e*S.y, R.f*S.z, T.y,
				 R.g*S.x, R.h*S.y, R.i*S.z, T.z,
				 0		, 0		 , 0	  , 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getLocalMatrixInverse()
{
	f32 a, b, c;
	f32 d, e, f;
	f32 g, h, i;

	vec3f X = vec3f(1.0f / S.x, 1.0f / S.y, 1.0f / S.z);

	a = R.a * X.x;
	b = R.b * X.y;
	c = R.c * X.z;

	d = R.d * X.x;
	e = R.e * X.y;
	f = R.f * X.z;

	g = R.g * X.x;
	h = R.h * X.y;
	i = R.i * X.z;

	return mat4f(a, d, g, -(T.x * a + T.y * d + T.z * g),
				 b, e, h, -(T.x * b + T.y * e + T.z * h),
				 c, f, i, -(T.x * c + T.y * f + T.z * i),
				 0			, 0			, 0			, 1);
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getWorldMatrix()
{
	i32 i;

	Node *curr;
	Transformable *list[RC_SCENE_GRAPH_MAX_DEPTH];
	u32 n_list;

	mat4f w;

	if (frozen) {
		return worldMatrix;
	}

	curr = this;
	n_list = 0;

	while (curr) {

		ASSERT(curr->isActive);

		if (curr->is_transformable) {
			list[n_list++] = (Transformable *) curr;

			ASSERT(n_list < RC_SCENE_GRAPH_MAX_DEPTH); // UNLIKELY
		}

		curr = curr->getParent();
	}

	if (n_list == 1) {
		transformTime++; /* This might not be needed at all */

		worldMatrix = getLocalMatrix();
		worldTime = transformTime;

		return worldMatrix;
	}

	i = n_list - 1;

	if (list[i]->transformTime != list[i]->worldTime) {

		list[i]->worldMatrix = list[i]->getLocalMatrix();
		list[i]->worldTime = list[i]->transformTime;

		i--;

	} else {

		do {
			i--;
		} while (i >= 0 &&
			list[i]->parentWorldTime == list[i + 1]->worldTime &&
			list[i]->worldTime == list[i]->transformTime);

		if (i < 0)
			return worldMatrix;

	}

	w = list[i + 1]->worldMatrix;

	while (i >= 0) {
		w = w.affineMul(list[i]->getLocalMatrix());

		list[i]->transformTime++;

		list[i]->worldMatrix = w;
		list[i]->worldTime = list[i]->transformTime;
		list[i]->parentWorldTime = list[i + 1]->worldTime;

		i--;
	}

	return worldMatrix;
}

/*---------------------------------------------------------------------------*/

mat4f Transformable::getWorldMatrixInverse()
{
	i32 i;

	Node *curr;
	Transformable *list[RC_SCENE_GRAPH_MAX_DEPTH];
	u32 n_list;

	mat4f w;

	if (frozen) {
		return worldMatrixInverse;
	}

	curr = this;
	n_list = 0;

	while (curr) {

		ASSERT(curr->isActive);

		if (curr->is_transformable) {
			list[n_list++] = (Transformable *) curr;

			ASSERT(n_list < RC_SCENE_GRAPH_MAX_DEPTH); // UNLIKELY
		}

		curr = curr->getParent();
	}

	if (n_list == 1) {
		transformTime++; /* This might not be needed at all */

		worldMatrixInverse = getLocalMatrixInverse();
		worldInvTime = transformTime;

		return worldMatrixInverse;
	}

	i = n_list - 1;

	if (list[i]->transformTime != list[i]->worldInvTime) {

		list[i]->worldMatrixInverse = list[i]->getLocalMatrixInverse();
		list[i]->worldInvTime = list[i]->transformTime;

		i--;

	} else {

		do {
			i--;
		} while (i >= 0 &&
			list[i]->parentWorldInvTime == list[i + 1]->worldInvTime &&
			list[i]->worldInvTime == list[i]->transformTime);

		if (i < 0)
			return worldMatrixInverse;

	}

	w = list[i + 1]->worldMatrixInverse;

	while (i >= 0) {
		//w = list[i]->getLocalMatrixInverse() * w;
		w = list[i]->getLocalMatrixInverse().affineMul(w);

		list[i]->transformTime++;

		list[i]->worldMatrixInverse = w;
		list[i]->worldInvTime = list[i]->transformTime;
		list[i]->parentWorldInvTime = list[i + 1]->worldInvTime;

		i--;
	}

	return worldMatrixInverse;

}

/*---------------------------------------------------------------------------*/

mat3f Transformable::getWorldNormalMatrix()
{
	mat4f w = getWorldMatrix();

	return mat3f(w.a, w.b, w.c,
				 w.e, w.f, w.g,
				 w.i, w.j, w.k);
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getWorldPosition(
		vec3f pos
	)
{
	vec4f v = vec4f(pos.x, pos.y, pos.z, 1.0f);
	mat4f w = getWorldMatrix();

	v = w * v;

	return v.xyz();
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getWorldPosition()
{
	return getWorldPosition(vec3f(0.0f, 0.0f, 0.0f));
}

/*---------------------------------------------------------------------------*/

mat3f Transformable::getRotation()
{
	return R;
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getTranslation()
{
	return T;
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getScale()
{
	return S;
}

/*---------------------------------------------------------------------------*/

void Transformable::setFreeze(
		bool	do_freeze
	)
{
	frozen = do_freeze;
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getLocalFront()
{
	return -vec3f(R.c, R.f, R.i);
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getLocalUp()
{
	return vec3f(R.b, R.e, R.h);
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getLocalRight()
{
	return vec3f(R.a, R.d, R.g);
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getWorldFront()
{
	mat4f w;
	vec4f v;

	w = getWorldMatrix();
	v = w.getCol(2);

	return - v.xyz();
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getWorldUp()
{
	mat4f w;
	vec4f v;

	w = getWorldMatrix();
	v = w.getCol(1);

	return v.xyz();
}

/*---------------------------------------------------------------------------*/

vec3f Transformable::getWorldRight()
{
	mat4f w;
	vec4f v;

	w = getWorldMatrix();
	v = w.getCol(0);

	return v.xyz();
}

/*---------------------------------------------------------------------------*/

void Transformable::detached()
{
	parentWorldTime = 0;
	parentWorldInvTime = 0;
}

/*---------------------------------------------------------------------------*/

/**
* LookTowards
*
* Note 1: Function will exit if front_vec is (close to) parallel to the Y-axis; supply your own up_vec if this is the case.
* Note 2: Not fully tested, use at own risk...
*
* Example:
*	Make camera look at 'my_node'. Note that world space positions are used.
*	camera->lookAt( my_node->getWorldPosition() - camera->getWorldPosition() );
*/
void Transformable::lookTowards(
		vec3f	front_vec,
		vec3f	up_vec
	)
{
	vec3f right;
	vec3f up;
	vec3f prev_up;

	front_vec.normalize();
	up_vec.normalize();

	if (abs(up_vec.dot(front_vec)) > 0.99999f) {
		return;
	}

	if (parent && parent->is_transformable) {
		mat3f mat;
		R = ((Transformable *) parent)->getWorldMatrix().rotationMatrix();
		R.inv();

		prev_up = up_vec;

		right = front_vec.cross(prev_up);
		up = right.cross(front_vec);

		right.normalize();
		up.normalize();

		mat.setCol(0, right);
		mat.setCol(1, up);
		mat.setCol(2, -front_vec);

		R = R * mat;

	} else {
		prev_up = up_vec;

		right = front_vec.cross(prev_up);
		up = right.cross(front_vec);

		right.normalize();
		up.normalize();

		R.setCol(0, right);
		R.setCol(1, up);
		R.setCol(2, -front_vec);
	}
}

/*---------------------------------------------------------------------------*/

void Transformable::lookTowards(
		vec3f	front_vec
	)
{
	lookTowards(front_vec, vec3f(0.0f, 1.0f, 0.0f));
}

/*---------------------------------------------------------------------------*/

void Transformable::logTransformableInfo()
{
	mat4f m;

	Console::log("Local transform:");
	m = getLocalMatrix();
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.a, m.b, m.c, m.d);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.e, m.f, m.g, m.h);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.i, m.j, m.k, m.l);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.m, m.n, m.o, m.p);
	Console::log(" ");
	Console::log("World transform:");
	m = getWorldMatrix();
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.a, m.b, m.c, m.d);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.e, m.f, m.g, m.h);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.i, m.j, m.k, m.l);
	Console::log("  %.3f ; %.3f ; %.3f ; %.3f", m.m, m.n, m.o, m.p);
	Console::log(" ");
}

/*====================================================[ Console commands ]====*/

static Transformable *fetchTransformable(
		char *nname
	)
{
	Node *node;

	node = SceneGraph::getNode(nname);

	if (!node || !node->isTransformable()) {
		REPORT_WARNING("Transformable \"%s\" not found", nname);
		return 0;
	}

	return (Transformable *) node;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_set_translate = {
	"/set_translate <Transformable> <x> <y> <z>"
};

bool CMD_set_translate(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[3];

	if (!Command::checkArgCount(4, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	t->setTranslate(f[0], f[1], f[2]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_translate = {
	"/translate <Transformable> <x> <y> <z>"
};

bool CMD_translate(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[3];

	if (!Command::checkArgCount(4, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	t->translate(f[0], f[1], f[2]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_set_rotate = {
	"/set_rotate <Transformable> <angle> <x> <y> <z>"
};

bool CMD_set_rotate(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[4];

	if (!Command::checkArgCount(5, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(4, f, &args[1]))
		return false;

	t->setRotate(f[0], f[1], f[2], f[3]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_rotate = {
	"/rotate <Transformable> <x> <y> <z>"
};

bool CMD_rotate(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[4];

	if (!Command::checkArgCount(5, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	t->rotate(f[0], f[1], f[2], f[3]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_set_scale = {
	"/set_scale <Transformable> <x> <y> <z>"
};

bool CMD_set_scale(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[3];

	if (!Command::checkArgCount(4, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	t->setScale(f[0], f[1], f[2]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_scale = {
	"/scale <Transformable> <x> <y> <z>"
};

bool CMD_scale(
		u32		n_args,
		char	**args
	)
{
	Transformable *t;
	f32 f[3];

	if (!Command::checkArgCount(4, n_args, args))
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (!parseFloats(3, f, &args[1]))
		return false;

	t->scale(f[0], f[1], f[2]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_axis = {
	"/axis <Transformable> <0/1>"
};

bool CMD_axis(
		u32		n_args,
		char	**args
	)
{
	bool add_axis;
	Transformable *t;

	if (n_args < 1)
		return false;

	if (!(t = fetchTransformable(args[0])))
		return false;

	if (n_args == 1) {
		if (t->axis) {
			add_axis = false;
		} else {
			add_axis = true;
		}
	} else {
		bool d = !(args[1][0] == '0' && args[1][1] == 0);

		if ((d && t->axis) || (!d && !t->axis))
			return true;

		add_axis = d;
	}

	if (add_axis) {
		t->axis = (Group *) DefaultObjects::axisGroup->duplicate(0, true);
		t->attachChild(t->axis);
	} else {
		t->detachChild(t->axis);
		SceneGraph::deleteNode(t->axis);
		t->axis = 0;
	}

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_axis_origin = {
	"/axis_origin <0/1>"
};

bool CMD_axis_origin(
		u32		n_args,
		char	**args
	)
{
	bool add_axis;

	if (n_args == 0) {
		if (DefaultObjects::helperAxis) {
			add_axis = false;
		} else {
			add_axis = true;
		}
	} else {
		bool d = !(args[0][0] == '0' && args[0][1] == 0);

		if ((d && DefaultObjects::helperAxis) || (!d && !DefaultObjects::helperAxis))
			return true;

		add_axis = d;
	}

	if (add_axis) {
		Geometry *g;
		DefaultObjects::helperAxis = (Group *) DefaultObjects::axisGroup->duplicate("AxisOrigin", true);

		g = (Geometry *) DefaultObjects::helperAxis->getChild("AxisX", 1);
		g->setScale(0.75f, 5.0f, 0.75f);

		g = (Geometry *) DefaultObjects::helperAxis->getChild("AxisY", 1);
		g->setScale(0.75f, 5.0f, 0.75f);

		g = (Geometry *) DefaultObjects::helperAxis->getChild("AxisZ", 1);
		g->setScale(0.75f, 5.0f, 0.75f);

	} else {
		SceneGraph::deleteNode(DefaultObjects::helperAxis);
		DefaultObjects::helperAxis = 0;
	}

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_grid = {
	"/grid <0/1>"
};

bool CMD_grid(
		u32		n_args,
		char	**args
	)
{
	bool add_grid;

	if (n_args == 0) {
		if (DefaultObjects::helperGrid) {
			add_grid = false;
		} else {
			add_grid = true;
		}
	} else {
		bool d = !(args[0][0] == '0' && args[0][1] == 0);

		if ((d && DefaultObjects::helperGrid) || (!d && !DefaultObjects::helperGrid))
			return true;

		add_grid = d;
	}

	if (add_grid) {
		DefaultObjects::helperGrid = (Geometry *) DefaultObjects::gridMesh->duplicate("GridOrigin", true);
	} else {
		SceneGraph::deleteNode(DefaultObjects::helperGrid);
		DefaultObjects::helperGrid = 0;
	}

	return true;
}

/*---------------------------------------------------------------------------*/

void Transformable::registerCommands()
{
	Command::add("set_translate", CMDHELP_set_translate, CMD_set_translate);
	Command::add("translate", CMDHELP_translate, CMD_translate);

	Command::add("set_rotate", CMDHELP_set_rotate, CMD_set_rotate);
	Command::add("rotate", CMDHELP_rotate, CMD_rotate);

	Command::add("set_scale", CMDHELP_set_scale, CMD_set_scale);
	Command::add("scale", CMDHELP_scale, CMD_scale);

	Command::add("axis", CMDHELP_axis, CMD_axis);

	/* LOWPRIO: Hacky commands... */
	Command::add("axis_origin", CMDHELP_axis_origin, CMD_axis_origin);
	Command::add("grid", CMDHELP_grid, CMD_grid);
}

/*---------------------------------------------------------------------------*/
