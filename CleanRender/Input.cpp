#include "Input.h"

#include <string>
#include "ShaderProgram.h"
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Window.h"
#include "Debug.h"



bool Input::keyDownStates[KEY_STATES_SIZE];
bool Input::keyPressedStates[KEY_STATES_SIZE];
bool Input::keyUpStates[KEY_STATES_SIZE];
bool Input::mouseDownStates[MOUSE_STATES_SIZE];
bool Input::mousePressedStates[MOUSE_STATES_SIZE];
bool Input::mouseUpStates[MOUSE_STATES_SIZE];

bool Input::mouseLocked = false;

Vec2f Input::mousePosition = Vec2f::zero;
Vec2f Input::mouseDelta = {INFINITY, INFINITY};


void Input::PollEvents() {
	for (unsigned int i = 0; i < KEY_STATES_SIZE; i++) {
		keyDownStates[i] = false;
		keyUpStates[i] = false;
	}
	for (unsigned int i = 0; i < MOUSE_STATES_SIZE; i++) {
		mouseDownStates[i] = false;
		mouseUpStates[i] = false;
	}
	glfwPollEvents();
}

void Input::LockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::UnlockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::getKeyDown(KeyCode keyCode) {
	return keyDownStates[keyCode];
}

bool Input::getKeyPressed(KeyCode keyCode) {
	return keyPressedStates[keyCode];
}

bool Input::getKeyUp(KeyCode keyCode) {
	return keyUpStates[keyCode];
}

bool Input::getMouseDown(MouseButton btn) {
	return getMouseDown((int) btn);
}
bool Input::getMouseDown(int btnCode) {
	return mouseDownStates[btnCode];
}

bool Input::getMousePressed(MouseButton btn) {
	return getMousePressed((int) btn);
}
bool Input::getMousePressed(int btnCode) {
	return mousePressedStates[btnCode];
}

bool Input::getMouseUp(MouseButton btn) {
	return getMouseUp((int) btn);
}
bool Input::getMouseUp(int btnCode) {
	return mouseUpStates[btnCode];
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Engine::window->closeWindow();
	}
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		ShaderProgram::reloadAll();
	}

	if (action == GLFW_PRESS) {
		keyPressedStates[key] = true;
		keyDownStates[key] = true;
	} else if (action == GLFW_RELEASE) {
		keyPressedStates[key] = false;
		keyUpStates[key] = true;
	}

	//Debug::log("Input", ("Key Event: key=" + std::to_string(key) + " action=" + std::to_string(action)).c_str());
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//Debug::log("Input", ("Mouse Button Event: button=" + std::to_string(button) + " action=" + std::to_string(action)).c_str());

	if (action == GLFW_PRESS) {
		mousePressedStates[button] = true;
		mouseDownStates[button] = true;
	} else if (action == GLFW_RELEASE) {
		mousePressedStates[button] = false;
		mouseUpStates[button] = true;
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
