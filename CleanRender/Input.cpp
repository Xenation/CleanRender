#include "Input.h"

#include <string>
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Window.h"
#include "Debug.h"



bool Input::wPressed = false;
bool Input::sPressed = false;
bool Input::aPressed = false;
bool Input::dPressed = false;


void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Engine::window->closeWindow();
	}

	//Debug::log("Input", ("Event: key=" + std::to_string(key) + " action=" + std::to_string(action)).c_str());
	switch (key) {
	case GLFW_KEY_W:
		if (action == GLFW_PRESS) {
			wPressed = true;
		} else if (action == GLFW_RELEASE) {
			wPressed = false;
		}
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS) {
			sPressed = true;
		} else if (action == GLFW_RELEASE) {
			sPressed = false;
		}
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS) {
			aPressed = true;
		} else if (action == GLFW_RELEASE) {
			aPressed = false;
		}
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS) {
			dPressed = true;
		} else if (action == GLFW_RELEASE) {
			dPressed = false;
		}
		break;
	}
}

GLFWkeyfunction Input::getKeyCallback() {
	return key_callback;
}
