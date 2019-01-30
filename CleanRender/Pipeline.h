#pragma once
class Pipeline {
public:
	Pipeline();
	~Pipeline();
	void render();
	void resizeFrameBuffer(int width, int height);
private:
	Pipeline(const Pipeline&) {};
};

