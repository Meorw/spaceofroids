/*
 * Vector and Matrix operations (along with various other constants and math
 * stuff...)
 */

/*
TODO:
 * Constructor with length and rotation angle for vectors
 * Affine transforms for 3x3 matrix
 * Fix left multiplication
 * Left multiply affine with vector
 */

#ifndef VECTORMATH_H
#define VECTORMATH_H

#ifdef WIN32
#	pragma warning(disable: 4201) // Non standard, nameless struct/union
//#	pragma warning(disable: 4100) // Unreferenced parameter
#	pragma warning(disable: 4706) // Assignment within conditional expr.
#	pragma warning(disable: 4458) // declaration of hides class member
#endif // WIN32

#ifndef SKIP_TYPEDEFS
	typedef float				f32;
	typedef	double				f64;
	typedef signed char			i8;
	typedef unsigned char		u8;
	typedef signed short		i16;
	typedef unsigned short		u16;
	typedef signed int			i32;
	typedef unsigned int		u32;
#	ifdef WIN32
		typedef signed __int64		i64;
		typedef unsigned __int64	u64;
#	else // WIN32
typedef signed long long	i64;
typedef unsigned long long	u64;
#	endif // WIN32
#endif // SKIP_TYPEDEFS

//#define SSE
//#define SSE2

//#ifdef MMX
//#include <mmintrin.h>
//#endif
#define SSE_VER 0
#ifdef SSE
#	include <xmmintrin.h>
#	undef SSE_VER
#	define SSE_VER 1
#endif // SSE
#ifdef SSE2
#	include <emmintrin.h>
#	undef SSE_VER
#	define SSE_VER 2
#endif // SSE2
#ifdef SSE3
#	include <pmmintrin.h>
#	undef SSE_VER
#	define SSE_VER 3
#endif // SSE3
#ifdef SSSE3
#	include <tmmintrin.h>
#	undef SSE_VER
#	define SSE_VER 3
#endif // SSSE3
#if defined SSE4_1 || defined SSE4_2
#	include <smmintrin.h>
#	undef SSE_VER
#	define SSE_VER 4
#endif // SSE4_1 || SSE4_2

#if defined SSE || defined SSE2 || defined SSE3 || defined SSSE3 || defined SSE4_1 || defined SSE4_2
#	define SIMDf			4
#	define SIMD_SHIFTf		2
#	define SIMDd			2
#	define SIMD_SHIFTd		1
#else // SSE...
#	define SIMDf			1
#	define SIMD_SHIFTf		0
#	define SIMDd			1
#	define SIMD_SHIFTd		0
#	define NO_WIDE
#endif // SSE...


#ifndef NOMINMAX
#define MIN(a,b) (a<=b?a:b)
#define MAX(a,b) (a>=b?a:b)
#endif

/*===========================================================[ Constants ]====*/

#define fPI					3.141592653589793238462643383280f
#define f2PI				6.283185307179586476925286766560f
#define fPIinv				0.318309886183790671537767526745f
#define f2PIinv				0,159154943091895335768883763373f

#define fMin				1.17549e-038f
#define fMax				3.40282e+038f

#define dMin				2.2250738585072014e-308
#define dMax				1.7976931348623158e+308

/*============================================================[ Typedefs ]====*/

template<class T> class vec2;
template<class T> class vec3;
template<class T> class vec4;

template<class T> class mat2;
template<class T> class mat3;
template<class T> class mat4;

template<class T> class box3;
template<class T> class ray3;

typedef vec2<f32>			vec2f;
typedef vec2<f64>			vec2d;
typedef vec2<i32>			vec2i;
typedef vec2<u32>			vec2u;
typedef vec3<f32>			vec3f;
typedef vec3<f64>			vec3d;
typedef vec3<i32>			vec3i;
typedef vec3<u32>			vec3u;
typedef vec3<i8>			vec3c;
typedef vec3<u8>			vec3b;
typedef vec4<f32>			vec4f;
typedef vec4<f64>			vec4d;
typedef vec4<i32>			vec4i;
typedef vec4<u32>			vec4u;
typedef vec4<i8>			vec4c;
typedef vec4<u8>			vec4b;

