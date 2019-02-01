#pragma once
#include <string>
#include "gl3w.h"
class ShaderProgram {
public:
	static void initializeAll();
	static void reloadAll();

	ShaderProgram(std::string path);
	~ShaderProgram();
	void load();
	void reload();
private:
	static unsigned int shaderCount;
	static ShaderProgram** shaders;

	std::string path;
	bool loaded = false;

	char* loadSource(const char* fileName);
	GLuint loadShader(GLenum type, std::string fileName);

	ShaderProgram(const ShaderProgram&);
};

