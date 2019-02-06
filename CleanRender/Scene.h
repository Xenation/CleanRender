#pragma once
class Scene {
public:
	bool isLoaded = false;

	Scene();
	Scene(const Scene&) = delete;
	~Scene();

	virtual void load();
	virtual void destroy();
};

