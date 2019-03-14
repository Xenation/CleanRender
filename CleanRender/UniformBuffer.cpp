#include "UniformBuffer.h"

#include <string>
#include "Debug.h"



UniformLayout::UniformLayout(unsigned int binding, ShaderUniformBufferLayoutFieldInfo* bufferField)
	: UniformLayout(binding, bufferField->layoutType, bufferField->subFieldCount, bufferField->getMembersTypes()) {
	memberNames = new std::string[bufferField->subFieldCount];
	for (uint i = 0; i < bufferField->subFieldCount; i++) {
		memberNames[i] = bufferField->subFields[i]->name;
	}
}

UniformLayout::UniformLayout(unsigned int binding, unsigned int memberCount, GLSLType* members)
	: UniformLayout(binding, UniformLayoutType::STD140, memberCount, members) {

}

UniformLayout::UniformLayout(unsigned int binding, UniformLayoutType type, unsigned int memberCount, GLSLType* members)
	: binding(binding), type(type), memberCount(memberCount), members(members) {
	membersOffsets = new unsigned int[memberCount];
}

UniformLayout::~UniformLayout() {
	if (members != nullptr) {
		delete[] members;
		members = nullptr;
	}
	if (membersOffsets != nullptr) {
		delete[] membersOffsets;
		membersOffsets = nullptr;
	}
}

void UniformLayout::setMember(unsigned int index, bool value) {
	unsigned char bytes[4]{value, 0, 0, 0}; // Required because booleans are 4bytes in glsl
	setMember(index, bytes, 4);
}
void UniformLayout::setMember(unsigned int index, int value) {
	setMember(index, (unsigned char*) &value, 4);
}
void UniformLayout::setMember(unsigned int index, unsigned int value) {
	setMember(index, (unsigned char*) &value, 4);
}
void UniformLayout::setMember(unsigned int index, float value) {
	setMember(index, (unsigned char*) &value, 4);
}
void UniformLayout::setMember(unsigned int index, double value) {
	setMember(index, (unsigned char*) &value, 8);
}
void UniformLayout::setMember(unsigned int index, Vec2f value) {
	setMember(index, (unsigned char*) &value, 8);
}
void UniformLayout::setMember(unsigned int index, Vec3f value) {
	setMember(index, (unsigned char*) &value, 12);
}
void UniformLayout::setMember(unsigned int index, Vec4f value) {
	setMember(index, (unsigned char*) &value, 16);
}
void UniformLayout::setMember(unsigned int index, Vec2i value) {
	setMember(index, (unsigned char*) &value, 8);
}
void UniformLayout::setMember(unsigned int index, Vec3i value) {
	setMember(index, (unsigned char*) &value, 12);
}
void UniformLayout::setMember(unsigned int index, Vec4i value) {
	setMember(index, (unsigned char*) &value, 16);
}
void UniformLayout::setMember(unsigned int index, Matrix4x4f value) {
	setMember(index, (unsigned char*) &value, 64);
}

void UniformLayout::setMember(unsigned int index, unsigned char* bytes, unsigned int byteSize) {
	for (unsigned int i = 0; i < byteSize; i++) {
		buffer[membersOffsets[index] + i] = bytes[i];
	}
}

unsigned char* UniformLayout::getBytes(unsigned int index) {
	return &buffer[membersOffsets[index]];
}

void UniformLayout::computeLayoutOffsets() {
	unsigned int currentOffset = 0;
	switch (type) {
	case UniformLayoutType::STD140:
		for (unsigned int i = 0; i < memberCount; i++) {
			membersOffsets[i] = currentOffset;
			currentOffset += glslTypeBaseAlignment(members[i]);
		}
		size = currentOffset;
		break;
	default:
		Debug::logError("UniformBuffer", "Unsupported layout type!");
		break;
	}
}



UniformBuffer::UniformBuffer(std::string name) : name(name) {}

UniformBuffer::~UniformBuffer() {
	if (layouts != nullptr) {
		delete[] layouts;
		layouts = nullptr;
	}
	if (buffer != nullptr) {
		delete[] buffer;
		buffer = nullptr;
	}
	deleteFromGL();
}


void UniformBuffer::setLayouts(unsigned int layoutCount, UniformLayout* layouts) {
	this->layouts = layouts;
	this->layoutCount = layoutCount;
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < layoutCount; i++) {
		this->layouts[i].offset = currentOffset;
		this->layouts[i].computeLayoutOffsets();
		currentOffset += this->layouts[i].size;
		currentOffset = glGetUniformBufferAlignment(currentOffset);
	}
	bufferSize = this->layouts[this->layoutCount - 1].offset + this->layouts[this->layoutCount - 1].size;
	buffer = new unsigned char[bufferSize];
	for (unsigned int i = 0; i < layoutCount; i++) {
		this->layouts[i].buffer = buffer + layouts[i].offset;
	}
}

UniformLayout& UniformBuffer::getLayout(unsigned int index) {
	return layouts[index];
}

void UniformBuffer::updateLayout(unsigned int index) { // TODO make a version of this function to update a single member of a layout
	if (!loadedToGL) {
		Debug::logError("UniformBuffer", "Trying to update a layout not yet uploaded!");
		return;
	}
	unsigned int offset = layouts[index].offset;
	unsigned int size = layouts[index].size;
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, layouts[index].buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindLayout(unsigned int index) {
	if (!loadedToGL) {
		Debug::logError("UniformBuffer", "Trying to bind a layout not yet uploaded!");
		return;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, layouts[index].binding, ubo, layouts[index].offset, layouts[index].size);
}

void UniformBuffer::uploadToGL() {
	if (ubo == 0) {
		glGenBuffers(1, &ubo);
		std::string fullName = "UBO " + name;
		glObjectLabel(GL_BUFFER, ubo, fullName.size(), fullName.c_str());
	}
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (unsigned int i = 0; i < layoutCount; i++) {
		glBindBufferRange(GL_UNIFORM_BUFFER, layouts[i].binding, ubo, layouts[i].offset, layouts[i].size);
	}

	loadedToGL = true;
}

void UniformBuffer::deleteFromGL() {
	if (ubo != 0) {
		glDeleteBuffers(1, &ubo);
		ubo = 0;
	}
	loadedToGL = false;
}