typedef mat2<f32>			mat2f;
typedef mat2<f64>			mat2d;
typedef mat3<f32>			mat3f;
typedef mat3<f64>			mat3d;
typedef mat4<f32>			mat4f;
typedef mat4<f64>			mat4d;

/*==============================================================[ Scalar ]====*/

template<class T>
inline T sabs(T a) {
	return a < ((T) 0) ? -a : a;
}
template<class T>
inline T smax2(T a, T b) {
	return a > b ? a : b;
}
template<class T>
inline T smin2(T a, T b) {
	return a < b ? a : b;
}
template<class T>
inline T smax3(T a, T b, T c) {
	return a > b ? smax2<T>(a, c) : smax2<T>(b, c);
}
template<class T>
inline T smid3(T a, T b, T c) {
	return a > b ? (a < c ? a : (b > c ? b : c)) : (b < c ? b : (a > c ? a : c));
}
template<class T>
inline T smin3(T a, T b, T c) {
	return a < b ? smin2<T>(a, c) : smin2<T>(b, c);
}
template<class T>
inline T smax4(T a, T b, T c, T d) {
	return smax2<T>(smax2<T>(a, b), smax2<T>(c, d));
}
template<class T>
inline T smin4(T a, T b, T c, T d) {
	return smin2<T>(smin2<T>(a, b), smin2<T>(c, d));
}
template<class T>
inline i32 smaxi2(T a, T b) {
	return a > b ? 0 : 1;
}
template<class T>
inline i32 smini2(T a, T b) {
	return a < b ? 0 : 1;
}
template<class T>
inline i32 smaxi3(T a, T b, T c) {
	return a > c ? smaxi2<T>(a, b) : (smaxi2<T>(b, c) + 1);
}
template<class T>
inline i32 smidi3(T a, T b, T c) {
	return a > b ? (a < c ? 0 : (b > c ? 1 : 2)) : (b < c ? 1 : (a > c ? 0 : 2));
}
template<class T>
inline i32 smini3(T a, T b, T c) {
	return a < c ? smini2<T>(a, b) : (smini2<T>(b, c) + 1);
}
template<class T>
inline i32 smaxi4(T a, T b, T c, T d) {
	return smaxi2<T>(smaxi2<T>(a, b), smaxi2<T>(c, d) + 2);
}
template<class T>
inline i32 smini4(T a, T b, T c, T d) {
	return smini2<T>(smini2<T>(a, b), smini2<T>(c, d) + 2);
}
template<class T>
inline T sclamp(T s, T a, T b) {
	return s > b ? b : (s < a ? a : s);
}
template<class T>
inline T sclamp(T s) {
	return sclamp(s, (T) 0, (T) 1);
}
template<class T>
inline void sswap(T *a, T *b) {
	T t = *a;
	*a = *b;
	*b = t;
}

#define toRadf(a)				((a) * 0.01745329251994329576923690768489f)
#define toDegf(a)				((a) * 57.29577951308232087679815481428f)
#define toRadd(a)				((a) * 0.01745329251994329576923690768489)
#define toDegd(a)				((a) * 57.29577951308232087679815481428)

/*===========================================================[ 2D Vector ]====*/

template<class T> 
class vec2
{
	public:
		/* Members */
		union {
			struct {
				T x, y;
			};
			struct {
				T u, v;
			};

			T vec[2];
		};

		static vec2<T> nullVector() {return vec2<T>((T) 0, (T) 0);}

		/* Constructors */
		vec2() {}
		vec2(const vec2<T> &v) : x(v.x), y(v.y) {}
		vec2(T nx, T ny) : x(nx), y(ny) {}

		/* Subscript */
		T &operator[](u32 idx) { return vec[idx]; }
		const T operator[](u32 idx) const { return vec[idx]; }

		/* Assignment */
		vec2<T> &operator=(const vec2<T> &v);
		vec2<T> &operator=(T v);

		/* Comparison */
		bool operator==(const vec2<T> &v) const;
		bool operator!=(const vec2<T> &v) const;

		/* Addition */
		vec2<T> operator+(const vec2<T> &v) const;
		vec2<T> &operator+=(const vec2<T> &v);
		vec2<T> operator+(T v) const;
		vec2<T> &operator+=(T v);
		vec2<T> operator+() const;

