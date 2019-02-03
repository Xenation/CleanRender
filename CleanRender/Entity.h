#pragma once
#include <type_traits>
#include <typeinfo>
#include "Component.h"
class Transform;
class Entity {
public:
	typedef unsigned int Id;
	Id id;
	Transform* transform;
	bool isDying = false;

	Entity();
	Entity(const Entity&) = delete;
	~Entity();

	// Adds a component
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent(T* comp) {
		if (componentCount == componentCapacity) {
			growComponentsArray();
			if (componentCount == componentCapacity) { // Grow failled
				return nullptr;
			}
		}
		components[getFreeComponentIndex()] = comp;
		componentCount++;
		return comp;
	}

	// Adds a component by creating a new
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent() {
		if (componentCount == componentCapacity) {
			growComponentsArray();
			if (componentCount == componentCapacity) { // Grow failled
				return nullptr;
			}
		}
		T* nComponent = new T(this);
		components[getFreeComponentIndex()] = nComponent;
		nComponent->entity = this;
		componentCount++;
		return nComponent;
	}
	
	// Gets a component of the given type
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* getComponent() {
		for (int i = 0; i < componentCapacity; i++) {
			if (components[i] != nullptr && typeid(components[i]) == T) return components[i];
		}
		return nullptr;
	}

	// Gets all the components of the given type
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T** getComponents() {
		unsigned int count = 0;
		for (int i = 0; i < componentCapacity; i++) {
			if (count == componentCount) break;
			if (components[i] != nullptr && typeid(components[i]) == T) count++;
		}
		T** comps = new Component*[count];
		getComponents(comps, count);
		return comps;
	}

	// Gets all the components of the given type using a preallocated array, returns the count of components found
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	unsigned int getComponents(T** arr, unsigned int arrSize) {
		unsigned int arrIndex = 0;
		for (int i = 0; i < componentCapacity && arrIndex < arrSize; i++) {
			if (arrIndex == componentCount) break;
			if (components[i] != nullptr && typeid(components[i]) == T) {
				arr[arrIndex++] = components[i];
			}
		}
		return arrIndex;
	}

	// Removes a previously added component
	void removeComponent(Component* comp) {
		for (unsigned int i = 0; i < componentCount; i++) {
			if (components[i] != nullptr && components[i] == comp) {
				components[i] = nullptr;
				break;
			}
		}
	}

private:
	Component** components = nullptr;
	unsigned int componentCount = 0;
	unsigned int componentCapacity = 0;

	void initComponentsArray();
	void growComponentsArray();
	unsigned int getFreeComponentIndex();
};

