#include <iostream>
#include <Engine.h>

#include "TestScene.h"


int main(int argc, char** args) {

	Engine::initialize();
	Engine::scene = new TestScene();
	Engine::scene->load();

	Engine::loop();

	Engine::destroy();

	system("pause");
	return 0;
}
