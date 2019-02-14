#include "ShaderProgram.h"

#include <filesystem>
#include "Debug.h"

#define SHADER_DIRECTORY "res/shaders/"
#define SHADER_CODE_ERROR_VS "#version 330 core\nlayout (location = 0) in vec3 vp;\nuniform mat4x4 projectionMatrix;\nuniform mat4x4 viewMatrix;\nuniform mat4x4 modelMatrix;\nvoid main() { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0); }\n\0"
#define SHADER_CODE_ERROR_FS "#version 330 core\nout vec4 fc;\nvoid main() { fc = vec4(1.0, 0.0, 1.0, 1.0); }\n\0"


namespace fs = std::filesystem;

ShaderProgram* ShaderProgram::errorShader = nullptr;
unsigned int ShaderProgram::shaderCount = 0;
ShaderProgram** ShaderProgram::shaders = nullptr;

void ShaderProgram::initializeAll() {
	// Loading Error shader
	errorShader = new ShaderProgram("Error");
	errorShader->load(errorShader->loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_VS), 0, errorShader->loadShaderFromSource(GL_FRAGMENT_SHADER, SHADER_CODE_ERROR_FS));


	// Searching for shaders
	if (!fs::exists(SHADER_DIRECTORY)) {
		Debug::logError("ShaderInit", ("Shader directory " + std::string(SHADER_DIRECTORY) + " not found!").c_str());
		shaders = new ShaderProgram*[1];
		shaders[0] = errorShader;
		return;
	}
	shaderCount = 1;
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) { // Counting Pass
		shaderCount++;
	}
	Debug::log("ShaderInit", ("Found " + std::to_string(shaderCount - 1) + " potential shaders:").c_str());
	shaders = new ShaderProgram*[shaderCount];
	shaders[0] = errorShader;
	int index = 1;
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) { // Filling Pass
		if (!entry.is_directory()) continue; // Keep directories only
		shaders[index] = new ShaderProgram(entry.path().filename().string()); // TODO be carefull of UTF8 names
		Debug::log("ShaderInit", ("    " + shaders[index]->path).c_str());
		index++;
	}
}

void ShaderProgram::reloadAll() {
	// Skip first as it is the fallback
	for (unsigned int i = 1; i < shaderCount; i++) {
		shaders[i]->reload();
	}
}

ShaderProgram* ShaderProgram::find(std::string name) {
	for (int i = 0; i < shaderCount; i++) {
		ShaderProgram* sp = shaders[i];
		if (shaders[i]->path == name) {
			return shaders[i];
		}
	}
	Debug::log("ShaderProgram", ("Could not find program named " + name).c_str());
	return nullptr;
}


ShaderProgram::ShaderProgram(std::string path) : renderers(4, 16) {
	this->path = path;
}

ShaderProgram::~ShaderProgram() {

}

void ShaderProgram::load() {
	std::string vertexFile = std::string("vs.glsl");
	std::string geometryFile = std::string("gs.glsl");
	std::string fragmentFile = std::string("fs.glsl");

	Debug::log("ShaderProgram", ("Loading Program " + path + "...").c_str());

	vertex = loadShader(GL_VERTEX_SHADER, vertexFile);
	geometry = loadShader(GL_GEOMETRY_SHADER, geometryFile, true);
	fragment = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

	if (vertex == 0 || fragment == 0) {
		vertex = loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_VS, true);
		fragment = loadShaderFromSource(GL_FRAGMENT_SHADER, SHADER_CODE_ERROR_FS, true);
	}

	load(vertex, geometry, fragment);
}

void ShaderProgram::reload() {
	if (!loaded) {
		load();
		return;
	}
	if (program != 0) {
		if (vertex != 0) {
			glDetachShader(program, vertex);
			glDeleteShader(vertex);
		}
		if (fragment != 0) {
			glDetachShader(program, fragment);
			glDeleteShader(fragment);
		}
		glDeleteProgram(program);
	}
	load();
}

void ShaderProgram::use() {
	glUseProgram(program);
}

void ShaderProgram::unuse() {
	glUseProgram(0);
}

GLuint ShaderProgram::getUniformLocation(const char* str) {
	return glGetUniformLocation(program, str);
}

void ShaderProgram::loadFloat(GLuint location, float f) {
	glUniform1f(location, f);
}

void ShaderProgram::loadVec2f(GLuint location, Vec2f vec) {
	glUniform2f(location, vec.x, vec.y);
}

