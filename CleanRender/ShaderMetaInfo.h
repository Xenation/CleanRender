#pragma once
#include <string>
#include "GLUtils.h"

enum class ShaderFieldType {
	In,
	InLayout,
	Out,
	Uniform,
	UniformLayout,
	NativeType,
};

struct ShaderFieldInfo {
	ShaderFieldType fieldType;
	std::string name;

public:
	ShaderFieldInfo() {}
	ShaderFieldInfo(ShaderFieldType fieldType, std::string name) : fieldType(fieldType), name(name) {}

	inline virtual ShaderFieldInfo* copy() {
		return new ShaderFieldInfo(fieldType, std::string(name));
	};
};

struct ShaderNativeTypeFieldInfo : public ShaderFieldInfo {
	GLSLType type = GLSL_UNKNOWN;

public:
	ShaderNativeTypeFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type) : ShaderFieldInfo(fieldType, name), type(type) {}

	inline virtual ShaderFieldInfo* copy() override {
		return new ShaderNativeTypeFieldInfo(fieldType, std::string(name), type);
	}
};

struct ShaderInLayoutFieldInfo : public ShaderNativeTypeFieldInfo {
	uint location = 0;

public:
	ShaderInLayoutFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type, uint location) : ShaderNativeTypeFieldInfo(fieldType, name, type), location(location) {}
	
	inline virtual ShaderFieldInfo* copy() override {
		return new ShaderInLayoutFieldInfo(fieldType, std::string(name), type, location);
	}
};

struct ShaderUniformLayoutFieldInfo : public ShaderFieldInfo {
	UniformLayoutType layoutType = UniformLayoutType::STD140;
	uint binding = 0;
	ShaderFieldInfo** subFields = nullptr;
	uint subFieldCount = 0;

public:
	ShaderUniformLayoutFieldInfo(ShaderFieldType fieldType, std::string name, UniformLayoutType layoutType, uint binding) : ShaderFieldInfo(fieldType, name), layoutType(layoutType), binding(binding) {}
	~ShaderUniformLayoutFieldInfo() {
		if (subFields != nullptr) {
			for (uint i = 0; i < subFieldCount; i++) {
				delete subFields[i];
			}
			delete[] subFields;
		}
	}

	inline virtual ShaderFieldInfo* copy() override {
		ShaderUniformLayoutFieldInfo* c = new ShaderUniformLayoutFieldInfo(fieldType, std::string(name), layoutType, binding);
		c->subFields = new ShaderFieldInfo*[subFieldCount];
		c->subFieldCount = subFieldCount;
		for (uint i = 0; i < subFieldCount; i++) {
			c->subFields[i] = subFields[i]->copy();
		}
		return c;
	}

	inline GLSLType* getMembersTypes() {
		GLSLType* types = new GLSLType[subFieldCount];
		for (uint i = 0; i < subFieldCount; i++) {
			types[i] = ((ShaderNativeTypeFieldInfo*) subFields[i])->type;
		}
		return types;
	}
};

struct ShaderFileMetaInfo { // TODO rename
	ShaderFieldInfo** shaderFields = nullptr;
	uint shaderFieldCount = 0;

public:
	ShaderFileMetaInfo();
	~ShaderFileMetaInfo();
};

struct ShaderProgramMetaInfo {
	std::string* passNames = nullptr;
	uint passCount = 0;
	ShaderFieldInfo** programFields = nullptr;
	uint programFieldCount = 0;

public:
	ShaderProgramMetaInfo();
	~ShaderProgramMetaInfo();
};
