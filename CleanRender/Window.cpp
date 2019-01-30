#include "Window.h"

#include "gl3w.h"
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Debug.h"
#include "Input.h"
#include "Pipeline.h"



void error_callback(int error, const char* description) {
	Debug::logError("GLFW", description);
}


bool Window::isGLFWinitialized = false;

void Window::initializeGLFW() {
	if (isGLFWinitialized) return;
	isGLFWinitialized = true;
	if (!glfwInit()) {
		Debug::fail("Failed to initialize GLFW!");
	} else {
		Debug::log("GLFW initialized");
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

void Window::resizeFrameCallback(GLFWwindow* window, int width, int height) {
	Engine::pipeline->resizeFrameBuffer(width, height);
}


Window::Window(const char* title) {
	initializeGLFW();
	window = glfwCreateWindow(640, 480, title, NULL, NULL);
	if (!window) {
		Debug::fail("Failed to create window!");
	} else {
		Debug::log("Window created");
	}
	glfwSetKeyCallback(window, Input::getKeyCallback());
	glfwSetFramebufferSizeCallback(window, resizeFrameCallback);

	glfwMakeContextCurrent(window);
	if (gl3wInit2(glfwGetProcAddress)) {
		Debug::fail("Failed to initialize GL3W!");
	} else {
		Debug::log("GL3W initialized");
	}
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::display() {
	glfwPollEvents();
	glfwSwapBuffers(window);
}

int Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::closeWindow() {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}
