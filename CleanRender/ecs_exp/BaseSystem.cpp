#include "BaseSystem.h"



void BaseSystem::filter(unsigned int compId) {
	componentFilter[compId / 8] |= 1 << (compId % 8);
}