		/* Subtraction */
		vec2<T> operator-(const vec2<T>	&v) const;
		vec2<T> &operator-=(const vec2<T> &v);
		vec2<T> operator-(T v) const;
		vec2<T> &operator-=(T v);
		vec2<T> operator-() const;

		/* Element-wise multiplication */
		vec2<T> operator&(const vec2<T>	&v) const;
		vec2<T> &operator&=(const vec2<T> &v);

		/* Multiplication/Dot product */
		vec2<T> operator*(const mat2<T>	&v) const;
		vec2<T> &operator*=(const mat2<T> &v);
		vec2<T> operator*(T v) const;
		vec2<T> &operator*=(T v);
		T dot(const vec2<T> &v) const;
		T operator*(const vec2<T> &v) const;

		/* Division */
		vec2<T> operator/(const vec2<T>	&v) const;
		vec2<T> &operator/=(const vec2<T> &v);
		vec2<T> operator/(T v) const;
		vec2<T> &operator/=(T v);

		/* Misc useful stuff */
		T minElement() const;
		T maxElement() const;
		vec2<T> &clamp(T min, T max);
		vec2<T> getClamped(T min, T max) const;
		vec2<T> &clamp(const vec2<T> &min, const vec2<T> &max);
		vec2<T> getClamped(const vec2<T> &min, const vec2<T> &max) const;
		vec2<T> &saturate();
		vec2<T> getSaturated() const;

		vec2<T> &normalize();
		vec2<T> getNormalized() const;

		T len() const;
		T length() const;
		T len2() const;
		T length2() const;
		T lengthSquared() const;

		vec2<T> reflection(const vec2<T> &v) const;
		vec2<T> refraction(const vec2<T> &v, T r) const;
		vec2<T> projectOn(const vec2<T> &v) const;
		T projectionLengthOn(const vec2<T> &v) const;
		vec2<T> &rotate(T rad);
		vec2<T> getRotated(T rad) const;
};

/* Scalar-vector operations */
template<class T> vec2<T> operator+(T lv, vec2<T> &rv);
template<class T> vec2<T> operator-(T lv, vec2<T> &rv);
template<class T> vec2<T> operator*(T lv, vec2<T> &rv);
template<class T> vec2<T> operator/(T lv, vec2<T> &rv);
template<class T> vec2<T> operator+(T lv, const vec2<T> &rv);
template<class T> vec2<T> operator-(T lv, const vec2<T> &rv);
template<class T> vec2<T> operator*(T lv, const vec2<T> &rv);
template<class T> vec2<T> operator/(T lv, const vec2<T> &rv);

/*===========================================================[ 3D Vector ]====*/

template<class T> 
class vec3
{
	public:
		/* Members */
		union {
			struct {
				T x, y;
				union {
					T z;
					T t;
				};
			};
			struct {
				T u, v, w;
			};
			struct {
				T r, g, b;
			};

			T vec[3];
		};

		static vec3<T> nullVector() {return vec3<T>((T) 0, (T) 0, (T) 0);}

		/* Constructors */
		vec3() {};
		vec3(const vec3<T> &v) : x(v.x), y(v.y), z(v.z) {}
		vec3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
		vec3(T n) : x(n), y(n), z(n) {}
		vec3(T nxyz[3]) : x(nxyz[0]), y(nxyz[1]), z(nxyz[2]) {}

		/* Subscript */
		T &operator[](u32 idx) { return vec[idx]; }
		const T operator[](u32 idx) const { return vec[idx]; }

		/* Assignment */
		vec3<T> &operator=(const vec3<T> &v);
		vec3<T> &operator=(T v);

		/* Comparison */
		bool operator==(const vec3<T> &v) const;
		bool operator!=(const vec3<T> &v) const;

		/* Addition */
		vec3<T> operator+(const vec3<T> &v) const;
		vec3<T> &operator+=(const vec3<T> &v);
		vec3<T> operator+(T v) const;
		vec3<T> &operator+=(T v);
		vec3<T> operator+() const;

