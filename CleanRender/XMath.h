#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <unordered_map>
//#include <xmmintrin.h> // SSE
//#include <emmintrin.h> // SSE2
//#include <pmmintrin.h> // SSE3
//#include <tmmintrin.h> // SSSE3
#include <smmintrin.h> // SSE4.1

#pragma region Constants
const __m128 _mm_one = _mm_set1_ps(1.0f);
const __m128 _mm_negOne = _mm_set1_ps(-1.0f);
const __m128 _mm_mask_signf = _mm_castsi128_ps(_mm_set1_epi32(0x80'00'00'00));
#pragma endregion

#pragma region MathFunctions
inline float sqrtfInline(float x) {
	if (x <= 0) {
		return 0;
	}
	int exp = 0;
	x = frexpf(x, &exp);
	if (exp & 1) {
		exp--;
		x *= 2;
	}
	float y = (1 + x) / 2;
	float z = 0;
	while (y != z) {
		z = y;
		y = (y + x / y) / 2;
	}
	return ldexpf(y, exp / 2);
}

inline float maxf(const float &a, const float &b) {
	return (a > b) ? a : b;
}

inline float minf(const float &a, const float &b) {
	return (a < b) ? a : b;
}

inline void minmax3f(const float &a, const float &b, const float &c, float &min, float&max) {
	if (a < b) {
		if (b < c) {
			min = a;
			max = c;
		} else {
			if (a < c) {
				min = a;
				max = b;
			} else {
				min = c;
				max = b;
			}
		}
	} else {
		if (b < c) {
			if (a < c) {
				min = b;
				max = c;
			} else {
				min = b;
				max = a;
			}
		} else {
			min = c;
			max = a;
		}
	}
}

inline double lerpd(double a, double b, double t) {
	return (1 - t) * a + t * b;
}

inline float lerpf(float a, float b, float t) {
	return (1 - t) * a + t * b;
}

inline double faded(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float fadef(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float invsqrt(const float &n) {
	union {
		float f;
		unsigned int i;
	} conv;

	float x2;
	const float threehalfs = 1.5f;

	x2 = n * 0.5f;
	conv.f = n;
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f = conv.f * (threehalfs - (conv.f * x2 * x2));
	return conv.f;
}

inline int floorToInt(double d) {
	return (d > 0) ? (int) d : (int) (d - 1);
}

inline int floorToInt(float f) {
	return (f > 0) ? (int) f : (int) (f - 1);
}
#pragma endregion

#pragma region Vectors
/* ==== VEC2i ==== */
struct Vec2i {
	int x;
	int y;
public:
	/* ---- CONSTANTS ---- */
	static const Vec2i zero;
	static const Vec2i one;
	static const Vec2i right;
	static const Vec2i left;
	static const Vec2i up;
	static const Vec2i down;

	/* ---- CONSTRUCTORS ---- */
	inline Vec2i() : x(0), y(0) { }
	inline Vec2i(int x, int y) : x(x), y(y) { }

	/* ---- OPERATORS ---- */
	// Add/Sub
	inline Vec2i operator+(const Vec2i &other) const {
		return {this->x + other.x, this->y + other.y};
	}
	inline Vec2i  operator-(const Vec2i &other) const {
		return {this->x - other.x, this->y - other.y};
	}
	inline void operator+=(const Vec2i &other) {
		this->x += other.x;
		this->y += other.y;
	}
	inline void operator-=(const Vec2i &other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Mul/Div
	inline friend Vec2i operator*(const Vec2i &vec, int mult) {
		return {vec.x * mult, vec.y * mult};
	}
	inline friend Vec2i operator*(int mult, const Vec2i &vec) {
		return {vec.x * mult, vec.y * mult};
	}

	inline friend Vec2i operator/(const Vec2i &vec, int mult) {
		return {vec.x / mult, vec.y / mult};
	}
	inline friend Vec2i operator/(int mult, const Vec2i &vec) {
		return {mult / vec.x, mult / vec.y};
	}

	inline friend Vec2i operator*(const Vec2i &a, const Vec2i &b) {
		return {a.x * b.x , a.y * b.y};
	}
	inline friend Vec2i operator/(const Vec2i &a, const Vec2i &b) {
		return {a.x / b.x , a.y / b.y};
	}

	// Comp
	inline bool operator==(const Vec2i &other) const {
		return (this->x == other.x) && (this->y == other.y);
	}
	inline bool operator!=(const Vec2i &other) const {
		return (this->x != other.x) || (this->y != other.y);
	}

	// Index
	inline const int& operator[](int index) const {
		return *((&x) + index);
	}
	inline int& operator[](int index) {
		return *((&x) + index);
	}
};

/* ==== VEC3i ==== */
struct Vec3i {
	union {
		struct {
			int x, y, z;
		};
		int data[3];
		__m128i _emm;
	};
public:
	/* ---- CONSTANTS ---- */
	static const Vec3i zero;
	static const Vec3i one;
	static const Vec3i right;
	static const Vec3i left;
	static const Vec3i up;
	static const Vec3i down;
	static const Vec3i forward;
	static const Vec3i backward;

	/* ---- CONSTRUCTORS ---- */
	inline Vec3i() : x(0), y(0), z(0) {}
	inline Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
	inline Vec3i(__m128i emm) : _emm(emm) { }

	/* ---- OPERATORS ---- */
	// Add/Sub
	inline Vec3i operator+(const Vec3i &other) const {
		return Vec3i(_mm_add_epi32(this->_emm, other._emm));
	}
	inline Vec3i  operator-(const Vec3i &other) const {
		return Vec3i(_mm_sub_epi32(this->_emm, other._emm));
	}
	inline void operator+=(const Vec3i &other) {
		_emm = _mm_add_epi32(_emm, other._emm);
	}
	inline void operator-=(const Vec3i &other) {
		_emm = _mm_sub_epi32(_emm, other._emm);
	}

	// Mul/Div
	inline friend Vec3i operator*(const Vec3i &vec, int mult) {
		return Vec3i(_mm_mul_epi32(vec._emm, _mm_set1_epi32(mult)));
	}
	inline friend Vec3i operator*(int mult, const Vec3i &vec) {
		return Vec3i(_mm_mul_epi32(vec._emm, _mm_set1_epi32(mult)));
	}

	inline friend Vec3i operator/(const Vec3i &vec, int mult) {
		return {vec.x / mult, vec.y / mult, vec.z / mult};
	}
	inline friend Vec3i operator/(int mult, const Vec3i &vec) {
		return {mult / vec.x, mult / vec.y, mult / vec.z};
	}

	inline friend Vec3i operator*(const Vec3i &a, const Vec3i &b) {
		return Vec3i(_mm_mul_epi32(a._emm, b._emm));
	}
	inline friend Vec3i operator/(const Vec3i &a, const Vec3i &b) {
		return {a.x / b.x , a.y / b.y, a.z / b.z};
	}

	// Comp
	inline bool operator==(const Vec3i &other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}
	inline bool operator!=(const Vec3i &other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
	}

	// Index
	inline const int& operator[](int index) const {
		return *((&x) + index);
	}
	inline int& operator[](int index) {
		return *((&x) + index);
	}
};
struct Vec3iHash {
	size_t operator()(const Vec3i& v) const {
		return (size_t) (v.x + v.y + v.z);
	}
};

/* ==== VEC2f ==== */
struct Vec2f {
	float x, y;
public:
	/* ---- CONSTANTS ---- */
	static const Vec2f zero;
	static const Vec2f one;
	static const Vec2f right;
	static const Vec2f left;
	static const Vec2f up;
	static const Vec2f down;

	/* ---- CONSTRUCTORS ---- */
	inline Vec2f() : x(0.0f), y(0.0f) { }
	inline Vec2f(float x, float y) : x(x), y(y) { }
	
	/* ---- OPERATORS ---- */
	// Misc
	inline Vec2f operator-() const {
		return {-this->x, -this->y};
	}

	// Add/Sub
	inline Vec2f operator+(const Vec2f &other) const {
		return {this->x + other.x, this->y + other.y};
	}
	inline Vec2f operator-(const Vec2f &other) const {
		return {this->x - other.x, this->y - other.y};
	}

	inline void operator+=(const Vec2f &other) {
		this->x += other.x;
		this->y += other.y;
	}
	inline void operator-=(const Vec2f &other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Mul/Div
	inline friend Vec2f operator*(const Vec2f &vec, float scalar) {
		return {vec.x * scalar, vec.y * scalar};
	}
	inline friend Vec2f operator*(float scalar, const Vec2f &vec) {
		return {vec.x * scalar, vec.y * scalar};
	}

	inline friend Vec2f operator/(const Vec2f &vec, float scalar) {
		return {vec.x / scalar, vec.y / scalar};
	}
	inline friend Vec2f operator/(float scalar, const Vec2f &vec) {
		return {scalar / vec.x, scalar / vec.y};
	}

	inline friend Vec2f operator*(const Vec2f &a, const Vec2f &b) {
		return {a.x * b.x , a.y * b.y};
	}
	inline friend Vec2f operator/(const Vec2f &a, const Vec2f &b) {
		return {a.x / b.x , a.y / b.y};
	}

	// Comp
	inline bool operator==(const Vec2f &other) const {
		return (this->x == other.x) && (this->y == other.y);
	}
	inline bool operator!=(const Vec2f &other) const {
		return (this->x != other.x) || (this->y != other.y);
	}

	// Index
	inline const float& operator[](int index) const {
		return *((&x) + index);
	}
	inline float& operator[](int index) {
		return *((&x) + index);
	}
};

/* ==== VEC3f ==== */
struct Vec3f {
	union {
		struct {
			float x, y, z;
		};
		float data[3];
		__m128 _xmm;
	};

public:
	/* ---- CONSTANTS ---- */
	static const Vec3f zero;
	static const Vec3f one;
	static const Vec3f right;
	static const Vec3f left;
	static const Vec3f up;
	static const Vec3f down;
	static const Vec3f forward;
	static const Vec3f backward;

	/* ---- CONSTRUCTORS ---- */
	inline Vec3f() : x(0.0f), y(0.0f), z(0.0f) { }
	inline Vec3f(float x, float y, float z) : x(x), y(y), z(z) { }
	inline Vec3f(__m128 xmm) : _xmm(xmm) { }
	
	/* ---- OPERATORS ---- */
	// Misc
	inline Vec3f operator-() const {
		return Vec3f(_mm_xor_ps(_xmm, _mm_mask_signf));
	}

	// Add/Sub
	inline Vec3f operator+(const Vec3f &other) const {
		return Vec3f(_mm_add_ps(_xmm, other._xmm));
	}
	inline Vec3f operator-(const Vec3f &other) const {
		return Vec3f(_mm_sub_ps(_xmm, other._xmm));
	}

	inline void operator+=(const Vec3f &other) {
		_xmm = _mm_add_ps(_xmm, other._xmm);
	}
	inline void operator-=(const Vec3f &other) {
		_xmm = _mm_sub_ps(_xmm, other._xmm);
	}

	// Mul/Div
	inline friend Vec3f operator*(const Vec3f &vec, float scalar) {
		return Vec3f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec3f operator*(float scalar, const Vec3f &vec) {
		return Vec3f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}

	inline friend Vec3f operator/(const Vec3f &vec, float scalar) {
		return Vec3f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec3f operator/(float scalar, const Vec3f &vec) {
		return Vec3f(_mm_div_ps(_mm_set1_ps(scalar), vec._xmm));
	}

	inline friend Vec3f operator*(const Vec3f &a, const Vec3f &b) {
		return Vec3f(_mm_mul_ps(a._xmm, b._xmm));
	}
	inline friend Vec3f operator/(const Vec3f &a, const Vec3f &b) {
		return Vec3f(_mm_div_ps(a._xmm, b._xmm));
	}

	// Comp
	inline bool operator==(const Vec3f &other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}
	inline bool operator!=(const Vec3f &other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z);
	}

	// Index
	inline const float& operator[](int index) const {
		return *((&x) + index);
	}
	inline float& operator[](int index) {
		return *((&x) + index);
	}

	/* ---- METHODS ---- */
	inline float dot(const Vec3f &other) const {
		return _mm_dp_ps(_xmm, other._xmm, 0b0111'1111).m128_f32[0]; // TODO may not be supported on every CPU
	}
	inline Vec3f cross(const Vec3f &other) const {
		return {other.y * this->z - other.z * this->y, other.z * this->x - other.x * this->z, other.x * this->y - other.y * this->x};
	}
	inline Vec3f normalized() const {
		return (*this) * invsqrt(this->dot(*this));
	}
	inline Vec3f normalized_precise() const {
		float mag = this->magnitude();
		return {this->x / mag, this->y / mag, this->z / mag};
	}
	inline float magnitude() const {
		return sqrtfInline(_mm_dp_ps(_xmm, _xmm, 0b0111'1111).m128_f32[0]); // TODO may not be supported on every CPU
	}
	inline Vec3f abs() const {
		return Vec3f(_mm_andnot_ps(_mm_mask_signf, _xmm));
	}
	inline Vec3f sign() const {
		return Vec3f(_mm_or_ps(_mm_and_ps(_mm_mask_signf, _xmm), _mm_one));
	}
	inline Vec3f step(const Vec3f &edge) const {
		return Vec3f(_mm_or_ps(_mm_and_ps(_mm_sub_ps(edge._xmm, _xmm), _mm_mask_signf), _mm_one));
	}
	inline static Vec3f lerp(const Vec3f &a, const Vec3f &b, float t) {
		return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)};
	}
	inline Vec3f inverse() const {
		return {1.0f / x, 1.0f / y, 1.0f / z};
	}
	inline Vec3i floor() const {
		return {floorToInt(x), floorToInt(y), floorToInt(z)};
	}
};

/* ==== VEC4f ==== */
struct Vec4f {
	union {
		struct {
			float x, y, z, w;
		};
		float data[4];
		__m128 _xmm;
	};
public:
	/* ---- CONSTANTS ---- */
	static const Vec4f zero;
	static const Vec4f zeroPoint;
	static const Vec4f one;
	static const Vec4f oneDir;
	static const Vec4f right;
	static const Vec4f left;
	static const Vec4f up;
	static const Vec4f down;
	static const Vec4f forward;
	static const Vec4f backward;

	/* ---- CONSTRUCTORS ---- */
	inline Vec4f() : x(0), y(0), z(0), w(0) { }
	inline Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	inline Vec4f(__m128 xmm) : _xmm(xmm) { }

	/* ---- OPERATOR ---- */
	// Misc
	inline Vec4f operator-() const {
		return {-this->x, -this->y, -this->z, -this->w};
	}

	// Add/Sub
	inline Vec4f operator+(const Vec4f &other) const {
		return {this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w};
	}
	inline Vec4f operator-(const Vec4f &other) const {
		return {this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w};
	}

	inline void operator+=(const Vec4f &other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
	}
	inline void operator-=(const Vec4f &other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
	}

	// Mul/Div
	inline friend Vec4f operator*(const Vec4f &vec, float scalar) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator*(float scalar, const Vec4f &vec) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}

	inline friend Vec4f operator/(const Vec4f &vec, float scalar) {
		return Vec4f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator/(float scalar, const Vec4f &vec) {
		return Vec4f(_mm_div_ps(_mm_set1_ps(scalar), vec._xmm));
	}

	inline friend Vec4f operator*(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_mul_ps(lhs._xmm, rhs._xmm));
	}
	inline friend Vec4f operator/(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_div_ps(lhs._xmm, rhs._xmm));
	}

	// Comp
	inline bool operator==(const Vec4f &other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
	}
	inline bool operator!=(const Vec4f &other) const {
		return (this->x != other.x) || (this->y != other.y) || (this->z != other.z) || (this->w != other.w);
	}
	
	// Index
	inline const float& operator[](int index) const {
		return *((&x) + index);
	}
	inline float& operator[](int index) {
		return *((&x) + index);
	}
};

/* ==== BIVEC3f ==== */
struct Bivec3f {
	float xy, xz, yz;

public:
	inline Bivec3f() : xy(0), xz(0), yz(0) {}
	inline Bivec3f(float xy, float xz, float yz) : xy(xy), xz(xz), yz(yz) {}

	inline static Bivec3f wedge(const Vec3f& u, const Vec3f& v) {
		return {
			u.x * v.y - u.y * v.x,	// XY
			u.x * v.z - u.z * v.x,	// XZ
			u.y * v.z - u.z * v.y	// YZ
		};
	}
};

/* ==== ROTOR3f ==== */
struct Rotor3f {
	float a;
	union {
		Bivec3f bv;
		struct {
			float xy, xz, yz;
		};
	};

public:
	/* ---- CONSTANTS ---- */
	static const Rotor3f identity;

	/* ---- CONSTRUCTORS ---- */
	inline Rotor3f() : a(1), bv() {}
	inline Rotor3f(float a, float xy, float xz, float yz) : a(a), xy(xy), xz(xz), yz(yz) {}
	inline Rotor3f(float a, const Bivec3f& bv) : a(a), bv(bv) {}
	inline Rotor3f(const Vec3f& from, const Vec3f& to) : a(1 + to.dot(from)), bv(Bivec3f::wedge(to, from)) {
		normalize();
	}

	/* ---- OPERATORS ---- */
	inline Rotor3f operator*(const Rotor3f& r) const {
		const Rotor3f& p = *this;
		return {
			p.a * r.a - p.xy * r.xy - p.xz * r.xz - p.yz * r.yz,
			{
				p.xy * r.a + p.a * r.xy + p.yz * r.xz - p.xz * r.yz,
				p.xz * r.a + p.a * r.xz - p.yz * r.xy + p.xy * r.yz,
				p.yz * r.a + p.a * r.yz + p.xz * r.xy - p.xy * r.xz
			}
		};
	}
	inline friend void operator*=(Rotor3f& a, const Rotor3f& b) {
		a = a * b;
	}

	/* ----- METHODS ---- */
	inline static Rotor3f AnglePlane(float angleRad, const Bivec3f& bvPlane) {
		float sina = sinf(angleRad / 2.f);
		return {
			cosf(angleRad / 2.f),
			{
				Bivec3f(-sina * bvPlane.xy, -sina * bvPlane.xz, -sina * bvPlane.yz)
			}
		};
	}
	inline Vec3f rotate(const Vec3f& v) const {
		const Rotor3f& p = *this;

		Vec3f q;
		q.x = p.a * v.x + v.y * p.xy + v.z * p.xz;
		q.y = p.a * v.y - v.x * p.xy + v.z * p.yz;
		q.z = p.a * v.z - v.x * p.xz - v.y * p.yz;

		float q012 = -v.x * p.yz + v.y * p.xz - v.z * p.xy;

		return  {
			p.a * q.x + q.y * p.xy + q.z * p.xz - q012 * p.yz,
			p.a * q.y - q.x * p.xy + q012 * p.xz + q.z * p.yz,
			p.a * q.z - q012 * p.xy - q.x * p.xz - q.y * p.yz
		};
	}
	inline Rotor3f rotate(const Rotor3f& r) const {
		return (*this) * r * reverse();
	}
	inline Rotor3f reverse() const {
		return Rotor3f(a, -xy, -xz, -yz);
	}
	inline float magnitudeSqr() const {
		return a * a + xy * xy + xz * xz + yz * yz;
	}
	inline float magnitude() const {
		return sqrtfInline(magnitudeSqr());
	}
	inline void normalize() {
		float isqrt = invsqrt(magnitudeSqr());
		a *= isqrt;
		xy *= isqrt;
		xz *= isqrt;
		yz *= isqrt;
	}
	inline Rotor3f normalized() const {
		float isqrt = invsqrt(magnitudeSqr());
		return {
			a * isqrt,
			{
				xy * isqrt,
				xz * isqrt,
				yz * isqrt
			}
		};
	}
};

/* ==== QUATERNION ==== */
struct Quaternion {
	float x, y, z, w;
public:
	/* ---- CONSTANTS ---- */
	static const Quaternion identity;

	/* ---- CONSTRUCTORS ---- */
	inline Quaternion() : x(0), y(0), z(0), w(1) { }
	inline Quaternion(const float &x, const float &y, const float &z, const float &w) : x(x), y(y), z(z), w(w) {}

	/* ---- OPERATORS ---- */
	// Mul
	inline Quaternion operator*(const Quaternion& b) {
		return {
			b.w * this->x + b.x * this->w - b.y * this->z + b.z * this->y,
			b.w * this->y + b.x * this->z + b.y * this->w - b.z * this->x,
			b.w * this->z - b.x * this->y + b.y * this->x + b.z * this->w,
			b.w * this->w - b.x * this->x - b.y * this->y - b.z * this->z
		};
	}
	inline friend void operator*=(Quaternion& a, const Quaternion& b) {
		a = a * b;
	}

	/* ---- METHODS ---- */
	inline static Quaternion euler(const Vec3f& eulerRot) {
		return headingAttitudeBank(eulerRot.y, eulerRot.z, eulerRot.x);
	}
	inline static Quaternion attitude(const float& pitch, const float& roll, const float& yaw) {
		return headingAttitudeBank(yaw, roll, pitch);
	}
	inline static Quaternion headingAttitudeBank(const float& heading, const float& attitude, const float& bank) {
		float c1 = cosf(heading * 0.5f);
		float s1 = sinf(heading * 0.5f);
		float c2 = cosf(attitude * 0.5f);
		float s2 = sinf(attitude * 0.5f);
		float c3 = cosf(bank * 0.5f);
		float s3 = sinf(bank * 0.5f);
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;

		return {c1c2 * s3 + s1s2 * c3, s1 * c2 * c3 + c1 * s2 * s3, c1 * s2 * c3 - s1 * c2 * s3, c1c2 * c3 - s1s2 * s3};
	}
};

#pragma endregion

#pragma region Matrices
/* ===== MATRIX4x4f ==== */
struct Matrix4x4f {
	union {
		// Stored in column major order but accessed in row major
		struct {
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
		};
		float data[16];
		__m128 _xmm[4];
	};
public:
	/* ---- CONSTANTS ---- */
	static const Matrix4x4f identity;

	/* ---- OPERATORS ---- */
	inline static void transfColumn(const Matrix4x4f *mat, const __m128 &column, __m128 &res) {
		res = _mm_mul_ps(mat->_xmm[0], _mm_set1_ps(column.m128_f32[0]));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[1], _mm_set1_ps(column.m128_f32[1])));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[2], _mm_set1_ps(column.m128_f32[2])));
		res = _mm_add_ps(res, _mm_mul_ps(mat->_xmm[3], _mm_set1_ps(column.m128_f32[3])));
	}
	inline Matrix4x4f operator*(const Matrix4x4f &other) const {
		Matrix4x4f m;
		transfColumn(this, other._xmm[0], m._xmm[0]);
		transfColumn(this, other._xmm[1], m._xmm[1]);
		transfColumn(this, other._xmm[2], m._xmm[2]);
		transfColumn(this, other._xmm[3], m._xmm[3]);
		return m;
	}
	inline Vec4f operator*(const Vec4f &other) const {
		Vec4f v;
		transfColumn(this, other._xmm, v._xmm);
		return v;
	}
	inline Matrix4x4f operator*(float scalar) const {
		Matrix4x4f m;
		__m128 scalar_xmm = _mm_set1_ps(scalar);
		transfColumn(this, scalar_xmm, m._xmm[0]);
		transfColumn(this, scalar_xmm, m._xmm[1]);
		transfColumn(this, scalar_xmm, m._xmm[2]);
		transfColumn(this, scalar_xmm, m._xmm[3]);
		return m;
	}

	/* ---- METHODS ---- */
	void luDecomposition(Matrix4x4f &lu);
	static Vec4f solve(Matrix4x4f &lu, Vec4f toSolve);

	// Access
	inline float get(Vec2i pos) {
		return data[pos.y + pos.x * 4];
	}
	inline float set(Vec2i pos, float val) {
		data[pos.y + pos.x * 4] = val;
	}
	inline float& at(int row, int col) {
		return data[row + col * 4];
	}

	// Optimized Operations
	inline Matrix4x4f inverseAffine() const {
		float a = m11 * m22 - m12 * m21;
		float b = m12 * m20 - m10 * m22;
		float c = m10 * m21 - m11 * m20;
		float det = m00 * a + m01 * b + m02 * c;
		float detInv = 1.0f / det;
		float inv3[3][3] = {
			{a * detInv, (m02 * m21 - m01 * m22) * detInv, (m01 * m12 - m02 * m11) * detInv},
			{b * detInv, (m00 * m22 - m02 * m20) * detInv, (m02 * m10 - m00 * m12) * detInv},
			{c * detInv, (m01 * m20 - m00 * m21) * detInv, (m00 * m11 - m01 * m10) * detInv}
		};
		return {{
			inv3[0][0], inv3[1][0], inv3[2][0], 0,
			inv3[0][1], inv3[1][1], inv3[2][1], 0,
			inv3[0][2], inv3[1][2], inv3[2][2], 0,
			-inv3[0][0] * m03 + -inv3[0][1] * m13 + -inv3[0][2] * m23, -inv3[1][0] * m03 + -inv3[1][1] * m13 + -inv3[1][2] * m23, -inv3[2][0] * m03 + -inv3[2][1] * m13 + -inv3[2][2] * m23, 1
		}};
	}
	inline Vec3f multPoint(Vec3f point) const {
		Vec3f v;
		v._xmm = _mm_mul_ps(_xmm[0], _mm_set1_ps(point.x));
		v._xmm = _mm_add_ps(v._xmm, _mm_mul_ps(_xmm[1], _mm_set1_ps(point.y)));
		v._xmm = _mm_add_ps(v._xmm, _mm_mul_ps(_xmm[2], _mm_set1_ps(point.z)));
		v._xmm = _mm_add_ps(v._xmm, _xmm[3]);
		v._xmm.m128_f32[3] = 1.0f; // Safety messure
		return v;
	}
	inline Vec3f multDirection(Vec3f dir) const {
		Vec3f v;
		v._xmm = _mm_mul_ps(_xmm[0], _mm_set1_ps(dir.x));
		v._xmm = _mm_add_ps(v._xmm, _mm_mul_ps(_xmm[1], _mm_set1_ps(dir.y)));
		v._xmm = _mm_add_ps(v._xmm, _mm_mul_ps(_xmm[2], _mm_set1_ps(dir.z)));
		v._xmm.m128_f32[3] = 0.0f; // Safety messure
		return v;
	}

	// Transformations
	static Matrix4x4f translation(Vec3f transVec) {
		return {{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			transVec.x, transVec.y, transVec.z, 1
		}};
	}
	static Matrix4x4f scale(Vec3f scaleVec) {
		return {{{
			scaleVec.x, 0, 0, 0,
			0, scaleVec.y, 0, 0,
			0, 0, scaleVec.z, 0,
			0, 0, 0, 1
		}}};
	}
	static Matrix4x4f translationScale(Vec3f transVec, Vec3f scaleVec) {
		return {{
			scaleVec.x, 0, 0, 0,
			0, scaleVec.y, 0, 0,
			0, 0, scaleVec.z, 0,
			transVec.x, transVec.y, transVec.z, 1
		}};
	}
	static Matrix4x4f rotation(Rotor3f rot) {
		// TODO optimize
		Vec3f vx = rot.rotate(Vec3f(1, 0, 0));
		Vec3f vy = rot.rotate(Vec3f(0, 1, 0));
		Vec3f vz = rot.rotate(Vec3f(0, 0, 1));
		return {{
			vx.x, vy.x, vz.x,	0,
			vx.y, vy.y, vz.y,	0,
			vx.z, vy.z, vz.z,	0,
			0,		0,		0,	1
		}};
	}
	static Matrix4x4f rotation(Quaternion rot) {
		return {{
			1.0f - 2.0f * rot.y * rot.y - 2.0f * rot.z * rot.z,		2.0f * rot.x * rot.y + 2.0f * rot.z * rot.w,		2.0f * rot.x * rot.z - 2.0f * rot.y * rot.w,		0,
			2.0f * rot.x * rot.y - 2.0f * rot.z * rot.w,			1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z, 2.0f * rot.y * rot.z + 2.0f * rot.x * rot.w,		0,
			2.0f * rot.x * rot.z + 2.0f * rot.y * rot.w,			2.0f * rot.y * rot.z - 2.0f * rot.x * rot.w,		1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y, 0,
			0,														0,													0,													1
		}};
	}
	static Matrix4x4f rotation(Vec3f euler) {
		float cx = cosf(euler.x);
		float sx = sinf(euler.x);
		float cy = cosf(euler.y);
		float sy = sinf(euler.y);
		float cz = cosf(euler.z);
		float sz = sinf(euler.z);
		return {{
			cx * cz - sx * sy * sz,		cz * sx + cx * sy * sz,		-(cy * sz),		0,
			-(cy * sx),					cx * cy,					sy,				0,
			cx * sz + cz * sx * sy,		sx * sz - cx * cz * sy,		cy * cz,		0,
			0,							0,							0,				1
		}};
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Rotor3f rotor) {
		return translationScale(translationVec, scaleVec) * rotation(rotor);
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Quaternion rotationQuat) {
		return translationScale(translationVec, scaleVec) * rotation(rotationQuat);
	}
	static Matrix4x4f transformation(Vec3f translationVec, Vec3f scaleVec, Vec3f rotationVec) {
		return translationScale(translationVec, scaleVec) * rotation(rotationVec);
	}
	static Matrix4x4f perspectiveProjection(float fov, float aspect, float nearPlane, float farPlane) {
		float zRange = farPlane - nearPlane;

		float xScale = 1.0f / tanf(fov * 0.5f);
		float yScale = xScale * aspect;

		return {{
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, (farPlane + nearPlane) / zRange, 1,
			0, 0, -(2.0f * farPlane * nearPlane) / zRange, 0
		}};
	}
};
#pragma endregion

#pragma region OtherStructures
struct Box {
	Vec3f center, extents;

public:
	inline static Box fromMinMax(Vec3f min, Vec3f max) {
		return {{(min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f}, {(max.x - min.x) / 2.0f, (max.y - min.y) / 2.0f, (max.z - min.z) / 2.0f}};
	}
};

struct Ray {
	Vec3f origin, direction;

public:
	Ray() : origin(Vec3f(0, 0, 0)), direction(Vec3f(0, 1, 0)) {}
	Ray(Vec3f origin, Vec3f dir) : origin(origin), direction(dir) {}
};

struct Rect {
	Vec2f min, max;
};

struct Recti {
	Vec2i min, max;
};

struct Plane {
	Vec4f normalDistance;

public:
	Plane() : normalDistance(0.0f, 1.0f, 0.0f, 0.0f) {}
	Plane(Vec3f normal, float distance) : normalDistance(Vec4f(normal.x, normal.y, normal.z, distance)) {}
	Plane(Vec4f normalDistance) : normalDistance(normalDistance) {}

	inline bool intersectRay(const Ray& ray, Vec3f& intersect, float& t) const {
		float denom = normalDistance.x * ray.direction.x + normalDistance.y * ray.direction.y + normalDistance.z * ray.direction.z;
		t = (-normalDistance.x * ray.origin.x - normalDistance.y * ray.origin.y - normalDistance.z * ray.origin.z - normalDistance.w) / denom;
		if (t <= 0) {
			return false;
		}
		intersect = ray.origin + ray.direction * t;
		return true;
	}
};
#pragma endregion

#pragma region Structures Math Functions
// NDC Raster functions
inline float edge2D(const Vec4f &a, const Vec4f &b, const Vec4f &c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

inline bool triangleBarycentric2D(const Vec4f &v0, const Vec4f &v1, const Vec4f &v2, const Vec4f &p, Vec3f &bary) {
	float area = edge2D(v0, v1, v2);
	bary.x = edge2D(v1, v2, p) / area;
	bary.y = edge2D(v2, v0, p) / area;
	bary.z = edge2D(v0, v1, p) / area;
	return bary.x >= 0 && bary.y >= 0 && bary.z >= 0;
}

inline bool triangleBarycentric(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Vec3f& p, Vec3f& bary) {
	float divider = (v2.z - v1.z) * (v0.x - v1.x) + (v1.x - v2.x) * (v0.z - v1.z);
	if (divider == 0) return false;
	float weightV0 = ((v2.z - v1.z) * (p.x - v1.x) + (v1.x - v2.x) * (p.y - v1.z)) / divider;
	float weightV2 = ((v1.z - v0.z) * (p.x - v1.x) + (v0.x - v1.x) * (p.y - v1.z)) / divider;
	float weightV1 = 1.0f - weightV0 - weightV2;
	bary = Vec3f(weightV0, weightV1, weightV2);
	return weightV0 >= 0 && weightV1 >= 0 && weightV2 >= 0;
}

inline bool intersectTriangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Ray& ray, Vec3f& intersect, float& t) {
	Vec3f v01 = v1 - v0;
	Vec3f v02 = v2 - v0;
	Vec3f normal = v01.cross(v02).normalized();
	Plane triPlane = Plane(normal, -normal.x * v0.x - normal.y * v0.y - normal.z * v0.z);
	if (!triPlane.intersectRay(ray, intersect, t)) {
		return false;
	}
	Vec3f bary;
	return triangleBarycentric(v0, v1, v2, intersect, bary);
}

inline bool intersectQuad(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Ray& ray, Vec3f& intersect, float& t) {
	Vec3f v01 = v1 - v0;
	Vec3f v12 = v2 - v1;
	Vec3f v23 = v3 - v2;
	Vec3f v30 = v0 - v3;
	Vec3f normal = v01.cross(v2 - v0).normalized();
	if (normal.dot(ray.direction) < 0) return false;
	Plane triPlane = Plane(normal, -normal.x * v0.x - normal.y * v0.y - normal.z * v0.z);
	if (!triPlane.intersectRay(ray, intersect, t)) {
		return false;
	}
	Vec3f v0i = intersect - v0;
	Vec3f v1i = intersect - v1;
	Vec3f v2i = intersect - v2;
	Vec3f v3i = intersect - v3;
	Vec3f c0 = v01.cross(v0i);
	Vec3f c1 = v12.cross(v1i);
	Vec3f c2 = v23.cross(v2i);
	Vec3f c3 = v30.cross(v3i);
	return c0.dot(c1) > 0 && c1.dot(c2) > 0 && c2.dot(c3) > 0;
}

inline void swapToMinMax(Vec3i& min, Vec3i& max) {
	float tmp;
	if (min.x > max.x) {
		tmp = min.x;
		min.x = max.x;
		max.x = tmp;
	}
	if (min.y > max.y) {
		tmp = min.y;
		min.y = max.y;
		max.y = tmp;
	}
	if (min.z > max.z) {
		tmp = min.z;
		min.z = max.z;
		max.z = tmp;
	}
}
#pragma endregion