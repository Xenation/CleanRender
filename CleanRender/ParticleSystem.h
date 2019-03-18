#pragma once
#include "Renderer.h"
#include "XTypes.h"
#include "XMath.h"

class Entity;

class ParticleSystem : public Renderer {
public:
	struct Particle {
		bool alive;
		Vec3f position;
		Vec3f velocity;
		float creationTime;
	};

	Particle* particles = nullptr;

	float emitRate = 5;
	float lifetime = 5;

	ParticleSystem(Entity* entity);
	~ParticleSystem();

	void startEmit();
	virtual void onUpdate() override;
	void stopEmit();

	virtual void setMaterial(Material* mat) override;
	virtual void render() override;

private:
	bool isEmitting = false;
	uint maxParticles = 0;
	float emissionInterval = 0;
	float lastEmissionTime = 0;

	uint modelMatrixLocation = 0;

	void createParticle(const Vec3f& pos, const Vec3f& vel);
	void initializeQuadMesh();
};

