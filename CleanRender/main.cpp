#include "gl3w.h"
#include <GLFW\glfw3.h>
#include <iostream>

#include "Debug.h"
#include "Window.h"
#include "Engine.h"


int main(int argc, char** args) {

	Engine::initialize();

	Engine::loop();

	Engine::destroy();

	system("pause");
	return 0;
}
