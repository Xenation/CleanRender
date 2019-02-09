#include <iostream>
#include <Engine.h>


int main(int argc, char** args) {

	Engine::initialize();

	Engine::loop();

	Engine::destroy();

	system("pause");
	return 0;
}
