#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
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

inline float lerpf(const float &a, const float &b, const float &t) {
	return (1 - t) * a + t * b;
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
	inline Vec2i(const int &x, const int &y) : x(x), y(y) { }

	/* ---- OPERATORS ---- */
	// Add/Sub
	inline Vec2i operator+(const Vec2i &other) const {
		return {this->x + other.x, this->y + other.y};
	}
	inline Vec2i  operator-(const Vec2i &other) const {
		return {this->x - other.x, this->y - other.y};
	}
	inline void operator+=(Vec2i other) {
		this->x += other.x;
		this->y += other.y;
	}
	inline void operator-=(Vec2i other) {
		this->x -= other.x;
		this->y -= other.y;
	}

	// Mul/Div
	inline friend Vec2i operator*(const Vec2i &vec, const int &mult) {
		return {vec.x * mult, vec.y * mult};
	}
	inline friend Vec2i operator*(const int &mult, const Vec2i &vec) {
		return {vec.x * mult, vec.y * mult};
	}

	inline friend Vec2i operator/(const Vec2i &vec, const int &mult) {
		return {vec.x / mult, vec.y / mult};
	}
	inline friend Vec2i operator/(const int &mult, const Vec2i &vec) {
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
		return !(*this == other);
	}

	// Index
	inline const int& operator[](const int &index) const {
		return *((&x) + index);
	}
	inline int& operator[](const int &index) {
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
	inline Vec3i(const int &x, const int &y, const int &z) : x(x), y(y), z(z) {}
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
	inline friend Vec3i operator*(const Vec3i &vec, const int &mult) {
		return Vec3i(_mm_mul_epi32(vec._emm, _mm_set1_epi32(mult)));
	}
	inline friend Vec3i operator*(const int &mult, const Vec3i &vec) {
		return Vec3i(_mm_mul_epi32(vec._emm, _mm_set1_epi32(mult)));
	}

	inline friend Vec3i operator/(const Vec3i &vec, const int &mult) {
		return {vec.x / mult, vec.y / mult, vec.z / mult};
	}
	inline friend Vec3i operator/(const int &mult, const Vec3i &vec) {
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
		return (this->x == other.x) && (this->y == other.y);
	}
	inline bool operator!=(const Vec3i &other) const {
		return !(*this == other);
	}

	// Index
	inline const int& operator[](const int &index) const {
		return *((&x) + index);
	}
	inline int& operator[](const int &index) {
		return *((&x) + index);
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
	inline Vec2f(const float &x, const float &y) : x(x), y(y) { }
	
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

	inline void operator+=(Vec2f *other) {
		this->x += other->x;
		this->y += other->y;
	}
	inline void operator-=(Vec2f *other) {
		this->x -= other->x;
		this->y -= other->y;
	}

	// Mul/Div
	inline friend Vec2f operator*(const Vec2f &vec, const float &scalar) {
		return {vec.x * scalar, vec.y * scalar};
	}
	inline friend Vec2f operator*(const float &scalar, const Vec2f &vec) {
		return {vec.x * scalar, vec.y * scalar};
	}

	inline friend Vec2f operator/(const Vec2f &vec, const float &scalar) {
		return {vec.x / scalar, vec.y / scalar};
	}
	inline friend Vec2f operator/(const float &scalar, const Vec2f &vec) {
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
		return !(*this == other);
	}

	// Index
	inline const float& operator[](const int &index) const {
		return *((&x) + index);
	}
	inline float& operator[](const int &index) {
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
	inline Vec3f(const float &x, const float &y, const float &z) : x(x), y(y), z(z) { }
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

	inline void operator+=(Vec3f *other) {
		_xmm = _mm_add_ps(_xmm, other->_xmm);
	}
	inline void operator-=(Vec3f *other) {
		_xmm = _mm_sub_ps(_xmm, other->_xmm);
	}

	// Mul/Div
	inline friend Vec3f operator*(const Vec3f &vec, const float &scalar) {
		return Vec3f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec3f operator*(const float &scalar, const Vec3f &vec) {
		return Vec3f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}

	inline friend Vec3f operator/(const Vec3f &vec, const float &scalar) {
		return Vec3f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec3f operator/(const float &scalar, const Vec3f &vec) {
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
		return !(*this == other);
	}

	// Index
	inline const float& operator[](const int &index) const {
		return *((&x) + index);
	}
	inline float& operator[](const int &index) {
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
	inline static Vec3f lerp(const Vec3f &a, const Vec3f &b, const float &t) {
		return {lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)};
	}
	inline Vec3f inverse() const {
		return {1.0f / x, 1.0f / y, 1.0f / z};
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

	inline void operator+=(Vec4f *other) {
		this->x += other->x;
		this->y += other->y;
		this->z += other->z;
		this->w += other->w;
	}
	inline void operator-=(Vec4f *other) {
		this->x -= other->x;
		this->y -= other->y;
		this->z -= other->z;
		this->w -= other->w;
	}

	// Mul/Div
	inline friend Vec4f operator*(const Vec4f &vec, const float &scalar) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator*(const float &scalar, const Vec4f &vec) {
		return Vec4f(_mm_mul_ps(vec._xmm, _mm_set1_ps(scalar)));
	}

	inline friend Vec4f operator/(const Vec4f &vec, float &scalar) {
		return Vec4f(_mm_div_ps(vec._xmm, _mm_set1_ps(scalar)));
	}
	inline friend Vec4f operator/(float &scalar, const Vec4f &vec) {
		return Vec4f(_mm_div_ps(_mm_set1_ps(scalar), vec._xmm));
	}

	inline friend Vec4f operator*(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_mul_ps(lhs._xmm, rhs._xmm));
	}
	inline friend Vec4f operator/(const Vec4f &lhs, const Vec4f &rhs) {
		return Vec4f(_mm_div_ps(lhs._xmm, rhs._xmm));
	}

	// Comp
	inline bool operator==(const Vec4f& other) const {
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
	}
	inline bool operator!=(const Vec4f& other) const {
		return !(*this == other);
	}
	
	// Index
	inline const float& operator[](const int &index) const {
		return *((&x) + index);
	}
	inline float& operator[](const int &index) {
		return *((&x) + index);
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
	inline Quaternion operator*(Quaternion b) {
		return {
			b.w * this->x + b.x * this->w - b.y * this->z + b.z * this->y,
			b.w * this->y + b.x * this->z + b.y * this->w - b.z * this->x,
			b.w * this->z - b.x * this->y + b.y * this->x + b.z * this->w,
			b.w * this->w - b.x * this->x - b.y * this->y - b.z * this->z
		};
	}

	/* ---- METHODS ---- */
	static Quaternion euler(Vec3f eulerRot) {
		return headingAttitudeBank(eulerRot.y, eulerRot.z, eulerRot.x);
	}
	static Quaternion attitude(float pitch, float roll, float yaw) {
		return headingAttitudeBank(yaw, roll, pitch);
	}
	static Quaternion headingAttitudeBank(float heading, float attitude, float bank) {
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
};

struct Rect {
	Vec2f min, max;
};

struct Recti {
	Vec2i min, max;
};
#pragma endregion

#pragma region Structures Math Functions
// NDC Raster functions
inline float edge(const Vec4f &a, const Vec4f &b, const Vec4f &c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

inline bool triangleBarycentric(const Vec4f &v0, const Vec4f &v1, const Vec4f &v2, const Vec4f &p, Vec3f &bary) {
	float area = edge(v0, v1, v2); // TODO compute it once only for each triangle to avoid recomputing for each pixel
	bary.x = edge(v1, v2, p) / area;
	bary.y = edge(v2, v0, p) / area;
	bary.z = edge(v0, v1, p) / area;
	return bary.x >= 0 && bary.y >= 0 && bary.z >= 0;
}
#pragma endregion