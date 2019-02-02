#pragma once
#include <string>
#include "gl3w.h"
class ShaderProgram {
public:
	static void initializeAll();
	static void reloadAll();
	static ShaderProgram* find(std::string name);

	ShaderProgram(std::string path);
	~ShaderProgram();
	void load();
	void reload();
	void use();
	void unuse();
private:
	static unsigned int shaderCount;
	static ShaderProgram** shaders;
	static GLuint usedProgram;

	std::string path;
	bool loaded = false;
	GLuint program = 0;

	char* loadSource(std::string fileName);
	GLuint loadShader(GLenum type, std::string fileName);

	ShaderProgram(const ShaderProgram&);
};