		/* Subtraction */
		vec3<T> operator-(const vec3<T> &v) const;
		vec3<T> &operator-=(const vec3<T> &v);
		vec3<T> operator-(T v) const;
		vec3<T> &operator-=(T v);
		vec3<T> operator-() const;

		/* Element-wise multiplication */
		vec3<T> operator&(const vec3<T> &v) const;
		vec3<T> &operator&=(const vec3<T> &v);

		/* Multiplication/Dot product */
		vec3<T> operator*(const mat3<T> &v) const;
		vec3<T> &operator*=(const mat3<T> &v);
		vec3<T> operator*(T v) const;
		vec3<T> &operator*=(T v);
		T dot(const vec3<T> &v) const;
		T operator*(const vec3<T> &v) const;

		/* Cross product */
		vec3<T> cross(const vec3<T> &v) const;
		T crossX(const vec3<T> &v) const;
		T crossY(const vec3<T> &v) const;
		T crossZ(const vec3<T> &v) const;
		vec3<T> operator^(const vec3<T>	&v) const;

		/* Division */
		vec3<T> operator/(const vec3<T> &v) const;
		vec3<T> &operator/=(const vec3<T> &v);
		vec3<T> operator/(T v) const;
		vec3<T> &operator/=(T v);

		/* Misc useful stuff */
		T minElement() const;
		T maxElement() const;
		vec3<T> &clamp(T min, T max);
		vec3<T> getClamped(T min, T max) const;
		vec3<T> &clamp(const vec3<T> &min, const vec3<T> &max);
		vec3<T> getClamped(const vec3<T> &min, const vec3<T> &max) const;
		vec3<T> &saturate();
		vec3<T> getSaturated() const;
		
		vec3<T> &normalize();
		vec3<T> getNormalized() const;
		T len() const;
		T length() const;
		T len2() const;
		T length2() const;
		T lengthSquared() const;

		vec3<T> reflection(const vec3<T> &v) const;
		vec3<T> refraction(const vec3<T> &v, T r) const;
		vec3<T> projectOn(const vec3<T> &v) const;
		f32 projectionLengthOn(const vec3<T> &v) const;
		
		vec4<T> xyz0() const;
		vec4<T> xyz1() const;
		vec2<T> xy() const;

		vec3<T> &rotateX(T rad);
		vec3<T> &rotateY(T rad);
		vec3<T> &preRotateY(T rad);
		vec3<T> &rotateZ(T rad);
		vec3<T> &rotate(T rad, const vec3<T> axis);
		inline vec3<T> &rotate(T rad, T x, T y, T z) {vec3<T> v(x, y, t); return rotate(rad, v);} // LOWPRIO: To cpp

		vec3<T> getRotatedX(T rad) const;
		vec3<T> getRotatedY(T rad) const;
		vec3<T> getRotatedZ(T rad) const;
		vec3<T> getRotated(T rad, const vec3<T> axis) const;
		inline vec3<T> getRotated(T rad, T x, T y, T z) const {vec3<T> v(x, y, t); return getRotated(rad, v);}
};

/* Scalar-vector operations */
template<class T> vec3<T> operator+(T lv, vec3<T> &rv);
template<class T> vec3<T> operator-(T lv, vec3<T> &rv);
template<class T> vec3<T> operator*(T lv, vec3<T> &rv);
template<class T> vec3<T> operator/(T lv, vec3<T> &rv);
template<class T> vec3<T> operator+(T lv, const vec3<T> &rv);
template<class T> vec3<T> operator-(T lv, const vec3<T> &rv);
template<class T> vec3<T> operator*(T lv, const vec3<T> &rv);
template<class T> vec3<T> operator/(T lv, const vec3<T> &rv);

/*===========================================================[ 4D Vector ]====*/

template<class T> 
class vec4
{
	public:

		/* Members */
		union {
			struct {
				T x, y, z, w;
			};
			struct {
				T r, g, b, a;
			};

			T vec[4];
		};

		static vec4<T> nullVector() {return vec4<T>((T) 0, (T) 0, (T) 0, (T) 0);}

