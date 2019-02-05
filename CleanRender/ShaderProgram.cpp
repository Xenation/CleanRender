#include "ShaderProgram.h"

#include <filesystem>
#include "Debug.h"
#define SHADER_DIRECTORY "res/shaders/"



namespace fs = std::filesystem;

unsigned int ShaderProgram::shaderCount = 0;
ShaderProgram** ShaderProgram::shaders = nullptr;
GLuint ShaderProgram::usedProgram = 0;

void ShaderProgram::initializeAll() {
	if (!fs::exists(SHADER_DIRECTORY)) {
		Debug::logError("ShaderInit", ("Shader directory " + std::string(SHADER_DIRECTORY) + " not found!").c_str());
		return;
	}
	shaderCount = 0;
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) { // Counting Pass
		shaderCount++;
	}
	Debug::log("ShaderInit", ("Found " + std::to_string(shaderCount) + " potential shaders:").c_str());
	shaders = new ShaderProgram*[shaderCount];
	int index = 0;
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) { // Filling Pass
		if (!entry.is_directory()) continue; // Keep directories only
		shaders[index] = new ShaderProgram(entry.path().filename().string()); // TODO be carefull of UTF8 names
		Debug::log("ShaderInit", ("    " + shaders[index]->path).c_str());
	}
}

void ShaderProgram::reloadAll() {
	for (unsigned int i = 0; i < shaderCount; i++) {
		shaders[i]->reload();
	}
}

ShaderProgram* ShaderProgram::find(std::string name) {
	for (int i = 0; i < shaderCount; i++) {
		if (shaders[i]->path == name) {
			return shaders[i];
		}
	}
	Debug::log("ShaderProgram", ("Could not find program named " + name).c_str());
	return nullptr;
}


ShaderProgram::ShaderProgram(std::string path) {
	this->path = path;
}

ShaderProgram::~ShaderProgram() {

}

void ShaderProgram::load() {
	std::string vertexFile = std::string("vs.glsl");
	std::string fragmentFile = std::string("fs.glsl");

	GLuint vs = loadShader(GL_VERTEX_SHADER, vertexFile);
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

	if (vs > 0 || fs > 0) {
		program = glCreateProgram();
		if (vs > 0) {
			glAttachShader(program, vs);
		}
		if (fs > 0) {
			glAttachShader(program, fs);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			memset(infoLog, 0, maxLength * sizeof(char));

			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

			glDeleteProgram(program);

			std::string error = "Could not link";
			if (vs != 0) {
				error += " vs[" + vertexFile + "]";
			}
			if (fs != 0) {
				error += " fs[" + fragmentFile + "]";
			}

			error += ", log is: " + std::string(infoLog);
			Debug::logError("ShaderProgram", error.c_str());

			delete[] infoLog;

			program = 0;
			return;
		}
		Debug::log("ShaderProgram", "Program load and link done!");
		getAllLocations();
		return;
	}

}

void ShaderProgram::reload() {
	if (!loaded) return;
}

void ShaderProgram::use() {
	/*if (program == usedProgram) return;
	usedProgram = program;*/
	glUseProgram(program);
}

void ShaderProgram::unuse() {
	//usedProgram = 0;
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

void ShaderProgram::getAllLocations() {
	locationProjectionMatrix = glGetUniformLocation(program, "projectionMatrix");
	locationViewMatrix = glGetUniformLocation(program, "viewMatrix");
	locationModelMatrix = glGetUniformLocation(program, "modelMatrix");
}

char* ShaderProgram::loadSource(std::string filePath) {
	char* src = nullptr;
	FILE* fp = nullptr;
	long size;
	long i;

	fopen_s(&fp, filePath.c_str(), "r");
	if (fp == nullptr) {
		Debug::logError("ShaderLoad", ("Could not load shader source file " + filePath).c_str());
		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	src = (char*) malloc(size + 1);
	if (src == nullptr) {
		fclose(fp);
		Debug::logError("ShaderLoad", ("Could not allocate memory for shader source file " + filePath).c_str());
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

GLuint ShaderProgram::loadShader(GLenum type, std::string fileName) {
	GLuint shader = 0;
	GLsizei logSize = 0;
	GLint compileStatus = GL_TRUE;
	char* log = nullptr;
	char* src = nullptr;
	std::string filePath = std::string(SHADER_DIRECTORY) + path + "/" + fileName;

	if (!fs::exists(filePath)) {
		Debug::logError("ShaderLoad", ("File " + filePath + " not found").c_str());
		return 0;
	}

	shader = glCreateShader(type);
	if (shader == 0) {
		Debug::logError("ShaderLoad", ("Could not create shader " + fileName).c_str());
		return 0;
	}

	src = loadSource(filePath);
	if (src == nullptr) {
		glDeleteShader(shader);
		return 0;
	}

	glShaderSource(shader, 1, (const GLchar**) &src, NULL);
	glCompileShader(shader);

	free(src);
	src = nullptr;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	log = new char[logSize + 1];
	if (log == nullptr) {
		Debug::logError("ShaderLoad", ("Could not allocate memory for shader compilation log (" + fileName + ")").c_str());
		return 0;
	}
	memset(log, '\0', logSize + 1);

	glGetShaderInfoLog(shader, logSize, &logSize, log);

	std::string logStr = std::string(log);
	if (compileStatus != GL_TRUE) {
		Debug::logError("ShaderLoad", ("Could not compile shader " + fileName + ": " + logStr).c_str());
		delete[] log;
		glDeleteShader(shader);
		return 0;
	} else {
		Debug::log("ShaderLoad", ("Program " + path + " shader " + fileName + " compile success").c_str());
		if (logStr.length() > 0) {
			Debug::log("ShaderLoad", ("Compile result " + fileName + ":\n" + logStr).c_str());
		}
		delete[] log;
	}

	return shader;
}
