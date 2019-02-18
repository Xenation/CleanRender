#include "Input.h"

#include <string>
#include "ShaderProgram.h"
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Window.h"
#include "Debug.h"



bool Input::mouseLocked = false;

bool Input::mouseLeftPressed = false;
bool Input::mouseRightPressed = false;

Vec2f Input::mousePosition = Vec2f::zero;
Vec2f Input::mouseDelta = {INFINITY, INFINITY};

bool Input::wPressed = false;
bool Input::sPressed = false;
bool Input::aPressed = false;
bool Input::dPressed = false;
bool Input::qPressed = false;
bool Input::ePressed = false;

bool Input::lShiftPressed = false;

bool Input::f1Down = false;
bool Input::f1Pressed = false;


void Input::PollEvents() {
	f1Down = false;
	glfwPollEvents();
}

void Input::LockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::UnlockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Engine::window->closeWindow();
	}
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		ShaderProgram::reloadAll();
	}

	//Debug::log("Input", ("Key Event: key=" + std::to_string(key) + " action=" + std::to_string(action)).c_str());
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
	case GLFW_KEY_Q:
		if (action == GLFW_PRESS) {
			qPressed = true;
		} else if (action == GLFW_RELEASE) {
			qPressed = false;
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_PRESS) {
			ePressed = true;
		} else if (action == GLFW_RELEASE) {
			ePressed = false;
		}
		break;
	case GLFW_KEY_LEFT_SHIFT:
		if (action == GLFW_PRESS) {
			lShiftPressed = true;
		} else if (action == GLFW_RELEASE) {
			lShiftPressed = false;
		}
		break;
	case GLFW_KEY_F1:
		if (action == GLFW_PRESS) {
			f1Pressed = true;
			f1Down = true;
		} else if (action == GLFW_RELEASE) {
			f1Pressed = false;
		}
		break;
	}
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//Debug::log("Input", ("Mouse Button Event: button=" + std::to_string(button) + " action=" + std::to_string(action)).c_str());
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) {
			mouseLeftPressed = true;
		} else if (action == GLFW_RELEASE) {
			mouseLeftPressed = false;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) {
			mouseRightPressed = true;
			LockMouse(); // TODO remove from here
		} else if (action == GLFW_RELEASE) {
			mouseRightPressed = false;
			UnlockMouse(); // TODO remove from here
		}
		break;
	}
}

void Input::mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Vec2f nMousePosition = Vec2f((float) xpos, (float) ypos);
	if (mouseDelta == Vec2f(INFINITY, INFINITY)) { // kinda ugly ...
		mousePosition = nMousePosition;
	}
	mouseDelta = nMousePosition - mousePosition;
	mousePosition = nMousePosition;
	//Debug::log("Input", ("Mouse Position Event: x=" + std::to_string(xpos) + " y=" + std::to_string(ypos) + " delta=(" + std::to_string(mouseDelta.x) + ", " + std::to_string(mouseDelta.y) + ")").c_str());
}

GLFWkeyFunction Input::getKeyCallback() {
	return keyCallback;
}

GLFWmouseButtonFunction Input::getMouseButtonCallback() {
	return mouseButtonCallback;
}

GLFWmousePositionFunction Input::getMousePosCallback() {
	return mousePositionCallback;
}
