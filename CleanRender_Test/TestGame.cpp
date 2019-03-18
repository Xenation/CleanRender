#include "TestGame.h"

#include <ShaderProgram.h>
#include <Material.h>



TestGame::TestGame() {}

TestGame::~TestGame() {}


void TestGame::initialize() {
	//postTestShader = ShaderProgram::find("postprocess_test");
	//postTestShader->load();
	//postTestMaterial = new Material(postTestShader, "postprocess");
	//postTestMaterial->setField(0, 10.f);

	testShader = ShaderProgram::find("test");
	if (testShader != nullptr) {
		testShader->load();
	}
	testMaterial = new Material("Test", testShader, "opaque");
	testMaterial->setField(0, Color(1.0f, 0.5f, 0.0f));

	particleShader = ShaderProgram::find("particle_basic");
	if (particleShader != nullptr) {
		particleShader->load();
	}
	particleMaterial = new Material("ParticleBasic", particleShader, "opaque");
	particleMaterial->setField(0, Color(1.0f, 1.0f, 0.0f));
}

void TestGame::preUpdate() {

}

void TestGame::postUpdate() {

}

void TestGame::cleanUp() {
	//delete postTestMaterial;
	//delete postTestShader;
	delete testMaterial;
	delete testShader;
	delete particleMaterial;
	delete particleShader;
}
