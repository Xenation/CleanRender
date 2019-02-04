#pragma once
#include <string>
#include "gl3w.h"
#include "Math.h"
class ShaderProgram {
public:
	static void initializeAll();
	static void reloadAll();
	static ShaderProgram* find(std::string name);

	ShaderProgram(std::string path);
	ShaderProgram(const ShaderProgram&) = delete;
	~ShaderProgram();

	void load();
	void reload();
	void use();
	void unuse();

	void loadProjectionMatrix(Matrix4x4f projection);
	void loadViewMatrix(Matrix4x4f view);
	void loadModelMatrix(Matrix4x4f model);

private:
	static unsigned int shaderCount;
	static ShaderProgram** shaders;
	static GLuint usedProgram;

	std::string path;
	bool loaded = false;
	GLuint program = 0;

	GLuint locationProjectionMatrix;
	GLuint locationViewMatrix;
	GLuint locationModelMatrix;

	void getAllLocations();

	char* loadSource(std::string fileName);
	GLuint loadShader(GLenum type, std::string fileName);

};

