#include "ParticleSystem.h"

#include "Time.h"
#include "Entity.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "SpecializedShaderProgram.h"



ParticleSystem::ParticleSystem(Entity* entity) : Renderer(entity) {}

ParticleSystem::~ParticleSystem() {
	if (particles != nullptr) {
		delete[] particles;
	}
}

void ParticleSystem::startEmit() {
	if (isEmitting) return;
	if (Mesh::quadCentered == nullptr) {
		initializeQuadMesh();
	}
	isEmitting = true;
	uint nMax = ceilToInt(emitRate * lifetime);
	emissionInterval = 1.0f / emitRate;
	if (nMax != maxParticles) {
		maxParticles = nMax;
		if (particles != nullptr) {
			delete[] particles;
		}
		particles = new Particle[maxParticles];
		for (uint i = 0; i < maxParticles; i++) {
			particles[i].alive = false;
		}
	}
}

void ParticleSystem::onUpdate() {
	// Kill expired Particles
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		if (Time::time - particles[i].creationTime > lifetime) {
			particles[i].alive = false;
		}
	}

	// New Particle generation
	if (isEmitting && Time::time - lastEmissionTime > emissionInterval) {
		lastEmissionTime = Time::time;
		createParticle(entity->transform->getWorldPosition(), entity->transform->up() * 10);
	}

	// Particles state update
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue; // Skip dead
		// Apply gravity
		particles[i].velocity += Vec3f::down * 9.81f * Time::deltaTime;
		// Change position
		particles[i].position += particles[i].velocity * Time::deltaTime;
	}
}

void ParticleSystem::stopEmit() {
	isEmitting = false;
}

void ParticleSystem::setMaterial(Material* mat) {
	Renderer::setMaterial(mat);
	modelMatrixLocation = mat->specializedProgram->getUniformLocation("modelMatrix");
}

void ParticleSystem::render() {
	
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		Matrix4x4f particleMatrix = Matrix4x4f::translation(particles[i].position);

		material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, particleMatrix);

		Mesh::quadCentered->render();
	}
}

void ParticleSystem::createParticle(const Vec3f& pos, const Vec3f& vel) {
	for (uint i = 0; i < maxParticles; i++) {
		if (particles[i].alive) continue;
		particles[i].position = pos;
		particles[i].velocity = vel;
		particles[i].creationTime = Time::time;
		particles[i].alive = true;
		break;
	}
}

void ParticleSystem::initializeQuadMesh() {
	Mesh::quadCentered = new Mesh("Quad", 4, 6);
	Mesh::quadCentered->setAttributesDefinition(2, new int[2]{3, 2});
	Mesh::quadCentered->setAttribute(0, new float[4 * 3]{
		-0.5f, -0.5f, 0,
		-0.5f, 0.5f, 0,
		0.5f, 0.5f, 0,
		0.5f, -0.5f, 0
	});
	Mesh::quadCentered->setAttribute(1, new float[4 * 2]{
		0, 0,
		0, 1,
		1, 1,
		1, 0
	});
	Mesh::quadCentered->setIndices(new uint[6]{1, 0, 3, 1, 3, 2});
	Mesh::quadCentered->uploadToGL();
}
