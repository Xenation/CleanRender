#pragma once
#include <string>
#include "XTypes.h"
#include "ShaderMetaInfo.h"

class SpecializedShaderProgram;
class RenderPass;
class Pipeline;
class ShaderReader;

class ShaderProgram {
public:
	static ShaderProgram** shaders;
	static uint shaderCount;

	static void initializeAll(Pipeline* pipeline);
	static void reloadAll();
	static ShaderProgram* find(std::string name);

	const std::string name;
	ShaderProgramMetaInfo* info = nullptr;

	ShaderProgram(std::string name);
	ShaderProgram(const ShaderProgram&) = delete;
	~ShaderProgram();

	void load();
	void reload();
	void unload();

	SpecializedShaderProgram* getSpecializedProgram(std::string renderPassName);
	SpecializedShaderProgram* getSpecializedProgram(RenderPass* renderPass);
	ShaderFieldInfo* getMaterialFieldInfo();

private:
	static Pipeline* defaultPipeline;

	SpecializedShaderProgram** specializedPrograms = nullptr;
	uint specializedProgramsCount = 0;
	bool loaded = false;

	ShaderReader* readShaders();
};

