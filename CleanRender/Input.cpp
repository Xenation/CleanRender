#include "Input.h"

#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Window.h"



Input::Input() {}
Input::~Input() {}


void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Engine::window->closeWindow();
	}
}

GLFWkeyfunction Input::getKeyCallback() {
	return key_callback;
}
