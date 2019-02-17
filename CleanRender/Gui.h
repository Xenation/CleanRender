#pragma once
class Window;
class Gui {
public:
	Gui(Window* window);
	~Gui();

	void update();
	virtual void onUpdate();
	void render();

private:
	Window* window;
};