		/* Constructors */
		vec4() {}
		vec4(const vec4<T> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		vec4(const vec3<T> &v, T nw) : x(v.x), y(v.y), z(v.z), w(nw) {}
		vec4(T nx, T ny, T nz, T nw) : x(nx), y(ny), z(nz), w(nw) {}
		vec4(T nxyzw[4]) : x(nxyzw[0]), y(nxyzw[1]), z(nxyzw[2]), w(nxyzw[3]) {}

		/* Subscript */
		/* TODO: Fix! */
		T &operator[](u32 idx) { return vec[idx]; }
		const T operator[](u32 idx) const { return vec[idx]; }

		/* Assignment */
		vec4<T> &operator=(const vec4<T> &v);
		vec4<T> &operator=(T v);

		/* Comparison */
		bool operator==(const vec4<T> &v) const;
		bool operator!=(const vec4<T> &v) const;

		/* Addition */
		vec4<T> operator+(const vec4<T>	&v) const;
		vec4<T> &operator+=(const vec4<T> &v);
		vec4<T> operator+(T v) const;
		vec4<T> &operator+=(T v);
		vec4<T> operator+() const;

		/* Subtraction */
		vec4<T> operator-(const vec4<T> &v) const;
		vec4<T> &operator-=(const vec4<T> &v);
		vec4<T> operator-(T v) const;
		vec4<T> &operator-=(T v);
		vec4<T> operator-() const;

		/* Element-wise multiplication */
		vec4<T> operator&(const vec4<T> &v) const;
		vec4<T> &operator&=(const vec4<T> &v);

		/* Multiplication/Dot product */
		vec4<T> operator*(const mat4<T> &v) const;
		vec4<T> &operator*=(const mat4<T> &v);
		vec4<T> operator*(T v) const;
		vec4<T> &operator*=(T v);
		T dot(const vec4<T> &v) const;
		T operator*(const vec4<T> &v) const;

		/* Division */
		vec4<T> operator/(const vec4<T> &v) const;
		vec4<T> &operator/=(const vec4<T> &v);
		vec4<T> operator/(T v) const;
		vec4<T> &operator/=(T v);

		/* Misc useful stuff */
		T minElement() const;
		T maxElement() const;
		vec4<T> &clamp(T min, T max);
		vec4<T> getClamped(T min, T max) const;
		vec4<T> &clamp(const vec4<T> &min, const vec4<T> &max);
		vec4<T> getClamped(const vec4<T> &min, const vec4<T> &max) const;
		vec4<T> &saturate();
		vec4<T> getSaturated() const;
		
		vec4<T> &normalize();
		vec4<T> getNormalized() const;
	
		T len() const;
		T length() const;
		T len2() const;
		T length2() const;
		T lengthSquared() const;

		vec4<T> reflection(const vec4<T> &v) const;
		vec4<T> refraction(const vec4<T> &v, T r) const;
		vec4<T> projectOn(const vec4<T> &v) const;
		f32 projectionLengthOn(const vec4<T> &v) const;
		vec3<T> xyz() const;
		vec2<T> xy() const;
};

/* Scalar-vector operations */
template<class T> vec4<T> operator+(T lv, vec4<T> &rv);
template<class T> vec4<T> operator-(T lv, vec4<T> &rv);
template<class T> vec4<T> operator*(T lv, vec4<T> &rv);
template<class T> vec4<T> operator/(T lv, vec4<T> &rv);
template<class T> vec4<T> operator+(T lv, const vec4<T> &rv);
template<class T> vec4<T> operator-(T lv, const vec4<T> &rv);
template<class T> vec4<T> operator*(T lv, const vec4<T> &rv);
template<class T> vec4<T> operator/(T lv, const vec4<T> &rv);

/*===========================================================[ ND Vector ]====*/

// TODO

/*==========================================================[ 2x2 Matrix ]====*/

template<class T>
class mat2
{
	public:
		friend class vec2<T>;

		/* Members */
		union {
			struct {
				T			a, b, c, d;
			};
			T				vec[4];
			T				mat[2][2];
		};

		static mat2<T> identityMatrix() {return mat2<T>(1, 0, 0, 1);}

		/* Constructors */
		mat2() : a(1), b(0), c(0), d(1) {}
		mat2(const mat2<T> &v) : a(v.a), b(v.b), c(v.c), d(v.d) {}
		mat2(T na, T nb, T nc, T nd) : a(na), b(nb), c(nc), d(nd) {}

