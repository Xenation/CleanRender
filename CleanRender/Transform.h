#pragma once
#include "Component.h"
#include "Math.h"
class Transform : public Component {
public:
	Transform(Entity* entity);
	~Transform();

	inline Vec3f getPosition() { return position; }
	inline Vec3f getScale() { return scale; }
	inline Quaternion getRotation() { return rotation; }

	inline void setPosition(Vec3f pos) { position = pos; invalidateMatrices(); }
	inline void setScale(Vec3f sca) { scale = sca; invalidateMatrices(); }
	inline void setRotation(Quaternion rot) { rotation = rot; invalidateMatrices(); }

	Matrix4x4f getLocalToWorldMatrix();
	Matrix4x4f getWorldToLocalMatrix();

	void setParent(Transform* parent);
	inline Transform* getParent() { return parent; }
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
	void notifyChildrenLocalToWorldChange();

	Transform* parent = nullptr;
	unsigned int childIndex = 0;
	Transform** children = nullptr;
	unsigned int childrenCount = 0;
	unsigned int childrenCapacity = 0;
	void addChild(Transform* child);
	void removeChild(unsigned int index);
	void initChildArray();
	void growChildArray();
};

