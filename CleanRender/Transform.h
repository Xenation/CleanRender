#pragma once
#include "Component.h"
#include "Math.h"
#include "SimpleList.h"
class Transform : public Component {
public:
	Transform(Entity* entity);
	~Transform();

	inline Vec3f getPosition() { return position; }
	inline Vec3f getScale() { return scale; }
	inline Quaternion getRotation() { return rotation; }

	inline void setPosition(Vec3f pos) { position = pos; invalidateMatrices(); }
	inline void translate(Vec3f translation) { position += translation; invalidateMatrices(); }
	inline void setScale(Vec3f sca) { scale = sca; invalidateMatrices(); }
	inline void setRotation(Quaternion rot) { rotation = rot; invalidateMatrices(); }
	inline void rotate(Quaternion rot) { rotation *= rot; invalidateMatrices(); }

	Matrix4x4f getLocalToWorldMatrix();
	Matrix4x4f getWorldToLocalMatrix();

	Vec4f localToWorld(Vec4f vec);
	Vec3f localToWorldPos(Vec3f pos);
	Vec3f localToWorldDir(Vec3f dir);
	Vec4f worldToLocal(Vec4f vec);
	Vec3f worldToLocalPos(Vec3f pos);
	Vec3f worldToLocalDir(Vec3f dir);

	inline Vec3f forward() { return localToWorldDir(Vec3f::forward).normalized(); }
	inline Vec3f backward() { return localToWorldDir(Vec3f::backward).normalized(); }
	inline Vec3f right() { return localToWorldDir(Vec3f::right).normalized(); }
	inline Vec3f left() { return localToWorldDir(Vec3f::left).normalized(); }
	inline Vec3f up() { return localToWorldDir(Vec3f::up).normalized(); }
	inline Vec3f down() { return localToWorldDir(Vec3f::down).normalized(); }

	void parentChanged();

private:
	Vec3f position = Vec3f::zero;
	Vec3f scale = Vec3f::one;
	Quaternion rotation = Quaternion::identity;
	Matrix4x4f modelMatrix;
	bool modelMatrixExpired = true;
	Matrix4x4f localToWorldMatrix;
	bool localToWorldMatrixExpired = true;
	Matrix4x4f worldToLocalMatrix;
	bool worldToLocalMatrixExpired = true;

	void invalidateMatrices();
	void recalculateModelMatrix();
	void recalculateLTWMatrix();
	void recalculateWTLMatrix();
};