		/* Assignment */
		mat2<T> &operator=(const mat2<T> &v);

		/* Comparison */
		bool operator==(const mat2<T> &v) const;
		bool operator!=(const mat2<T> &v) const;
		bool isIdentity() const;

		/* Addition */
		mat2<T> operator+(const mat2<T> &v) const;
		mat2<T> &operator+=(const mat2<T> &v);
		mat2<T> operator+(T v) const;
		mat2<T> &operator+=(T v);
		mat2<T> operator+() const;

		/* Subtraction */
		mat2<T> operator-(const mat2<T> &v) const;
		mat2<T> &operator-=(const mat2<T> &v);
		mat2<T> operator-(T v) const;
		mat2<T> &operator-=(T v);
		mat2<T> operator-() const;

		/* Inner product */
		T iprod(const mat2<T> &v, u32 row, u32 col) const;

		/* Multiplication */
		mat2<T> operator*(const mat2<T>	&v) const;
		mat2<T> &operator*=(const mat2<T> &v);
		mat2<T> operator*(T v) const;
		mat2<T> &operator*=(T v);
		vec2<T> operator*(const vec2<T>	&v) const;

		/* Division */
		mat2<T> operator/(T v) const;
		mat2<T> &operator/=(T v);

		/* Element-wise multiplication */
		mat2<T> elemMul(const mat2<T> &v) const; // TODO: Overload &?

		/* Division with scalar */
		mat2<T> operator/(const mat2<T> &v) const;
		mat2<T> &operator/=(const mat2<T> &v);

		/* Misc matrix operations */
		mat2<T> &identity();
		mat2<T> &transpose();
		mat2<T> getTranspose() const;
		T det() const;
		T determinant() const;
		mat2<T> &inv();
		mat2<T> &invert();
		mat2<T> getInv() const;
		mat2<T> getInverse() const;
		T trace() const;
		vec2<T> eigenvalues() const;
		mat2<T> eigenvectors(const vec2<T> &eig) const;

		vec2<T> getRow(u32 row) const;
		void setRow(u32 row, const vec2<T> &vec);
		vec2<T> getCol(u32 col) const;
		void setCol(u32 col, const vec2<T> &vec);

		vec2<T> solveSystem(const vec2<T> &eq) const;

		mat2<T> &rotate(T rad);
		mat2<T> getRotated(T rad) const;

		vec2<T> up() const;
		vec2<T> down() const;
		vec2<T> left() const;
		vec2<T> right() const;
};

/* Scalar-matrix operations */
template<class T> mat2<T> operator+(T lv, mat2<T> &rv);
template<class T> mat2<T> operator-(T lv, mat2<T> &rv);
template<class T> mat2<T> operator*(T lv, mat2<T> &rv);

/*==========================================================[ 3x3 Matrix ]====*/

template<class T>
class mat3
{
	public:
		friend class vec3<T>;

		/* Members */
		union {
			struct {
				T			a, b, c, d, e, f, g, h, i;
			};
			T				vec[9];
			T				mat[3][3];
		};

		static mat3<T> identityMatrix() {return mat3<T>(1, 0, 0, 0, 1, 0, 0, 0, 1);}
	
		/* Constructors */
		mat3() : a(1), b(0), c(0),
				 d(0), e(1), f(0),
				 g(0), h(0), i(1) {}
		mat3(const mat3<T>	&v) :
				a(v.a), b(v.b), c(v.c),
				d(v.d), e(v.e), f(v.f),
				g(v.g), h(v.h), i(v.i) {}
		mat3(T na, T nb, T nc, T nd, T ne, T nf, T ng, T nh, T ni) :
				a(na), b(nb), c(nc),
				d(nd), e(ne), f(nf),
				g(ng), h(nh), i(ni) {}
		mat3(const vec3<T> &row0, const vec3<T> &row1,const vec3<T> &row2) :
				a(row0.x), b(row0.y), c(row0.z),
				d(row1.x), e(row1.y), f(row1.z),
				g(row2.x), h(row2.y), i(row2.z) {}

