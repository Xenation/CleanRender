#pragma once
struct GLFWwindow;
class Renderer;
class Window {
public:
	Window(const char* title = "CleanRender");
	~Window();
	void display();
	int shouldClose();
	void closeWindow();
private:
	static bool isGLFWinitialized;
	static void initializeGLFW();
	static void resizeFrameCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;

	Window(const Window& w) {};
};

