#pragma once
#include "XMath.h"
struct GLFWwindow;
typedef void (*GLFWkeyFunction)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void (*GLFWmouseButtonFunction)(GLFWwindow* window, int button, int action, int mods);
typedef void (*GLFWmousePositionFunction)(GLFWwindow* window, double xpos, double ypos);
class Input {
public:
	static bool mouseLocked;

	static bool mouseLeftPressed;
	static bool mouseRightPressed;

	static Vec2f mousePosition;
	static Vec2f mouseDelta;

	static bool wPressed;
	static bool sPressed;
	static bool aPressed;
	static bool dPressed;
	static bool qPressed;
	static bool ePressed;

	static bool lShiftPressed;

	static void PollEvents();
	static void LockMouse();
	static void UnlockMouse();

	static GLFWkeyFunction getKeyCallback();
	static GLFWmouseButtonFunction getMouseButtonCallback();
	static GLFWmousePositionFunction getMousePosCallback();

	Input() = delete;
	Input(const Input&) = delete;
	~Input() = delete;

private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
};

