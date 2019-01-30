#pragma once
struct GLFWwindow;
typedef void (*GLFWkeyfunction)(GLFWwindow* window, int key, int scancode, int action, int mods);
class Input {
public:
	static GLFWkeyfunction getKeyCallback();
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	Input();
	Input(const Input&);
	~Input();
};

