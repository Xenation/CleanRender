#pragma once
class Entity;
class Component {
public:
	Entity* entity;

	Component() = delete;
	Component(const Component&) = delete;
	Component(Entity* entity);
	~Component();

	virtual void onStart() {};
	virtual void onEnable() {};
	virtual void onUpdate() {};
	virtual void onDisable() {};
	virtual void onDestroy() {};
};

