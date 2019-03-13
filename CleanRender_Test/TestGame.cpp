#include "TestGame.h"

#include <ShaderProgram.h>
#include <Material.h>



TestGame::TestGame() {}

TestGame::~TestGame() {}


void TestGame::initialize() {
	postTestShader = ShaderProgram::find("postprocess_test");
	postTestShader->load();
	postTestMaterial = new Material(postTestShader, "postprocess");
	postTestMaterial->setField(0, 10.f);
}

void TestGame::preUpdate() {

}

void TestGame::postUpdate() {

}

void TestGame::cleanUp() {
	delete postTestMaterial;
	delete postTestShader;
}
