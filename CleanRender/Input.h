#pragma once
struct GLFWwindow;
typedef void (*GLFWkeyfunction)(GLFWwindow* window, int key, int scancode, int action, int mods);
class Input {
public:
	static bool wPressed;
	static bool sPressed;
	static bool aPressed;
	static bool dPressed;

	static GLFWkeyfunction getKeyCallback();

	Input() = delete;
	Input(const Input&) = delete;
	~Input() = delete;

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

