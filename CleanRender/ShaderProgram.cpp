#include "ShaderProgram.h"

#include <filesystem>
#include "Debug.h"
#define SHADER_DIRECTORY "res/shaders/"



namespace fs = std::filesystem;

unsigned int ShaderProgram::shaderCount = 0;
ShaderProgram** ShaderProgram::shaders = nullptr;

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


ShaderProgram::ShaderProgram(std::string path) {
	this->path = path;
}

ShaderProgram::~ShaderProgram() {

}

void ShaderProgram::load() {
	
}

void ShaderProgram::reload() {
	if (!loaded) return;
}

char* ShaderProgram::loadSource(const char* fileName) {
	char* src = nullptr;
	FILE* fp = nullptr;
	long size;
	long i;

	fopen_s(&fp, fileName, "r");
	if (fp == nullptr) {
		Debug::logError("ShaderLoad", ("Could not load shader source file " + std::string(fileName)).c_str());
		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	src = (char*) malloc(size + 1);
	if (src == nullptr) {
		fclose(fp);
		Debug::logError("ShaderLoad", ("Could not allocate memory for shader source file " + std::string(fileName)).c_str());
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
	const char* filePath = (path + "/" + fileName).c_str();

	FILE* file;
	if (fopen_s(&file, filePath, "r") == 0) {
		fclose(file);
	} else {
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
		delete[] log;
		glDeleteShader(shader);
		return 0;
	} else {
		Debug::log("ShaderLoad", ("Shader " + fileName + " compile success").c_str());
		std::string logStr = std::string(log);
		if (logStr.length() > 0) {
			Debug::log("ShaderLoad", ("Compile result " + fileName + ": " + logStr).c_str());
		}
		delete[] log;
	}

	return shader;
}
