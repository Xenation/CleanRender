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

	void enable();
	void disable();
	inline bool isEnabled() { return enabled; }
private:
	bool enabled = true;
};