		static mat3<T> getRotationMatrix(T rad, const vec3<T> &axis);
		static mat3<T> getRotationXMatrix(T rad);
		static mat3<T> getRotationYMatrix(T rad);
		static mat3<T> getRotationZMatrix(T rad);

		/* Assignment */
		mat3<T> &operator=(const mat3<T> &v);

		/* Comparison */
		bool operator==(const mat3<T> &v) const;
		bool operator!=(const mat3<T> &v) const;
		bool isIdentity() const;

		/* Addition */
		mat3<T> operator+(const mat3<T> &v) const;
		mat3<T> &operator+=(const mat3<T> &v);
		mat3<T> operator+(T v) const;
		mat3<T> &operator+=(T v);
		mat3<T> operator+() const;

		/* Subtraction */
		mat3<T> operator-(const mat3<T> &v) const;
		mat3<T> &operator-=(const mat3<T> &v);
		mat3<T> operator-(T v) const;
		mat3<T> &operator-=(T v);
		mat3<T> operator-() const;

		/* Inner product */
		T iprod(const mat3<T> &v, const u32 row, const u32 col) const;

		/* Multiplication */
		mat3<T> operator*(const mat3<T> &v) const;
		mat3<T> &operator*=(const mat3<T> &v);
		mat3<T> operator*(T v) const;
		mat3<T> &operator*=(T v);
		vec3<T> operator*(const vec3<T> &v) const;

		/* Division */
		mat3<T> operator/(T v) const;
		mat3<T> &operator/=(T v);

		/* Element-wise multiplication */
		mat3<T> elemMul(const mat3<T> &v) const;

		/* Division with scalar */
		mat3<T> operator/(const mat3<T> &v) const;
		mat3<T> &operator/=(const mat3<T> &v);

		/* Misc matrix operations */
		mat3<T> &identity();
		mat3<T> &transpose();
		mat3<T> getTranspose() const;

		T det() const;
		T determinant() const;

		mat3<T> &inv();
		mat3<T> &invert();
		mat3<T> getInv() const;
		mat3<T> getInverse() const;
		T trace() const;

		vec3<T> getRow(u32 row) const;
		void setRow(u32 row, const vec3<T> &vec);
		vec3<T> getCol(u32 col) const;
		void setCol(u32 col, const vec3<T> &vec);

		mat3<T> &rotateX(T rad);
		mat3<T> &rotateY(T rad);
		mat3<T> &preRotateY(T rad);
		mat3<T> &rotateZ(T rad);
		mat3<T> &rotate(T rad, const vec3<T> axis);
		inline mat3<T> &rotate(T rad, T x, T y, T z) {vec3<T> v(x, y, z);
            return rotate(rad, v);}
		mat3<T> getRotatedX(T rad) const;
		mat3<T> getRotatedY(T rad) const;
		mat3<T> getRotatedZ(T rad) const;
		mat3<T> getRotated(T rad, const vec3<T> axis) const;
		inline mat3<T> getRotated(T rad, T x, T y, T z) const {vec3<T> v(x, y, z);
            return getRotated(rad, v);}

		mat3<T> &scale(T sc);
		mat3<T> &scale(T sx, T sy, T sz);
		mat3<T> getScaled(T sc) const;
		mat3<T> getScaled(T sx, T sy, T sz) const;

		vec3<T> up() const;
		vec3<T> down() const;
		vec3<T> left() const;
		vec3<T> right() const;
		vec3<T> front() const;
		vec3<T> back() const;
};

/* Scalar-vector operations */
template<class T> mat3<T> operator+(T lv, mat3<T> &rv);
template<class T> mat3<T> operator-(T lv, mat3<T> &rv);
template<class T> mat3<T> operator*(T lv, mat3<T> &rv);

/*==========================================================[ 4x4 Matrix ]====*/

template<class T>
class mat4
{
	public:
		/* Members */
		union {
			struct {
				T			a, b, c, d;
				T			e, f, g, h;
				T			i, j, k, l;
				T			m, n, o, p;
			};
			struct {
				T			R00, R10, R20, T0;
				T			R01, R11, R21, T1;
				T			R02, R12, R22, T2;
				T			N0, N1, N2, N3;
			};
			T				vec[16];
			T				mat[4][4];
		};

