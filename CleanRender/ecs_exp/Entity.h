#pragma once
#include <cstdlib>
#include "BaseComponent.h"
class Entity {
public:
	typedef unsigned int Id;

	BaseComponent* components;
	Id id;
	
	Entity();
	Entity(const Entity& entity);
	~Entity();

	void addComponent(BaseComponent component);
	void removeComponent(unsigned int flagIndex);
	bool hasComponents(char* flags);
	
	void* operator new(size_t size) {
		size += BaseComponent::typesBitFieldSize;
		void* p = (void*) malloc(size);
		//((Entity*) p)->componentFlags = (char*) p + sizeof(Entity); // Basicly useless because the constructor sets it to zero
		return p;
	}
	void operator delete(void* p) {
		free(p);
	}
private:
	char* componentFlags;

	void clearComponentFlags();
};

