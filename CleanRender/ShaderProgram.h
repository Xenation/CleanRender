#pragma once
#include <string>
#include <gl3w.h>
#include "XMath.h"
#include "HollowSet.h"
class Renderer;
class ShaderProgram {
public:
	static ShaderProgram* errorShader;
	static unsigned int shaderCount;
	static ShaderProgram** shaders;
	
	static void initializeAll();
	static void reloadAll();
	static ShaderProgram* find(std::string name);

	HollowSet<Renderer*> renderers;

	ShaderProgram(std::string path);
	ShaderProgram(const ShaderProgram&) = delete;
	~ShaderProgram();

	void load();
	void reload();
	void use();
	void unuse();

	GLuint getUniformLocation(const char* str);
	void loadFloat(GLuint location, float f);
	void loadVec2f(GLuint location, Vec2f vec);
	void loadVec3f(GLuint location, Vec3f vec);
	void loadVec4f(GLuint location, Vec4f vec);
	void loadMatrix(GLuint location, Matrix4x4f matrix);
	void setTextureUnit(GLuint location, unsigned int unit);

	void loadProjectionMatrix(Matrix4x4f projection);
	void loadViewMatrix(Matrix4x4f view);
	void loadModelMatrix(Matrix4x4f model);
	void loadTime(float time);

private:
	std::string path;
	bool loaded = false;
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint geometry = 0;
	GLuint fragment = 0;

	GLuint locationProjectionMatrix;
	GLuint locationViewMatrix;
	GLuint locationModelMatrix;
	GLuint locationTime;

	void getAllLocations();

	void load(GLuint vs, GLuint gs, GLuint fs, bool silent = false);
	GLuint loadShader(GLenum type, std::string fileName, bool silent = false);
	GLuint loadShaderFromSource(GLenum type, const char* src, bool silent = false);
	char* loadSource(std::string fileName, bool silent = false);
};

