#include "XMath.h"

/* ==== CONSTANTS ==== */
// Vec2i
const Vec2i Vec2i::zero = {0, 0};
const Vec2i Vec2i::one = {1, 1};
const Vec2i Vec2i::right = {1, 0};
const Vec2i Vec2i::left = {-1, 0};
const Vec2i Vec2i::up = {0, 1};
const Vec2i Vec2i::down = {0, -1};
// Vec3i
const Vec3i Vec3i::zero = {0, 0, 0};
const Vec3i Vec3i::one = {1, 1, 1};
const Vec3i Vec3i::right = {1, 0, 0};
const Vec3i Vec3i::left = {-1, 0, 0};
const Vec3i Vec3i::up = {0, 1, 0};
const Vec3i Vec3i::down = {0, -1, 0};
const Vec3i Vec3i::forward = {0, 0, 1};
const Vec3i Vec3i::backward = {0, 0, -1};
// Vec2f
const Vec2f Vec2f::zero = {0, 0};
const Vec2f Vec2f::one = {1, 1};
const Vec2f Vec2f::right = {1, 0};
const Vec2f Vec2f::left = {-1, 0};
const Vec2f Vec2f::up = {0, 1};
const Vec2f Vec2f::down = {0, -1};
// Vec3f
const Vec3f Vec3f::zero = {0, 0, 0};
const Vec3f Vec3f::one = {1, 1, 1};
const Vec3f Vec3f::right = {1, 0, 0};
const Vec3f Vec3f::left = {-1, 0, 0};
const Vec3f Vec3f::up = {0, 1, 0};
const Vec3f Vec3f::down = {0, -1, 0};
const Vec3f Vec3f::forward = {0, 0, 1};
const Vec3f Vec3f::backward = {0, 0, -1};
// Vec4f
const Vec4f Vec4f::zero = {0, 0, 0, 0};
const Vec4f Vec4f::zeroPoint = {0, 0, 0, 1};
const Vec4f Vec4f::one = {1, 1, 1, 1};
const Vec4f Vec4f::oneDir = {1, 1, 1, 0};
const Vec4f Vec4f::right = {1, 0, 0, 0};
const Vec4f Vec4f::left = {-1, 0, 0, 0};
const Vec4f Vec4f::up = {0, 1, 0, 0};
const Vec4f Vec4f::down = {0, -1, 0, 0};
const Vec4f Vec4f::forward = {0, 0, 1, 0};
const Vec4f Vec4f::backward = {0, 0, -1, 0};
// Rotor3f
const Rotor3f Rotor3f::identity = {1, {0, 0, 0}};
// Matrix4x4f
const Matrix4x4f Matrix4x4f::identity = {{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
}};
// Quaternion
const Quaternion Quaternion::identity = {0.0f, 0.0f, 0.0f, 1.0f};


/* ==== METHODS ==== */
// Matrix4x4f
void Matrix4x4f::luDecomposition(Matrix4x4f &lu) {

	float sum = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, j);
			}
			lu.at(i, j) = at(i, j) - sum;
		}
		for (int j = i + 1; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < i; k++) {
				sum += lu.at(i, k) * lu.at(k, i);
			}
			lu.at(j, i) = (1.0f / lu.at(i, i)) * (at(j, i) - sum);
		}
	}

}

Vec4f Matrix4x4f::solve(Matrix4x4f &lu, Vec4f toSolve) {

	float sum = 0;
	// find solution of Ly = b
	Vec4f y = {0, 0, 0, 0};
	for (int i = 0; i < 4; i++) {
		sum = 0;
		for (int k = 0; k < i; k++) {
			sum += lu.at(i, k) * y[k];
		}
		y[i] = toSolve[i] - sum;
	}
	// find solution of Ux = y
	Vec4f x = {0, 0, 0, 0};
	for (int i = 3; i >= 0; i--) {
		sum = 0;
		for (int k = i + 1; k < 4; k++) {
			sum += lu.at(i, k) * x[k];
		}
		x[i] = (1.0f / lu.at(i, i)) * (y[i] - sum);
	}

	return x;
}
