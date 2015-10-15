
#ifdef VMATH

#include <math.h>

/* Functions */

/* Assignment */
template<class T>
inline vec2<T> &vec2<T>::operator=(const vec2<T> &v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<class T>
inline vec2<T> &vec2<T>::operator=(T v)
{
	x = v;
	y = v;
	return *this;
}


/* Comparison */
template<class T>
inline bool vec2<T>::operator==(const vec2<T> &v) const
{
	return x == v.x && y == v.y;
}

template<class T>
inline bool vec2<T>::operator!=(const vec2<T> &v) const
{
	return x != v.x || y != v.y;
}

/* Addition */
template<class T> 
inline vec2<T> vec2<T>::operator+(const vec2<T> &v) const
{
	return vec2<T>(x + v.x, y + v.y);
}

template<class T> 
inline vec2<T> &vec2<T>::operator+=(const vec2<T> &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<class T> 
inline vec2<T> vec2<T>::operator+(T v) const
{
	return vec2<T>(x + v, y + v);
}

template<class T> 
inline vec2<T> &vec2<T>::operator+=(T v)
{
	x += v;
	y += v;
	return *this;
}

template<class T> 
inline vec2<T> vec2<T>::operator+() const
{
	return vec2<T>(x, y);
}


/* Subtract */
template<class T> 
inline vec2<T> vec2<T>::operator-(const vec2<T> &v) const
{
	return vec2<T>(x - v.x, y - v.y);
}

template<class T> 
inline vec2<T> &vec2<T>::operator-=(const vec2<T> &v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template<class T> 
inline vec2<T> vec2<T>::operator-(T v) const
{
	return vec2<T>(x - v, y - v);
}

template<class T> 
inline vec2<T> &vec2<T>::operator-=(T v)
{
	x -= v;
	y -= v;
	return *this;
}

template<class T> 
inline vec2<T> vec2<T>::operator-() const
{
	return vec2<T>(-x, -y);
}


/* Element-wise multiplication */
template<class T> 
inline vec2<T> vec2<T>::operator&(const vec2<T> &v) const
{
	return vec2<T>(x * v.x, y * v.y);
}

template<class T> 
inline vec2<T> &vec2<T>::operator&=(const vec2<T> &v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

/* Multiplication/Dot product */
template<class T> 
inline vec2<T> vec2<T>::operator*(const mat2<T> &v) const
{
	return vec2<T>(
		v.a * x + v.c * y,
		v.b * x + v.d * y
	);
}

template<class T> 
inline vec2<T> &vec2<T>::operator*=(const mat2<T> &v)
{
	vec2<T> nv = vec2<T>(v * (*this));
	x = nv.x;
	y = nv.y;
	return *this;
}

template<class T> 
inline vec2<T> vec2<T>::operator*(T v) const
{
	return vec2<T>(x * v, y * v);
}

template<class T>
inline vec2<T> &vec2<T>::operator*=(T v)
{
	x *= v;
	y *= v;
	return *this;
}

template<class T>
inline T vec2<T>::dot(const vec2<T> &v) const
{
	return x * v.x + y * v.y;
}

template<class T>
inline T vec2<T>::operator*(const vec2<T> &v) const
{
	return dot(v);
}

/* Division */
template<class T>
inline vec2<T> vec2<T>::operator/(const vec2<T> &v) const
{
	return vec2<T>(x / v.x, y / v.y);
}

template<class T>
inline vec2<T> &vec2<T>::operator/=(const vec2<T> &v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::operator/(T v) const
{
	T t = T(1) / v;
	return vec2<T>(x * t, y * t);
}

template<class T>
inline vec2<T> &vec2<T>::operator/=(T v)
{
	T t = T(1) / v;
	x *= t;
	y *= t;
	return *this;
}


/* Misc useful stuff */
template<class T>
inline T vec2<T>::minElement() const
{
	return x < y ? x : y;
}

template<class T>
inline T vec2<T>::maxElement() const
{
	return x > y ? x : y;
}

template<class T>
inline vec2<T> &vec2<T>::clamp(T min, T max)
{
	x = sclamp(x, min, max);
	y = sclamp(y, min, max);
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::getClamped(T min, T max) const
{
	vec2<T> r;
	r.x = sclamp(x, min, max);
	r.y = sclamp(y, min, max);
	return r;
}

template<class T>
inline vec2<T> &vec2<T>::clamp(const vec2<T> &min, const vec2<T> &max)
{
	x = sclamp(x, min.x, max.x);
	y = sclamp(y, min.y, max.y);
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::getClamped(const vec2<T> &min, const vec2<T> &max) const
{
	vec2<T> r;
	r.x = sclamp(x, min.x, max.x);
	r.y = sclamp(y, min.y, max.y);
	return r;
}

template<class T>
inline vec2<T> &vec2<T>::saturate()
{
	x = ssaturate(x);
	y = ssaturate(y);
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::getSaturated() const
{
	vec2<T> r;
	r.x = ssaturate(x);
	r.y = ssaturate(y);
	return r;
}

template<class T>
inline vec2<T> &vec2<T>::normalize()
{
	T t = T(1) / length();
	x *= t;
	y *= t;
	return *this;
}

template<class T>
inline vec2<T> vec2<T>::getNormalized() const
{
	vec2<T> r;
	T t = T(1) / length();
	r.x = x * t;
	r.y = y * t;
	return r;
}

template<class T>
inline T vec2<T>::len() const
{
	return sqrt(x * x + y * y);
}

template<class T>
inline T vec2<T>::length() const
{
	return len();
}

template<class T>
inline T vec2<T>::len2() const
{
	return x * x + y * y;
}

template<class T>
inline T vec2<T>::length2() const
{
	return len2();
}

template<class T>
inline T vec2<T>::lengthSquared() const
{
	return len2();
}

template<class T>
inline vec2<T> vec2<T>::reflection(const vec2<T> &v) const
{
	T nv2 = dot(v) * ((T) 2);
	return vec2<T>(x + v.x, y + v.y) * nv2;
}

template<class T>
inline vec2<T> vec2<T>::refraction(const vec2<T> &v, T r) const
{
	T w, k, wk;
	w = -dot(v) * r;
	k = sqrt(T(1) + (w - r) * (w + r));
	wk = w - k;
	return vec2<T>(r * x + wk * v.x,
				   r * y + wk * v.y);
}

template<class T>
T vec2<T>::projectionLengthOn(const vec2<T> &v) const
{
	return dot(v) / v.len();
}

template<class T>
vec2<T> vec2<T>::projectOn(const vec2<T> &v) const
{
	return v * (dot(v) / (v.dot(v)));
}

/* Scalar-vector operations */
template<class T>
vec2<T> operator+(T lv, vec2<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec2<T> operator-(T lv, vec2<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec2<T> operator*(T lv, vec2<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec2<T> operator/(T lv, vec2<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	return d;
}

template<class T>
vec2<T> operator+(T lv, const vec2<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec2<T> operator-(T lv, const vec2<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec2<T> operator*(T lv, const vec2<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec2<T> operator/(T lv, const vec2<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	return d;
}






/* Functions */

/* Assignment */
template<class T>
inline vec3<T> &vec3<T>::operator=(const vec3<T> &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<class T>
inline vec3<T> &vec3<T>::operator=(T v)
{
	x = v;
	y = v;
	z = v;
	return *this;
}


/* Comparison */
template<class T>
inline bool vec3<T>::operator==(const vec3<T> &v) const
{
	return x == v.x &&
		y == v.y &&
		z == v.z;
}

template<class T>
inline bool vec3<T>::operator!=(const vec3<T> &v) const
{
	return x != v.x ||
		y != v.y ||
		z != v.z;
}


/* Addition */
template<class T> 
inline vec3<T> vec3<T>::operator+(const vec3<T> &v) const
{
	return vec3<T>(x + v.x, y + v.y, z + v.z);
}

template<class T> 
inline vec3<T> &vec3<T>::operator+=(const vec3<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template<class T> 
inline vec3<T> vec3<T>::operator+(T v) const
{
	return vec3<T>(x + v, y + v, z + v);
}

template<class T> 
inline vec3<T> &vec3<T>::operator+=(T v)
{
	x += v;
	y += v;
	z += v;
	return *this;
}

template<class T> 
inline vec3<T> vec3<T>::operator+() const
{
	return vec3<T>(x, y, z);
}


/* Subtract */
template<class T> 
inline vec3<T> vec3<T>::operator-(const vec3<T> &v) const
{
	return vec3<T>(x - v.x, y - v.y, z - v.z);
}

template<class T> 
inline vec3<T> &vec3<T>::operator-=(const vec3<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template<class T> 
inline vec3<T> vec3<T>::operator-(T v) const
{
	return vec3<T>(x - v, y - v, z - v);
}

template<class T> 
inline vec3<T> &vec3<T>::operator-=(T v)
{
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

template<class T> 
inline vec3<T> vec3<T>::operator-() const
{
	return vec3<T>(-x, -y, -z);
}


/* Element-wise multiplication */
template<class T> 
inline vec3<T> vec3<T>::operator&(const vec3<T>	&v) const
{
	return vec3<T>(x * v.x, y * v.y, z * v.z);
}

template<class T> 
inline vec3<T> &vec3<T>::operator&=(const vec3<T> &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

/* Multiplication/Dot product */
template<class T> 
inline vec3<T> vec3<T>::operator*(const mat3<T> &v) const
{
	return vec3<T>(
		v.a * x + v.d * y + v.g * z,
		v.b * x + v.e * y + v.h * z,
		v.c * x + v.f * y + v.i * z
	);
}

template<class T> 
inline vec3<T> &vec3<T>::operator*=(const mat3<T> &v)
{
	vec3<T> nv = vec3<T>(v * (*this));
	x = nv.x;
	y = nv.y;
	z = nv.z;
	return *this;
}

template<class T> 
inline vec3<T> vec3<T>::operator*(T v) const
{
	return vec3<T>(x * v, y * v, z * v);
}

template<class T>
inline vec3<T> &vec3<T>::operator*=(T v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

template<class T>
inline T vec3<T>::dot(const vec3<T> &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

template<class T>
inline T vec3<T>::operator*(const vec3<T> &v) const
{
	return dot(v);
}


/* Cross product */
template<class T>
inline vec3<T> vec3<T>::cross(const vec3<T> &v) const
{
	return vec3<T>(y * v.z - z * v.y,
				 z * v.x - x * v.z,
				 x * v.y - y * v.x);
}

template<class T>
inline T vec3<T>::crossX(const vec3<T> &v) const
{
	return y * v.z - z * v.y;
}

template<class T>
inline T vec3<T>::crossY(const vec3<T> &v) const
{
	return z * v.x - x * v.z;
}

template<class T>
inline T vec3<T>::crossZ(const vec3<T> &v) const
{
	return x * v.y - y * v.x;
}

template<class T>
inline vec3<T> vec3<T>::operator^(const vec3<T> &v) const
{
	return cross(v);
}


/* Division */
template<class T>
inline vec3<T> vec3<T>::operator/(const vec3<T>	&v) const
{
	return vec3<T>(x / v.x, y / v.y, z / v.z);
}

template<class T>
inline vec3<T> &vec3<T>::operator/=(const vec3<T> &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

template<class T>
inline vec3<T> vec3<T>::operator/(T v) const
{
	T t = T(1) / v;
	return vec3<T>(x * t, y * t, z * t);
}

template<class T>
inline vec3<T> &vec3<T>::operator/=(T v)
{
	T t = T(1) / v;
	x *= t;
	y *= t;
	z *= t;
	return *this;
}


/* Misc useful stuff */
template<class T>
inline T vec3<T>::minElement() const
{
	return x < y ? (x < z ? x : z) : (y < z ? y : z);
}

template<class T>
inline T vec3<T>::maxElement() const
{
	return x > y ? (x > z ? x : z) : (y > z ? y : z);
}

template<class T>
inline vec3<T> &vec3<T>::clamp(T min, T max)
{
	x = sclamp(x, min, max);
	y = sclamp(y, min, max);
	z = sclamp(z, min, max);
	return *this;
}

template<class T>
inline vec3<T> vec3<T>::getClamped(T min, T max) const
{
	vec3<T> r;
	r.x = sclamp(x, min, max);
	r.y = sclamp(y, min, max);
	r.z = sclamp(z, min, max);
	return r;
}

template<class T>
inline vec3<T> &vec3<T>::clamp(const vec3<T> &min, const vec3<T> &max)
{
	x = sclamp(x, min.x, max.x);
	y = sclamp(y, min.y, max.y);
	z = sclamp(z, min.z, max.z);
	return *this;
}

template<class T>
inline vec3<T> vec3<T>::getClamped(const vec3<T> &min,const vec3<T> &max) const
{
	vec3<T> r;
	r.x = sclamp(x, min.x, max.x);
	r.y = sclamp(y, min.y, max.y);
	r.z = sclamp(z, min.z, max.z);
	return r;
}

template<class T>
inline vec3<T> &vec3<T>::saturate()
{
	x = ssaturate(x);
	y = ssaturate(y);
	z = ssaturate(z);
	return *this;
}

template<class T>
inline vec3<T> vec3<T>::getSaturated() const
{
	vec3<T> r;
	r.x = ssaturate(x);
	r.y = ssaturate(y);
	r.z = ssaturate(z);
	return r;
}

template<class T>
inline vec3<T> &vec3<T>::normalize()
{
	T t = T(1) / length();
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

template<class T>
inline vec3<T> vec3<T>::getNormalized() const
{
	vec3<T> r;
	T t = T(1) / length();
	r.x = x * t;
	r.y = y * t;
	r.z = z * t;
	return r;
}

template<class T>
inline T vec3<T>::len() const
{
	return sqrtf(x * x + y * y + z * z);
}

template<class T>
inline T vec3<T>::length() const
{
	return len();
}

template<class T>
inline T vec3<T>::len2() const
{
	return x * x + y * y + z * z;
}

template<class T>
inline T vec3<T>::length2() const
{
	return len2();
}

template<class T>
inline T vec3<T>::lengthSquared() const
{
	return len2();
}

template<class T>
inline vec3<T> vec3<T>::reflection(const vec3<T> &v) const
{
	T nv2 = dot(v) * ((T) 2);
	return vec3<T>(v.x * nv2 - x, v.y * nv2 - y, v.z * nv2 - z);
}

template<class T>
inline vec3<T> vec3<T>::refraction(const vec3<T> &v, T r) const
{
	T w, k, wk;
	w = -dot(v) * r;
	k = sqrt(T(1) + (w - r) * (w + r));
	wk = w - k;
	return vec3<T>(r * x + wk * v.x,
				   r * y + wk * v.y,
				   r * z + wk * v.z);
}

template<class T>
f32 vec3<T>::projectionLengthOn(const vec3<T> &v) const
{
	return dot(v) / v.len();
}

template<class T>
vec3<T> vec3<T>::projectOn(const vec3<T> &v) const
{
	return v * (dot(v) / (v.dot(v)));
}

template<class T>
inline vec2<T> vec3<T>::xy() const
{
	return vec2<T>(x, y);
}

template<class T>
inline vec4<T> vec3<T>::xyz1() const
{
	return vec4<T>(x, y, z, 1);
}

template<class T>
inline vec4<T> vec3<T>::xyz0() const
{
	return vec4<T>(x, y, z, 0);
}

template<class T>
inline vec3<T> operator+(T lv, vec3<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec3<T> operator-(T lv, vec3<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec3<T> operator*(T lv, vec3<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec3<T> operator/(T lv, vec3<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	d.z = lv / rv.z;
	return d;
}

template<class T>
inline vec3<T> operator+(T lv, const vec3<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec3<T> operator-(T lv, const vec3<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec3<T> operator*(T lv, const vec3<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec3<T> operator/(T lv, const vec3<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	d.z = lv / rv.z;
	return d;
}










/* Functions */

/* Assignment */
template<class T>
inline vec4<T> &vec4<T>::operator=(const vec4<T> &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<class T>
inline vec4<T> &vec4<T>::operator=(T v)
{
	x = v;
	y = v;
	z = v;
	w = v;
	return *this;
}


/* Comparison */
template<class T>
inline bool vec4<T>::operator==(const vec4<T> &v) const
{
	return x == v.x &&
		y == v.y &&
		z == v.z &&
		w == v.w;
}

template<class T>
inline bool vec4<T>::operator!=(const vec4<T> &v) const
{
	return x != v.x ||
		y != v.y ||
		z != v.z ||
		w != v.w;
}


/* Addition */
template<class T> 
inline vec4<T> vec4<T>::operator+(const vec4<T> &v) const
{
	return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<class T> 
inline vec4<T> &vec4<T>::operator+=(const vec4<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

template<class T> 
inline vec4<T> vec4<T>::operator+(T v) const
{
	return vec4<T>(x + v, y + v, z + v, w + v);
}

template<class T> 
inline vec4<T> &vec4<T>::operator+=(T v)
{
	x += v;
	y += v;
	z += v;
	w += v;
	return *this;
}

template<class T> 
inline vec4<T> vec4<T>::operator+() const
{
	return vec4<T>(x, y, z, w);
}


/* Subtract */
template<class T> 
inline vec4<T> vec4<T>::operator-(const vec4<T> &v) const
{
	return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<class T> 
inline vec4<T> &vec4<T>::operator-=(const vec4<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return this;
}

template<class T> 
inline vec4<T> vec4<T>::operator-(T v) const
{
	return vec4<T>(x - v, y - v, z - v, w - v);
}

template<class T> 
inline vec4<T> &vec4<T>::operator-=(T v)
{
	x -= v;
	y -= v;
	z -= v;
	w -= v;
	return *this;
}

template<class T> 
inline vec4<T> vec4<T>::operator-() const
{
	return vec4<T>(-x, -y, -z, -w);
}


/* Element-wise multiplication */
template<class T> 
inline vec4<T> vec4<T>::operator&(const vec4<T> &v) const
{
	return vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
}

template<class T> 
inline vec4<T> &vec4<T>::operator&=(const vec4<T> &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

/* Multiplication/Dot product */
template<class T> 
inline vec4<T> vec4<T>::operator*(const mat4<T> &v) const
{
	return vec4<T>(
		v.a * x + v.e * y + v.i * z + v.m * w,
		v.b * x + v.f * y + v.j * z + v.n * w,
		v.c * x + v.g * y + v.k * z + v.o * w,
		v.d * x + v.h * y + v.l * z + v.p * w
	);
}

template<class T> 
inline vec4<T> &vec4<T>::operator*=(const mat4<T> &v)
{
	vec4<T> nv = vec4<T>(v * (*this));
	x = nv.x;
	y = nv.y;
	z = nv.z;
	w = nv.w;
	return *this;
}

template<class T> 
inline vec4<T> vec4<T>::operator*(T v) const
{
	return vec4<T>(x * v, y * v, z * v, w * v);
}

template<class T>
inline vec4<T> &vec4<T>::operator*=(T v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;
	return *this;
}

template<class T>
inline T vec4<T>::dot(const vec4<T> &v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<class T>
inline T vec4<T>::operator*(const vec4<T> &v) const
{
	return dot(v);
}


/* Division */
template<class T>
inline vec4<T> vec4<T>::operator/(const vec4<T> &v) const
{
	return vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
}

template<class T>
inline vec4<T> &vec4<T>::operator/=(const vec4<T> &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

template<class T>
inline vec4<T> vec4<T>::operator/(T v) const
{
	T t = T(1) / v;
	return vec4<T>(x * t, y * t, z * t, w * t);
}

template<class T>
inline vec4<T> &vec4<T>::operator/=(T v)
{
	T t = T(1) / v;
	x *= t;
	y *= t;
	z *= t;
	w *= t;
	return *this;
}


/* Misc useful stuff */
template<class T>
inline T vec4<T>::minElement() const
{
	return smin4<T>(x, y, z, w);
}

template<class T>
inline T vec4<T>::maxElement() const
{
	return smax4<T>(x, y, z, w);
}

template<class T>
inline vec4<T> &vec4<T>::clamp(T min, T max)
{
	x = sclamp(x, min, max);
	y = sclamp(y, min, max);
	z = sclamp(z, min, max);
	w = sclamp(w, min, max);
	return *this;
}

template<class T>
inline vec4<T> vec4<T>::getClamped(T min, T max) const
{
	vec4<T> r;
	r.x = sclamp(x, min, max);
	r.y = sclamp(y, min, max);
	r.z = sclamp(z, min, max);
	r.w = sclamp(w, min, max);
	return r;
}

template<class T>
inline vec4<T> &vec4<T>::clamp(const vec4<T> &min, const vec4<T> &max)
{
	x = sclamp(x, min.x, max.x);
	y = sclamp(y, min.y, max.y);
	z = sclamp(z, min.z, max.z);
	w = sclamp(w, min.w, max.w);
	return *this;
}

template<class T>
inline vec4<T> vec4<T>::getClamped(const vec4<T> &min, const vec4<T> &max) const
{
	vec4<T> r;
	r.x = sclamp(x, min.x, max.x);
	r.y = sclamp(y, min.y, max.y);
	r.z = sclamp(z, min.z, max.z);
	r.w = sclamp(w, min.w, max.w);
	return r;
}

template<class T>
inline vec4<T> &vec4<T>::saturate()
{
	x = ssaturate(x);
	y = ssaturate(y);
	z = ssaturate(z);
	w = ssaturate(w);
	return *this;
}

template<class T>
inline vec4<T> vec4<T>::getSaturated() const
{
	vec4<T> r;
	r.x = ssaturate(x);
	r.y = ssaturate(y);
	r.z = ssaturate(z);
	r.w = ssaturate(w);
	return r;
}

template<class T>
inline vec4<T> &vec4<T>::normalize()
{
	T t = T(1) / length();
	x *= t;
	y *= t;
	z *= t;
	w *= t;
	return *this;
}

template<class T>
inline vec4<T> vec4<T>::getNormalized() const
{
	vec4<T> r;
	T t = T(1) / length();
	r.x = x * t;
	r.y = y * t;
	r.z = z * t;
	r.w = w * t;
	return r;
}

template<class T>
inline T vec4<T>::len() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

template<class T>
inline T vec4<T>::length() const
{
	return len();
}

template<class T>
inline T vec4<T>::len2() const
{
	return x * x + y * y + z * z + w * w;
}

template<class T>
inline T vec4<T>::length2() const
{
	return len2();
}

template<class T>
inline T vec4<T>::lengthSquared() const
{
	return len2();
}


template<class T>
inline vec4<T> vec4<T>::reflection(const vec4<T> &v) const
{
	T nv2 = dot(v) * ((T) 2);
	return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w) * nv2;
}

template<class T>
inline vec4<T> vec4<T>::refraction(const vec4<T> &v, T r) const
{
	T w, k, wk;
	w = -dot(v) * r;
	k = sqrt(T(1) + (w - r) * (w + r));
	wk = w - k;
	return vec4<T>(r * x + wk * v.x,
				   r * y + wk * v.y,
				   r * z + wk * v.z,
				   r * w + wk * v.w);
}

template<class T>
f32 vec4<T>::projectionLengthOn(const vec4<T> &v) const
{
	return dot(v) / v.len();
}

template<class T>
vec4<T> vec4<T>::projectOn(const vec4<T> &v) const
{
	return v * (dot(v) / (v.dot(v)));
}

template<class T>
inline vec3<T> vec4<T>::xyz() const
{
	return vec3<T>(x, y, z);
}

template<class T>
inline vec2<T> vec4<T>::xy() const
{
	return vec2<T>(x, y);
}

template<class T>
inline vec4<T> operator+(T lv, vec4<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec4<T> operator-(T lv, vec4<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec4<T> operator*(T lv, vec4<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec4<T> operator/(T lv, vec4<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	d.z = lv / rv.z;
	d.w = lv / rv.w;
	return d;
}

template<class T>
inline vec4<T> operator+(T lv, const vec4<T> &rv)
{
	return rv + lv;
}

template<class T>
inline vec4<T> operator-(T lv, const vec4<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline vec4<T> operator*(T lv, const vec4<T> &rv)
{
	return rv * lv;
}

template<class T>
inline vec4<T> operator/(T lv, const vec4<T> &rv)
{
	vec4<T> d;
	d.x = lv / rv.x;
	d.y = lv / rv.y;
	d.z = lv / rv.z;
	d.w = lv / rv.w;
	return d;
}








/* Functions */

/* Assignment */
template<class T>
inline mat2<T> &mat2<T>::operator=(const mat2<T> &v)
{
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;
	return *this;
}


/* Comparison */
template<class T>
inline bool mat2<T>::operator==(const mat2<T> &v) const
{
	return a == v.a &&
		b == v.b &&
		c == v.c &&
		d == v.d;
}

template<class T>
inline bool mat2<T>::operator!=(const mat2<T> &v) const
{
	return a != v.a ||
		b != v.b ||
		c != v.c ||
		d != v.d;
}

template<class T>
inline bool mat2<T>::isIdentity() const
{
	return a == 1 &&
		b == 0 &&
		c == 0 &&
		d == 1;
}


/* Addition */
template<class T>
inline mat2<T> mat2<T>::operator+(const mat2<T> &v) const
{
	return mat2<T>(
		a + v.a,
		b + v.b,
		c + v.c,
		d + v.d);
}

template<class T>
inline mat2<T> &mat2<T>::operator+=(const mat2<T> &v)
{
	a += v.a;
	b += v.b;
	c += v.c;
	d += v.d;
	return *this;
}

template<class T>
inline mat2<T> mat2<T>::operator+(T v) const
{
	return mat2<T>(
		a + v,
		b + v,
		c + v,
		d + v);
}

template<class T>
inline mat2<T> &mat2<T>::operator+=(T v)
{
	a += v;
	b += v;
	c += v;
	d += v;
	return *this;
}

template<class T>
inline mat2<T> mat2<T>::operator+() const
{
	return mat2<T>(
		a,
		b,
		c,
		d);
}


/* Subtraction */
template<class T>
inline mat2<T> mat2<T>::operator-(const mat2<T> &v) const
{
	return mat2<T>(
		a - v.a,
		b - v.b,
		c - v.c,
		d - v.d);
}

template<class T>
inline mat2<T> &mat2<T>::operator-=(const mat2<T> &v)
{
	a -= v.a;
	b -= v.b;
	c -= v.c;
	d -= v.d;
	return *this;
}

template<class T>
inline mat2<T> mat2<T>::operator-(T v) const
{
	return mat2<T>(
		a - v,
		b - v,
		c - v,
		d - v);
}

template<class T>
inline mat2<T> &mat2<T>::operator-=(T v)
{
	a -= v;
	b -= v;
	c -= v;
	d -= v;
	return this;
}

template<class T>
inline mat2<T> mat2<T>::operator-() const
{
	return mat2<T>(
		-a,
		-b,
		-c,
		-d);
}


/* Inner product */
template<class T>
inline T mat2<T>::iprod(const mat2<T> &v, const u32 row, const u32 col) const
{
	return mat[row][0] * v.mat[0][col] +
		mat[row][1] * v.mat[1][col];
}


/* Multiplication */
template<class T>
inline mat2<T> mat2<T>::operator*(const mat2<T>	&v) const
{
	return mat2<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 1, 0),
			iprod(v, 1, 1));
}

template<class T>
inline mat2<T> &mat2<T>::operator*=(const mat2<T> &v)
{
	mat2<T> mat = mat2<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 1, 0),
			iprod(v, 1, 1));
	a = mat.a;
	b = mat.b;
	c = mat.c;
	d = mat.d;
	return *this;
}

template<class T>
inline mat2<T> mat2<T>::operator*(T v) const
{
	return mat2<T>(
		a * v,
		b * v,
		c * v,
		d * v);
}

template<class T>
inline mat2<T> &mat2<T>::operator*=(T v)
{
	a *= v;
	b *= v;
	c *= v;
	d *= v;
	return *this;
}

template<class T>
inline vec2<T> mat2<T>::operator*(const vec2<T> &v) const
{
	return vec2<T>(
		a * v.x + b * v.y,
		c * v.x + d * v.y
	);
}


/* Element-wise multiplication */
template<class T>
inline mat2<T> mat2<T>::elemMul(const mat2<T> &v) const
{
	return mat2<T>(
		a * v.a,
		b * v.b,
		c * v.c,
		d * v.d);
}


/* Division with scalar*/
template<class T>
inline mat2<T> mat2<T>::operator/(T v) const
{
	T f = T(1) / v;
	return mat2<T>(
		a * f,
		b * f,
		c * f,
		d * f);
}

template<class T>
inline mat2<T> &mat2<T>::operator/=(T v)
{
	T f = T(1) / v;
	a *= f;
	b *= f;
	c *= f;
	d *= f;
	return *this;
}


/* Misc matrix operations */
template<class T>
inline mat2<T> &mat2<T>::identity()
{
	a = 1;
	b = 0;
	c = 0;
	d = 1;
	return *this;
}

template<class T>
inline mat2<T> &mat2<T>::transpose()
{
	sswap(&b, &c);
	return *this;
}

template<class T>
inline T mat2<T>::det() const
{
	return a * d - b * c;
}

template<class T>
inline T mat2<T>::determinant() const
{
	return det();
}

template<class T>
inline mat2<T> &mat2<T>::inv()
{
	mat2<T> mat = *this;
	T deter = det();
	if (deter == 0) {
		a = (T) 1;
		b = (T) 0;
		c = (T) 0;
		d = (T) 1;
		return *this;
	}
	T denom = T(1) / deter();
	a = mat.d * denom;
	b = -mat.b * denom;
	c = -mat.c * denom;
	d = mat.a * denom;
	return *this;
}

template<class T>
inline mat2<T> &mat2<T>::invert()
{
	return inv();
}

template<class T>
inline mat2<T> mat2<T>::getInv() const
{
	return getInverse();
}

template<class T>
inline mat2<T> mat2<T>::getInverse() const
{
	mat2<T> mat = *this;
	mat.inv();
	return mat;
}

template<class T>
inline T mat2<T>::trace() const
{
	return a + d;
}

template<class T>
inline vec2<T> mat2<T>::eigenvalues() const
{
	vec2<T> ret;
	T t = trace();
	T u = sqrt(t * t - ((T) 4) * det());
	ret.x = t - u;
	ret.y = t + u;
	return ret;
}

template<class T>
inline mat2<T> mat2<T>::eigenvectors(const vec2<T> &eig) const
{
	if (b != 0)
		return mat2<T>(b, b, eig.x - a, eig.y - a);
	if (c != 0)
		return mat2<T>(eig.x - d, eig.y - d, c, c);
	return mat2<T>(1, 0, 0, 1);
}

template<class T>
inline vec2<T> mat2<T>::getRow(u32 row) const
{
	return vec2<T>(
		mat[row][0],
		mat[row][1]);
}

template<class T>
inline void mat2<T>::setRow(u32 row, const vec2<T> &vec)
{
	mat[row][0] = vec.x;
	mat[row][1] = vec.y;
}

template<class T>
inline vec2<T> mat2<T>::getCol(u32 col) const
{
	return vec2<T>(
		mat[0][col],
		mat[1][col]);
}

template<class T>
inline void mat2<T>::setCol(u32 col, const vec2<T> &vec)
{
	mat[0][col] = vec.x;
	mat[1][col] = vec.y;
}

template<class T>
vec2<T> mat2<T>::solveSystem(const vec2<T> &eq) const
{
	T invd = T(1) / (a * d - b * c);
	T sx = (eq.x * d - b * eq.y) * invd;
	T sy = (a * eq.x - eq.y * c) * invd;
	return vec2<T>(sx, sy);
}

template<class T>
inline mat2<T> operator+(T lv, mat2<T> &rv)
{
	return rv + lv;
}

template<class T>
inline mat2<T> operator-(T lv, mat2<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline mat2<T> operator*(T lv, mat2<T> &rv)
{
	return rv * lv;
}







template<class T>
inline mat3<T> mat3<T>::getRotationMatrix(T rad, const vec3<T> &axis)
{
	mat3<T> mat;
	vec3<T> v = axis;
	if (abs(1.0f - v.len2()) > 0.0001f) /* TODO: Hard coded tolerance */
		v.normalize();

	T cs = cos(rad);
	T sn = sin(rad);

	T xx = v.x * v.x;
	T xy = v.x * v.y;
	T xz = v.x * v.z;
	T yy = v.y * v.y;
	T yz = v.y * v.z;
	T zz = v.z * v.z;

	mat.a = xx + (1.0f - xx) * cs;
	mat.b = xy * (1.0f - cs) - v.z * sn;
	mat.c = xz * (1.0f - cs) + v.y * sn;

	mat.d = xy * (1.0f - cs) + v.z * sn;
	mat.e = yy + (1.0f - yy) * cs;
	mat.f = yz * (1.0f - cs) - v.x * sn;

	mat.g = xz * (1.0f - cs) - v.y * sn;
	mat.h = yz * (1.0f - cs) + v.x * sn;
	mat.i = zz + (1.0f - zz) * cs;
}

template<class T>
inline mat3<T> mat3<T>::getRotationXMatrix(T rad)
{
	T cs = cos(rad);
	T sn = sin(rad);
	mat3<T> mat;
	mat.a = 1;		mat.b = 0;		mat.c = 0;
	mat.d = 0;		mat.e = cs;		mat.f = -sn;
	mat.g = 0;		mat.h = sn;		mat.i = cs;
	return mat;
}

template<class T>
inline mat3<T> mat3<T>::getRotationYMatrix(T rad)
{
	T cs = cos(rad);
	T sn = sin(rad);
	mat3<T> mat;
	mat.a = cs;		mat.b = 0;		mat.c = sn;
	mat.d = 0;		mat.e = 1;		mat.f = 0;
	mat.g = -sn;	mat.h = 0;		mat.i = cs;
	return mat;
}

template<class T>
inline mat3<T> mat3<T>::getRotationZMatrix(T rad)
{
	T cs = cos(rad);
	T sn = sin(rad);
	mat3<T> mat;
	mat.a = cs;		mat.b = -sn;	mat.c = 0;
	mat.d = sn;		mat.e = cs;		mat.f = 0;
	mat.g = 0;		mat.h = 0;		mat.i = 1;
	return mat;
}

/* Functions */

/* Assignment */
template<class T>
inline mat3<T> &mat3<T>::operator=(const mat3<T> &v)
{
	a = v.a;
	b = v.b;
	c = v.c;

	d = v.d;
	e = v.e;
	f = v.f;

	g = v.g;
	h = v.h;
	i = v.i;

	return *this;
}


/* Comparison */
template<class T>
inline bool mat3<T>::operator==(const mat3<T> &v) const
{
	return a == v.a &&
		b == v.b &&
		c == v.c &&

		d == v.d &&
		e == v.e &&
		f == v.f &&

		g == v.g &&
		h == v.h &&
		i == v.i;
}

template<class T>
inline bool mat3<T>::operator!=(const mat3<T> &v) const
{
	return a != v.a ||
		b != v.b ||
		c != v.c ||

		d != v.d ||
		e != v.e ||
		f != v.f ||

		g != v.g ||
		h != v.h ||
		i != v.i;
}

template<class T>
inline bool mat3<T>::isIdentity() const
{
	return a == 1 &&
		b == 0 &&
		c == 0 &&

		d == 0 &&
		e == 1 &&
		f == 0 &&

		g == 0 &&
		h == 0 &&
		i == 1;
}


/* Addition */
template<class T>
inline mat3<T> mat3<T>::operator+(const mat3<T> &v) const
{
	return mat3<T>(
		a + v.a,
		b + v.b,
		c + v.c,

		d + v.d,
		e + v.e,
		f + v.f,

		g + v.g,
		h + v.h,
		i + v.i);
}

template<class T>
inline mat3<T> &mat3<T>::operator+=(const mat3<T> &v)
{
	a += v.a;
	b += v.b;
	c += v.c;

	d += v.d;
	e += v.e;
	f += v.f;

	g += v.g;
	h += v.h;
	i += v.i;

	return *this;
}

template<class T>
inline mat3<T> mat3<T>::operator+(T v) const
{
	return mat3<T>(
		a + v,
		b + v,
		c + v,

		d + v,
		e + v,
		f + v,

		g + v,
		h + v,
		i + v);
}

template<class T>
inline mat3<T> &mat3<T>::operator+=(T v)
{
	a += v;
	b += v;
	c += v;

	d += v;
	e += v;
	f += v;

	g += v;
	h += v;
	i += v;

	return *this;
}

template<class T>
inline mat3<T> mat3<T>::operator+() const
{
	return mat3<T>(
		a, b, c,
		d, e, f,
		g, h, i);
}


/* Subtraction */
template<class T>
inline mat3<T> mat3<T>::operator-(const mat3<T> &v) const
{
	return mat3<T>(
		a - v.a,
		b - v.b,
		c - v.c,

		d - v.d,
		e - v.e,
		f - v.f,

		g - v.g,
		h - v.h,
		i - v.i);
}

template<class T>
inline mat3<T> &mat3<T>::operator-=(const mat3<T> &v)
{
	a -= v.a;
	b -= v.b;
	c -= v.c;

	d -= v.d;
	e -= v.e;
	f -= v.f;

	g -= v.g;
	h -= v.h;
	i -= v.i;

	return *this;
}

template<class T>
inline mat3<T> mat3<T>::operator-(T v) const
{
	return mat3<T>(
		a - v,
		b - v,
		c - v,

		d - v,
		e - v,
		f - v,

		g - v,
		h - v,
		i - v);
}

template<class T>
inline mat3<T> &mat3<T>::operator-=(T v)
{
	a -= v;
	b -= v;
	c -= v;

	d -= v;
	e -= v;
	f -= v;

	g -= v;
	h -= v;
	i -= v;

	return *this;
}

template<class T>
inline mat3<T> mat3<T>::operator-() const
{
	return mat3<T>(
		-a, -b, -c,
		-d, -e, -f,
		-g, -h, -i);
}


/* Inner product */
template<class T>
inline T mat3<T>::iprod(const mat3<T> &v, const u32 row, const u32 col) const
{
	return mat[row][0] * v.mat[0][col] +
		mat[row][1] * v.mat[1][col] +
		mat[row][2] * v.mat[2][col];
}


/* Multiplication */
template<class T>
inline mat3<T> mat3<T>::operator*(const mat3<T> &v) const
{
	return mat3<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 0, 2),

			iprod(v, 1, 0),
			iprod(v, 1, 1),
			iprod(v, 1, 2),

			iprod(v, 2, 0),
			iprod(v, 2, 1),
			iprod(v, 2, 2));
}

template<class T>
inline mat3<T> &mat3<T>::operator*=(const mat3<T> &v)
{
	mat3<T> mat = mat3<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 0, 2),

			iprod(v, 1, 0),
			iprod(v, 1, 1),
			iprod(v, 1, 2),

			iprod(v, 2, 0),
			iprod(v, 2, 1),
			iprod(v, 2, 2));

	a = mat.a;
	b = mat.b;
	c = mat.c;

	d = mat.d;
	e = mat.e;
	f = mat.f;

	g = mat.g;
	h = mat.h;
	i = mat.i;

	return *this;
}

template<class T>
inline mat3<T> mat3<T>::operator*(T v) const
{
	return mat3<T>(
		a * v,
		b * v,
		c * v,

		d * v,
		e * v,
		f * v,

		g * v,
		h * v,
		i * v);
}

template<class T>
inline mat3<T> &mat3<T>::operator*=(T v)
{
	a *= v;
	b *= v;
	c *= v;

	d *= v;
	e *= v;
	f *= v;

	g *= v;
	h *= v;
	i *= v;

	return *this;
}

template<class T>
inline vec3<T> mat3<T>::operator*(const vec3<T> &v) const
{
	return vec3<T>(
		a * v.x + b * v.y + c * v.z,
		d * v.x + e * v.y + f * v.z,
		g * v.x + h * v.y + i * v.z
	);
}


/* Element-wise multiplication */
template<class T>
inline mat3<T> mat3<T>::elemMul(const mat3<T> &v) const
{
	return mat3<T>(
		a * v.a,
		b * v.b,
		c * v.c,

		d * v.d,
		e * v.e,
		f * v.f,

		g * v.g,
		h * v.h,
		i * v.i);
}



/* Division with scalar*/
template<class T>
inline mat3<T> mat3<T>::operator/(T v) const
{
	T f = T(1) / v;
	return mat3<T>(
		a * f,
		b * f,
		c * f,

		d * f,
		e * f,
		f * f,

		g * f,
		h * f,
		i * f);
}

template<class T>
inline mat3<T> &mat3<T>::operator/=(T v)
{
	T f = T(1) / v;
	a *= f;
	b *= f;
	c *= f;

	d *= f;
	e *= f;
	f *= f;

	g *= f;
	h *= f;
	i *= f;

	return *this;
}


/* Misc matrix operations */
template<class T>
inline mat3<T> &mat3<T>::identity()
{
	a = e = i = 1;
	b = c = 0;
	d = f = 0;
	g = h = 0;
	return *this;
}

template<class T>
inline mat3<T> &mat3<T>::transpose()
{
	sswap(&b, &d);
	sswap(&c, &g);
	sswap(&f, &h);
	return *this;
}

template<class T>
inline mat3<T> mat3<T>::getTranspose() const
{
	mat3<T> m = mat3<T>(
		a, d, g,
		b, e, h,
		c, f, i);
	return m;
}

template<class T>
inline T mat3<T>::det() const
{
	return a * (i * e - h * f) -
		d * (i * b - h * c) +
		g * (f * b - e * c);
}

template<class T>
inline T mat3<T>::determinant() const
{
	return det();
}

template<class T>
inline mat3<T> &mat3<T>::inv()
{
	T ie = i * e;
	T ib = i * b;
	T fb = f * b;
	T hf = h * f;
	T hc = h * c;
	T ec = e * c;

	T deter = a * (ie - hf) - d * (ib - hc) + g * (fb - ec);

	if (deter == ((T) 0)) {
		identity();
		return *this;
	}

	mat3<T> mat = *this;
	T denom = T(1) / deter;

	a = (ie            - hf           ) * denom;
	b = (hc            - ib           ) * denom;
	c = (fb            - ec           ) * denom;

	d = (mat.f * mat.g - mat.d * mat.i) * denom;
	e = (mat.a * mat.i - mat.c * mat.g) * denom;
	f = (mat.d * mat.c - mat.a * mat.f) * denom;

	g = (mat.d * mat.h - mat.e * mat.g) * denom;
	h = (mat.b * mat.g - mat.a * mat.h) * denom;
	i = (mat.a * mat.e - mat.b * mat.d) * denom;
	return *this;
}

template<class T>
inline mat3<T> &mat3<T>::invert()
{
	return inv();
}

template<class T>
inline mat3<T> mat3<T>::getInv() const
{
	return getInverse();
}

template<class T>
inline mat3<T> mat3<T>::getInverse() const
{
	mat3<T> mat = *this;
	mat.inv();
	return mat;
}

template<class T>
inline T mat3<T>::trace() const
{
	return a + e + i;
}

template<class T>
inline vec3<T> mat3<T>::getRow(u32 row) const
{
	return vec3<T>(
		mat[row][0],
		mat[row][1],
		mat[row][2]);
}

template<class T>
inline void mat3<T>::setRow(u32 row, const vec3<T> &vec)
{
	mat[row][0] = vec.x;
	mat[row][1] = vec.y;
	mat[row][2] = vec.z;
}

template<class T>
inline vec3<T> mat3<T>::getCol(u32 col) const
{
	return vec3<T>(
		mat[0][col],
		mat[1][col],
		mat[2][col]);
}

template<class T>
inline void mat3<T>::setCol(u32 col, const vec3<T> &vec)
{
	mat[0][col] = vec.x;
	mat[1][col] = vec.y;
	mat[2][col] = vec.z;
}

template<class T>
mat3<T> &mat3<T>::rotateX(T rad)
{
	T nd, ne, nf;
	T cs, sn;

	cs = cos(rad);
	sn = sin(rad);

	nd = d * cs - g * sn;
	ne = e * cs - h * sn;
	nf = f * cs - i * sn;

	g = d * sn + g * cs;
	h = e * sn + h * cs;
	i = f * sn + i * cs;

	d = nd;
	e = ne;
	f = nf;

	return *this;
}

template<class T>
mat3<T> &mat3<T>::rotateY(T rad)
{
	T na, nb, nc;
	T cs, sn;

	cs = cos(rad);
	sn = sin(rad);

	na = a * cs + g * sn;
	nb = b * cs + h * sn;
	nc = c * cs + i * sn;

	g = g * cs - a * sn;
	h = h * cs - b * sn;
	i = i * cs - c * sn;

	a = na;
	b = nb;
	c = nc;

	return *this;
}

template<class T>
mat3<T> &mat3<T>::preRotateY(T rad)
{
	T na, nd, ng;
	T cs, sn;

	cs = cos(rad);
	sn = sin(rad);

	na = a * cs - c * sn;
	nd = d * cs - f * sn;
	ng = g * cs - i * sn;

	c = a * sn + c * cs;
	f = d * sn + f * cs;
	i = g * sn + i * cs;

	a = na;
	d = nd;
	g = ng;

	return *this;
}

template<class T>
mat3<T> &mat3<T>::rotateZ(T rad)
{
	T na, nb, nc;
	T cs, sn;

	cs = cos(rad);
	sn = sin(rad);

	na = a * cs - d * sn;
	nb = b * cs - e * sn;
	nc = c * cs - f * sn;

	d = a * sn + d * cs;
	e = b * sn + e * cs;
	f = c * sn + f * cs;

	a = na;
	b = nb;
	c = nc;

	return *this;
}

template<class T>
mat3<T> &mat3<T>::rotate(T rad, const vec3<T> axis)
{
	mat3<T> r = getRotationMatrix(rad, axis);
	return *this;
}

template<class T>
mat3<T> &mat3<T>::scale(T sc)
{
	this *= sc;
	return *this;
}

template<class T>
mat3<T> &mat3<T>::scale(T sx, T sy, T sz)
{
	a *= sx; b *= sy; c *= sz;
	d *= sx; e *= sy; f *= sz;
	g *= sx; h *= sy; i *= sz;
	return *this;
}

template<class T>
vec3<T> mat3<T>::up() const
{
	return vec3<T>(b, e, h);
}

template<class T>
vec3<T> mat3<T>::down() const
{
	return -up();
}

template<class T>
vec3<T> mat3<T>::left() const
{
	return -right();
}

template<class T>
vec3<T> mat3<T>::right() const
{
	return vec3<T>(a, d, g);
}

template<class T>
vec3<T> mat3<T>::front() const
{
	return -back();
	//return vec3<T>(c, f, i);
}

template<class T>
vec3<T> mat3<T>::back() const
{
	return vec3<T>(c, f, i);
	//return -front(); // TODO: As define
}


template<class T>
inline mat3<T> operator+(T lv, mat3<T> &rv)
{
	return rv + lv;
}

template<class T>
inline mat3<T> operator-(T lv, mat3<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline mat3<T> operator*(T lv, mat3<T> &rv)
{
	return rv * lv;
}















/* Functions */

/* Assignment */
template<class T>
inline mat4<T> &mat4<T>::operator=(const mat4<T> &v)
{
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;

	e = v.e;
	f = v.f;
	g = v.g;
	h = v.h;

	i = v.i;
	j = v.j;
	k = v.k;
	l = v.l;

	m = v.m;
	n = v.n;
	o = v.o;
	p = v.p;

	return *this;
}


/* Comparison */
template<class T>
inline bool mat4<T>::operator==(const mat4<T> &v) const
{
	return a == v.a &&
		b == v.b &&
		c == v.c &&
		d == v.d &&

		e == v.e &&
		f == v.f &&
		g == v.g &&
		h == v.h &&

		i == v.i &&
		j == v.j &&
		k == v.k &&
		l == v.l &&

		m == v.m &&
		n == v.n &&
		o == v.o &&
		p == v.p;
}

template<class T>
inline bool mat4<T>::operator!=(const mat4<T> &v) const
{
	return a != v.a ||
		b != v.b ||
		c != v.c ||
		d != v.d ||

		e != v.e ||
		f != v.f ||
		g != v.g ||
		h != v.h ||

		i != v.i ||
		j != v.j ||
		k != v.k ||
		l != v.l ||

		m != v.m ||
		n != v.n ||
		o != v.o ||
		p != v.p;
}

template<class T>
inline bool mat4<T>::isIdentity() const
{
	return a == 1 &&
		b == 0 &&
		c == 0 &&
		d == 0 &&

		e == 0 &&
		f == 1 &&
		g == 0 &&
		h == 0 &&

		i == 0 &&
		j == 0 &&
		k == 1 &&
		l == 0 &&

		m == 0 &&
		n == 0 &&
		o == 0 &&
		p == 1;
}


/* Addition */
template<class T>
inline mat4<T> mat4<T>::operator+(const mat4<T> &v) const
{
	return mat4<T>(
		a + v.a,
		b + v.b,
		c + v.c,
		d + v.d,

		e + v.e,
		f + v.f,
		g + v.g,
		h + v.h,

		i + v.i,
		j + v.j,
		k + v.k,
		l + v.l,

		m + v.m,
		n + v.n,
		o + v.o,
		p + v.p);
}

template<class T>
inline mat4<T> &mat4<T>::operator+=(const mat4<T> &v)
{
	a += v.a;
	b += v.b;
	c += v.c;
	d += v.d;

	e += v.e;
	f += v.f;
	g += v.g;
	h += v.h;

	i += v.i;
	j += v.j;
	k += v.k;
	l += v.l;

	m += v.m;
	n += v.n;
	o += v.o;
	p += v.p;

	return *this;
}

template<class T>
inline mat4<T> mat4<T>::operator+(T v) const
{
	return mat4<T>(
		a + v,
		b + v,
		c + v,
		d + v,

		e + v,
		f + v,
		g + v,
		h + v,

		i + v,
		j + v,
		k + v,
		l + v,

		m + v,
		n + v,
		o + v,
		p + v);
}

template<class T>
inline mat4<T> &mat4<T>::operator+=(T v)
{
	a += v;
	b += v;
	c += v;
	d += v;

	e += v;
	f += v;
	g += v;
	h += v;

	i += v;
	j += v;
	k += v;
	l += v;

	m += v;
	n += v;
	o += v;
	p += v;

	return *this;
}

template<class T>
inline mat4<T> mat4<T>::operator+() const
{
	return mat4<T>(
		a, b, c, d,
		e, f, g, h,
		i, j, k, l,
		m, n, o, p);
}


/* Subtraction */
template<class T>
inline mat4<T> mat4<T>::operator-(const mat4<T> &v) const
{
	return mat4<T>(
		a - v.a,
		b - v.b,
		c - v.c,
		d - v.d,

		e - v.e,
		f - v.f,
		g - v.g,
		h - v.h,

		i - v.i,
		j - v.j,
		k - v.k,
		l - v.l,

		m - v.m,
		n - v.n,
		o - v.o,
		p - v.p);
}

template<class T>
inline mat4<T> &mat4<T>::operator-=(const mat4<T> &v)
{
	a -= v.a;
	b -= v.b;
	c -= v.c;
	d -= v.d;

	e -= v.e;
	f -= v.f;
	g -= v.g;
	h -= v.h;

	i -= v.i;
	j -= v.j;
	k -= v.k;
	l -= v.l;

	m -= v.m;
	n -= v.n;
	o -= v.o;
	p -= v.p;

	return *this;
}

template<class T>
inline mat4<T> mat4<T>::operator-(T v) const
{
	return mat4<T>(
		a - v,
		b - v,
		c - v,
		d - v,

		e - v,
		f - v,
		g - v,
		h - v,

		i - v,
		j - v,
		k - v,
		l - v,

		m - v,
		n - v,
		o - v,
		p - v);
}

template<class T>
inline mat4<T> &mat4<T>::operator-=(T v)
{
	a -= v;
	b -= v;
	c -= v;
	d -= v;

	e -= v;
	f -= v;
	g -= v;
	h -= v;

	i -= v;
	j -= v;
	k -= v;
	l -= v;

	m -= v;
	n -= v;
	o -= v;
	p -= v;

	return *this;
}

template<class T>
inline mat4<T> mat4<T>::operator-() const
{
	return mat4<T>(
		-a, -b, -c, -d,
		-e, -f, -g, -h,
		-i, -j, -k, -l,
		-m, -n, -o, -p);
}


/* Inner product */
template<class T>
inline T mat4<T>::iprod(const mat4<T> &v, const u32 row, const u32 col) const
{
	return mat[row][0] * v.mat[0][col] +
		mat[row][1] * v.mat[1][col] +
		mat[row][2] * v.mat[2][col] +
		mat[row][3] * v.mat[3][col];
}


/* Multiplication */
template<class T>
inline mat4<T> mat4<T>::operator*(const mat4<T> &v) const
{
	return mat4<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 0, 2),
			iprod(v, 0, 3),

			iprod(v, 1, 0),
			iprod(v, 1, 1),
			iprod(v, 1, 2),
			iprod(v, 1, 3),

			iprod(v, 2, 0),
			iprod(v, 2, 1),
			iprod(v, 2, 2),
			iprod(v, 2, 3),

			iprod(v, 3, 0),
			iprod(v, 3, 1),
			iprod(v, 3, 2),
			iprod(v, 3, 3));

}

template<class T>
inline mat4<T> &mat4<T>::operator*=(const mat4<T> &v)
{
	mat4<T> mat = mat4<T>(
			iprod(v, 0, 0),
			iprod(v, 0, 1),
			iprod(v, 0, 2),
			iprod(v, 0, 3),

			iprod(v, 1, 0),
			iprod(v, 1, 1),
			iprod(v, 1, 2),
			iprod(v, 1, 3),

			iprod(v, 2, 0),
			iprod(v, 2, 1),
			iprod(v, 2, 2),
			iprod(v, 2, 3),

			iprod(v, 3, 0),
			iprod(v, 3, 1),
			iprod(v, 3, 2),
			iprod(v, 3, 3));

	a = mat.a;
	b = mat.b;
	c = mat.c;
	d = mat.d;

	e = mat.e;
	f = mat.f;
	g = mat.g;
	h = mat.h;

	i = mat.i;
	j = mat.j;
	k = mat.k;
	l = mat.l;

	m = mat.m;
	n = mat.n;
	o = mat.o;
	p = mat.p;

	return *this;
}



template<class T>
inline T mat4<T>::affineiprod0(const mat4<T> &v, const u32 row, const u32 col) const
{
	return mat[row][0] * v.mat[0][col] +
		mat[row][1] * v.mat[1][col] +
		mat[row][2] * v.mat[2][col];
}

template<class T>
inline T mat4<T>::affineiprod1(const mat4<T> &v, const u32 row, const u32 col) const
{
	return mat[row][0] * v.mat[0][col] +
		mat[row][1] * v.mat[1][col] +
		mat[row][2] * v.mat[2][col] +
		mat[row][3];
}

template<class T>
inline mat4<T> mat4<T>::affineMul(const mat4<T> &v) const
{
	return mat4<T>(
			affineiprod0(v, 0, 0),
			affineiprod0(v, 0, 1),
			affineiprod0(v, 0, 2),
			affineiprod1(v, 0, 3),

			affineiprod0(v, 1, 0),
			affineiprod0(v, 1, 1),
			affineiprod0(v, 1, 2),
			affineiprod1(v, 1, 3),

			affineiprod0(v, 2, 0),
			affineiprod0(v, 2, 1),
			affineiprod0(v, 2, 2),
			affineiprod1(v, 2, 3),

			((T) 0),
			((T) 0),
			((T) 0),
			T(1));
}

template<class T>
inline vec4<T> mat4<T>::affineMul(const vec4<T> &v) const
{
	return vec4<T>(
		a * v.x + b * v.y + c * v.z + d * v.w,
		e * v.x + f * v.y + g * v.z + h * v.w,
		i * v.x + j * v.y + k * v.z + l * v.w,
		v.w
	);
}

template<class T>
inline vec3<T> mat4<T>::affineMul(const vec3<T> &v) const
{
	return vec3<T>(
		a * v.x + b * v.y + c * v.z + d,
		e * v.x + f * v.y + g * v.z + h,
		i * v.x + j * v.y + k * v.z + l
	);
}

template<class T>
inline mat4<T> mat4<T>::operator*(T v) const
{
	return mat4<T>(
		a * v,
		b * v,
		c * v,
		d * v,

		e * v,
		f * v,
		g * v,
		h * v,

		i * v,
		j * v,
		k * v,
		l * v,

		m * v,
		n * v,
		o * v,
		p * v);
}

template<class T>
inline mat4<T> &mat4<T>::operator*=(T v)
{
	a *= v;
	b *= v;
	c *= v;
	d *= v;

	e *= v;
	f *= v;
	g *= v;
	h *= v;

	i *= v;
	j *= v;
	k *= v;
	l *= v;

	m *= v;
	n *= v;
	o *= v;
	p *= v;

	return *this;
}

template<class T>
inline vec4<T> mat4<T>::operator*(const vec4<T>	&v) const
{
	return vec4<T>(
		a * v.x + b * v.y + c * v.z + d * v.w,
		e * v.x + f * v.y + g * v.z + h * v.w,
		i * v.x + j * v.y + k * v.z + l * v.w,
		m * v.x + n * v.y + o * v.z + p * v.w
	);
}


/* Element-wise multiplication */
template<class T>
inline mat4<T> mat4<T>::elemMul(const mat4<T> &v) const
{
	return mat4<T>(
		a * v.a,
		b * v.b,
		c * v.c,
		d * v.d,

		e * v.e,
		f * v.f,
		g * v.g,
		h * v.h,

		i * v.i,
		j * v.j,
		k * v.k,
		l * v.l,

		m * v.m,
		n * v.n,
		o * v.o,
		p * v.p);
}


/* Division with scalar*/
template<class T>
inline mat4<T> mat4<T>::operator/(T v) const
{
	T t = T(1) / v;
	return mat4<T>(
		a * t,
		b * t,
		c * t,
		d * t,

		e * t,
		f * t,
		g * t,
		h * t,

		i * t,
		j * t,
		k * t,
		l * t,

		m * t,
		n * t,
		o * t,
		p * t);
}

template<class T>
inline mat4<T> &mat4<T>::operator/=(T v)
{
	T t = T(1) / v;
	a *= t;
	b *= t;
	c *= t;
	d *= t;

	e *= t;
	f *= t;
	g *= t;
	h *= t;

	i *= t;
	j *= t;
	k *= t;
	l *= t;

	m *= t;
	n *= t;
	o *= t;
	p *= t;

	return *this;
}


/* Misc matrix operations */
template<class T>
inline mat4<T> &mat4<T>::identity()
{
	a = f = k = p = 1;
	b = c = d = 0;
	e = g = h = 0;
	i = j = l = 0;
	m = n = o = 0;
	return *this;
}

template<class T>
inline mat4<T> &mat4<T>::transpose()
{
	sswap(&e, &b);
	sswap(&i, &c);
	sswap(&m, &d);
	sswap(&j, &g);
	sswap(&n, &h);
	sswap(&o, &l);
	return *this;
}

template<class T>
inline mat4<T> mat4<T>::getTranspose() const
{
	mat4<T> m = mat4<T>(
		a, e, i, m,
		b, f, j, n,
		c, g, k, o,
		d, h, l, p);
	return m;
}

template<class T>
inline T mat4<T>::det2(
		T n0,
		T n1,
		T n2,
		T n3
	) const
{
	return n0 * n3 - n1 * n2;
}

template<class T>
inline T mat4<T>::det() const
{
	T s0, s1, s2, s3, s4, s5;
	T c0, c1, c2, c3, c4, c5;

	s0 = det2(a, b, e, f);
	s1 = det2(a, c, e, g);
	s2 = det2(a, d, e, h);
	s3 = det2(b, c, f, g);
	s4 = det2(b, d, f, h);
	s5 = det2(c, d, g, h);

	c5 = det2(k, l, o, p);
	c4 = det2(j, l, n, p);
	c3 = det2(j, k, n, o);
	c2 = det2(i, l, m, p);
	c1 = det2(i, k, m, o);
	c0 = det2(i, j, m, n);

	return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

template<class T>
inline T mat4<T>::determinant() const
{
	return det();
}

template<class T>
inline mat4<T> &mat4<T>::inv()
{
	T s0, s1, s2, s3, s4, s5;
	T c0, c1, c2, c3, c4, c5;
	T deter, denom;

	s0 = det2(a, b, e, f);
	s1 = det2(a, c, e, g);
	s2 = det2(a, d, e, h);
	s3 = det2(b, c, f, g);
	s4 = det2(b, d, f, h);
	s5 = det2(c, d, g, h);

	c5 = det2(k, l, o, p);
	c4 = det2(j, l, n, p);
	c3 = det2(j, k, n, o);
	c2 = det2(i, l, m, p);
	c1 = det2(i, k, m, o);
	c0 = det2(i, j, m, n);

	deter = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

	if (deter == 0) {
		identity();
		return *this;
	}

	denom = T(1) / deter;

	mat4<T> mat = *this;

	a = (mat.f * c5 - mat.g * c4 + mat.h * c3) * denom;
	b = (-mat.b * c5 + mat.c * c4 - mat.d * c3) * denom;
	c = (mat.n * s5 - mat.o * s4 + mat.p * s3) * denom;
	d = (-mat.j * s5 + mat.k * s4 - mat.l * s3) * denom;

	e = (-mat.e * c5 + mat.g * c2 - mat.h * c1) * denom;
	f = (mat.a * c5 - mat.c * c2 + mat.d * c1) * denom;
	g = (-mat.m * s5 + mat.o * s2 - mat.p * s1) * denom;
	h = (mat.i * s5 - mat.k * s2 + mat.l * s1) * denom;

	i = (mat.e * c4 - mat.f * c2 + mat.h * c0) * denom;
	j = (-mat.a * c4 + mat.b * c2 - mat.d * c0) * denom;
	k = (mat.m * s4 - mat.n * s2 + mat.p * s0) * denom;
	l = (-mat.i * s4 + mat.j * s2 - mat.l * s0) * denom;

	m = (-mat.e * c3 + mat.f * c1 - mat.g * c0) * denom;
	n = (mat.a * c3 - mat.b * c1 + mat.c * c0) * denom;
	o = (-mat.m * s3 + mat.n * s1 - mat.o * s0) * denom;
	p = (mat.i * s3 - mat.j * s1 + mat.k * s0) * denom;

	return *this;
}

template<class T>
inline mat4<T> &mat4<T>::invert()
{
	return inv();
}

template<class T>
inline mat4<T> mat4<T>::getInv() const
{
	return getInverse();
}

template<class T>
inline mat4<T> mat4<T>::getInverse() const
{
	mat4<T> mat = *this;
	mat.inv();
	return mat;
}

template<class T>
inline T mat4<T>::trace() const
{
	return a + f + k + p;
}

template<class T>
inline mat3<T> mat4<T>::rotationMatrix() const
{
	return mat3<T>(a, b, c,
		e, f, g,
		i, j, k);
}

template<class T>
inline vec4<T> mat4<T>::getRow(u32 row) const
{
	return vec4<T>(
		mat[row][0],
		mat[row][1],
		mat[row][2],
		mat[row][3]);
}

template<class T>
inline void mat4<T>::setRow(u32 row, const vec4<T> &vec)
{
	mat[row][0] = vec.x;
	mat[row][1] = vec.y;
	mat[row][2] = vec.z;
	mat[row][3] = vec.w;
}

template<class T>
inline vec4<T> mat4<T>::getCol(u32 col) const
{
	return vec4<T>(
		mat[0][col],
		mat[1][col],
		mat[2][col],
		mat[3][col]);
}

template<class T>
inline void mat4<T>::setCol(u32 col, const vec4<T> &vec)
{
	mat[0][col] = vec.x;
	mat[1][col] = vec.y;
	mat[2][col] = vec.z;
	mat[3][col] = vec.w;
}



/* Matlab-friendly print function */
template<class T>
inline void mat4<T>::print()
{
	char buf[4096];

	sprintf(buf, "[ %f %f %f %f ;\n %f %f %f %f ;\n %f %f %f %f ;\n %f %f %f %f ]\n",
		a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

	printf("%s\n", buf);
}

template<class T>
inline mat4<T> operator+(T lv, mat4<T> &rv)
{
	return rv + lv;
}

template<class T>
inline mat4<T> operator-(T lv, mat4<T> &rv)
{
	return - rv + lv;
}

template<class T>
inline mat4<T> operator*(T lv, mat4<T> &rv)
{
	return rv * lv;
}



#endif // VMATH