void ShaderProgram::loadVec3f(GLuint location, Vec3f vec) {
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::loadVec4f(GLuint location, Vec4f vec) {
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::loadMatrix(GLuint location, Matrix4x4f matrix) {
	glUniformMatrix4fv(location, 1, false, matrix.data);
}

void ShaderProgram::loadProjectionMatrix(Matrix4x4f mat) {
	glUniformMatrix4fv(locationProjectionMatrix, 1, false, mat.data);
}

void ShaderProgram::loadViewMatrix(Matrix4x4f mat) {
	glUniformMatrix4fv(locationViewMatrix, 1, false, mat.data);
}

void ShaderProgram::loadModelMatrix(Matrix4x4f mat) {
	glUniformMatrix4fv(locationModelMatrix, 1, false, mat.data);
}

void ShaderProgram::loadTime(float time) {
	loadFloat(locationTime, time);
}

void ShaderProgram::getAllLocations() {
	locationProjectionMatrix = glGetUniformLocation(program, "projectionMatrix");
	locationViewMatrix = glGetUniformLocation(program, "viewMatrix");
	locationModelMatrix = glGetUniformLocation(program, "modelMatrix");
	locationTime = glGetUniformLocation(program, "time");
}

void ShaderProgram::load(GLuint vs, GLuint gs, GLuint fs, bool silent) {
	if (vs <= 0 || fs <= 0) return;

	program = glCreateProgram();
	if (vs > 0) {
		glAttachShader(program, vs);
	}
	if (gs > 0) {
		glAttachShader(program, gs);
	}
	if (fs > 0) {
		glAttachShader(program, fs);
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		if (!silent) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			memset(infoLog, 0, maxLength * sizeof(char));

			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

			std::string error = "Could not link";
			if (vs != 0) {
				error += " [vertex shader]";
			}
			if (gs != 0) {
				error += " [geometry shader]";
			}
			if (fs != 0) {
				error += " [fragment shader]";
			}

			error += ", log is: " + std::string(infoLog);
			Debug::logError("ShaderProgram", error.c_str());

			delete[] infoLog;
		}

		glDeleteProgram(program);
		program = 0;
		return;
	}
	Debug::log("ShaderProgram", "Program load and link done!");
	getAllLocations();
	loaded = true;
}

GLuint ShaderProgram::loadShader(GLenum type, std::string fileName, bool silent) {
	char* src = nullptr;
	std::string filePath = std::string(SHADER_DIRECTORY) + path + "/" + fileName;

	if (!fs::exists(filePath)) {
		if (!silent) Debug::logError("ShaderLoad", ("File " + filePath + " not found").c_str());
		return 0;
	}

	src = loadSource(filePath);
	if (src == nullptr) {
		return 0;
	}

	GLuint shader = loadShaderFromSource(type, src);
	free(src);
	src = nullptr;

	return shader;
}

GLuint ShaderProgram::loadShaderFromSource(GLenum type, const char* src, bool silent) {
	GLuint shader = 0;
	GLsizei logSize = 0;
	GLint compileStatus = GL_TRUE;
	char* log = nullptr;
	std::string typeStr;
	switch (type) {
	case GL_VERTEX_SHADER:
		typeStr = std::string("Vertex Shader");
		break;
	case GL_FRAGMENT_SHADER:
		typeStr = std::string("Fragment Shader");
		break;
	case GL_GEOMETRY_SHADER:
		typeStr = std::string("Geometry Shader");
		break;
	}

	shader = glCreateShader(type);
	if (shader == 0) {
		if (!silent) Debug::logError("ShaderLoad", ("Program  " + path + " could not create " + typeStr).c_str());
		return 0;
	}

	glShaderSource(shader, 1, (const GLchar**) &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	std::string logStr;
	if (!silent) {
		log = new char[logSize + 1];
		if (log == nullptr) {
			Debug::logError("ShaderLoad", ("Program " + path + " could not allocate memory for " + typeStr + " compilation log!").c_str());
			return 0;
		}
		memset(log, '\0', logSize + 1);

		glGetShaderInfoLog(shader, logSize, &logSize, log);

		logStr = std::string(log);
	}

	if (compileStatus != GL_TRUE) {
		if (!silent) {
			Debug::logError("ShaderLoad", ("Program " + path + " could not compile " + typeStr + ": \n" + logStr).c_str());
			delete[] log;
		}
		glDeleteShader(shader);
		return 0;
	} else {
		if (!silent) {
			Debug::log("ShaderLoad", ("Program " + path + " " + typeStr + " compile success").c_str());
			if (logStr.length() > 0) {
				Debug::log("ShaderLoad", ("Compile result " + typeStr + ":\n" + logStr).c_str());
			}
		}
		delete[] log;
	}

	return shader;
}

char* ShaderProgram::loadSource(std::string filePath, bool silent) {
	char* src = nullptr;
	FILE* fp = nullptr;
	long size;
	long i;

	fopen_s(&fp, filePath.c_str(), "r");
	if (fp == nullptr) {
		if (!silent) Debug::logError("ShaderLoad", ("Could not load shader source file " + filePath).c_str());
		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	src = (char*) malloc(size + 1);
	if (src == nullptr) {
		fclose(fp);
		if (!silent) Debug::logError("ShaderLoad", ("Could not allocate memory for shader source file " + filePath).c_str());
		return nullptr;
	}

	for (i = 0; i < size && !feof(fp); i++) {
		src[i] = fgetc(fp);
	}

	src[i] = '\0';

	if (src[i - 1] == EOF) {
		src[i - 1] = '\0';
	}

	fclose(fp);

	return src;
}