		static mat4<T> identityMatrix() {return mat4<T>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);}
	
		/* Constructors */
		mat4() : a(1), b(0), c(0), d(0),
				 e(0), f(1), g(0), h(0),
				 i(0), j(0), k(1), l(0),
				 m(0), n(0), o(0), p(1) {}
		mat4(const mat4<T>&v) :
				a(v.a), b(v.b), c(v.c), d(v.d),
				e(v.e), f(v.f), g(v.g), h(v.h),
				i(v.i), j(v.j), k(v.k), l(v.l),
				m(v.m), n(v.n), o(v.o), p(v.p) {}
		mat4(T na, T nb, T nc, T nd,
			 T ne, T nf, T ng, T nh,
			 T ni, T nj, T nk, T nl,
			 T nm, T nn, T no, T np) :
				a(na), b(nb), c(nc), d(nd),
				e(ne), f(nf), g(ng), h(nh),
				i(ni), j(nj), k(nk), l(nl),
				m(nm), n(nn), o(no), p(np) {}

		/* Assignment */
		mat4<T> &operator=(const mat4<T> &v);

		/* Comparison */
		bool operator==(const mat4<T> &v) const;
		bool operator!=(const mat4<T> &v) const;
		bool isIdentity() const;

		/* Addition */
		mat4<T> operator+(const mat4<T> &v) const;
		mat4<T> &operator+=(const mat4<T> &v);
		mat4<T> operator+(T v) const;
		mat4<T> &operator+=(T v);
		mat4<T> operator+() const;

		/* Subtraction */
		mat4<T> operator-(const mat4<T> &v) const;
		mat4<T> &operator-=(const mat4<T> &v);
		mat4<T> operator-(T v) const;
		mat4<T> &operator-=(T v);
		mat4<T> operator-() const;

		/* Inner product */
		T iprod(const mat4<T> &v, const u32 row, const u32 col) const;

		/* Multiplication */
		mat4<T> operator*(const mat4<T>	&v) const;
		mat4<T> &operator*=(const mat4<T> &v);
		mat4<T> operator*(T v) const;
		mat4<T> &operator*=(T v);
		vec4<T> operator*(const vec4<T> &v) const;
		mat4<T> affineMul(const mat4<T> &v) const;
		vec4<T> affineMul(const vec4<T> &v) const;
		vec3<T> affineMul(const vec3<T> &v) const;

		/* Division */
		mat4<T> operator/(T v) const;
		mat4<T> &operator/=(T v);

		/* Element-wise multiplication */
		mat4<T> elemMul(const mat4<T> &v) const;

		/* Division with scalar */
		mat4<T> operator/(const mat4<T> &v) const;
		mat4<T> &operator/=(const mat4<T> &v);

		/* Misc matrix operations */
		mat4<T> &identity();
		mat4<T> &transpose();
		mat4<T> getTranspose() const;

		T det() const;
		T determinant() const;

		mat4<T> &inv();
		mat4<T> &invert();
		mat4<T> getInv() const;
		mat4<T> getInverse() const;

		T trace() const;
		mat3<T> rotationMatrix() const;
		vec4<T> getRow(u32 row) const;
		void setRow(u32 row, const vec4<T> &vec);
		vec4<T> getCol(u32 col) const;
		void setCol(u32 col, const vec4<T> &vec);

		void print();

		// TODO: Rotate and scale

	private:
		T det2(T n0, T n1, T n2, T n3) const;
		T affineiprod0(const mat4<T> &v, const u32 row, const u32 col) const;
		T affineiprod1(const mat4<T> &v, const u32 row, const u32 col) const;
};

/* Scalar-vector operations */
template<class T> mat4<T> operator+(T lv, mat4<T> &rv);
template<class T> mat4<T> operator-(T lv, mat4<T> &rv);
template<class T> mat4<T> operator*(T lv, mat4<T> &rv);

/*===================================================[ 4x3 Affine Matrix ]====*/



/*==========================================================[ NxM Matrix ]====*/

// TODO

/*=========================================================[ Quaternions ]====*/

/*================================================[ Inline ]====*/

#define VMATH
#include "VectorMath.cpp"
#undef VMATH

#endif /* VECTORMATH_H */

