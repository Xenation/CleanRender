#include "ShaderProgram.h"

#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include "Pipeline.h"
#include "Debug.h"
#include "Material.h"
#include "RenderPass.h"

#define SHADER_DIRECTORY "res/shaders/"
#define SHADER_CODE_ERROR_VS "#version 420 core\n//meta pass opaque\nlayout (location = 0) in vec3 vp;\nlayout (std140, binding = 1) uniform CameraMatrices {\nmat4x4 projectionMatrix;\nmat4x4 viewMatrix;\n};\nlayout (std140, binding = 10) uniform Material {\nmat4x4 modelMatrix;\n};\nvoid main() { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0); }\n\0"
#define SHADER_CODE_ERROR_FS "#version 420 core\nout vec4 fc;\nvoid main() { fc = vec4(1.0, 0.0, 1.0, 1.0); }\n\0"


namespace fs = std::filesystem;

const char ShaderMetaInfo::separators[SHADER_META_SEPARATOR_COUNT]{' ', '\t', '\n', '(', ')', ',', ';'};

ShaderMetaInfo::ShaderMetaInfo() {}

ShaderMetaInfo::ShaderMetaInfo(RenderPass* pass, GLSLType* fieldTypes, const char** fieldNames, unsigned int fieldCount)
	: renderPass(pass), materialFieldTypes(fieldTypes), materialFieldNames(fieldNames), materialFieldCount(fieldCount) {}

ShaderMetaInfo::~ShaderMetaInfo() {
	if (materialFieldTypes != nullptr) {
		delete[] materialFieldTypes;
		materialFieldTypes = nullptr;
	}
	if (materialFieldNames != nullptr) {
		for (unsigned int i = 0; i < materialFieldCount; i++) {
			delete[] materialFieldNames[i];
		}
		delete[] materialFieldNames;
		materialFieldNames = nullptr;
	}
}

ShaderMetaInfo* ShaderMetaInfo::extractFromSource(const char* src) {
	char* passName = new char[1]{'\0'};
	SimpleList<GLSLType> fieldTypesList(1, 1);
	SimpleList<char*> fieldNamesList(1, 1);

	bool passFound = false;
	bool materialFound = false;

	char* fieldName;
	char* word = new char[1]{'\0'};
	int wordSize;
	int currentIndex = 0;
	while (nextWord(src, currentIndex, word, wordSize)) {
		if (compareWords(word, wordSize, "//meta")) {
			if (!nextWord(src, currentIndex, word, wordSize)) break;
			if (compareWords(word, wordSize, "pass")) {
				if (!nextWord(src, currentIndex, word, wordSize)) break;
				copyWord(word, wordSize, passName);
				passFound = true;
			}
		} else if (compareWords(word, wordSize, "layout")) {
			if (!nextWord(src, currentIndex, word, wordSize)) break;
			if (compareWords(word, wordSize, "std140") || compareWords(word, wordSize, "shared") || compareWords(word, wordSize, "packed")) {
				if (!nextWord(src, currentIndex, word, wordSize)) break;
				if (compareWords(word, wordSize, "binding")) {
					if (!nextWord(src, currentIndex, word, wordSize)) break;
					if (compareWords(word, wordSize, "=")) {
						if (!nextWord(src, currentIndex, word, wordSize)) break;
						if (compareWords(word, wordSize, "10")) {
							if (!nextWord(src, currentIndex, word, wordSize)) break; //uniform
							if (!nextWord(src, currentIndex, word, wordSize)) break; //Material
							if (!nextWord(src, currentIndex, word, wordSize)) break; //{
							materialFound = true;
							while (true) {
								if (!nextWord(src, currentIndex, word, wordSize)) break;
								if (compareWords(word, wordSize, "}")) break;
								fieldTypesList.add(getTypeFromWord(word, wordSize));
								if (!nextWord(src, currentIndex, word, wordSize)) break;
								if (compareWords(word, wordSize, "}")) break;
								copyWord(word, wordSize, fieldName);
								fieldNamesList.add(fieldName);
							}
						}
					}
				}
			}
		}
	}

	RenderPass* pass = nullptr;
	if (passFound) {
		pass = ShaderProgram::defaultPipeline->getRenderPass(std::string(passName));
		delete[] passName;
	}

	GLSLType* fieldTypes = nullptr;
	const char** fieldNames = nullptr;
	unsigned int fieldCount = 0;
	if (materialFound) {
		fieldTypes = new GLSLType[fieldTypesList.count];
		for (int i = 0; i < fieldTypesList.count; i++) {
			fieldTypes[i] = fieldTypesList[i];
		}
		fieldNames = new const char*[fieldNamesList.count];
		for (int i = 0; i < fieldNamesList.count; i++) {
			fieldNames[i] = fieldNamesList[i];
		}

		fieldCount = fieldNamesList.count;
	}

	return new ShaderMetaInfo(pass, fieldTypes, fieldNames, fieldCount);
}

bool ShaderMetaInfo::isSeparator(const char c) {
	for (int i = 0; i < SHADER_META_SEPARATOR_COUNT; i++) {
		if (c == separators[i]) return true;
	}
	return false;
}

bool ShaderMetaInfo::nextWord(const char* src, int& index, char*& word, int& wordSize) {
	delete[] word;

	// Trim start separators
	int startIndex = index;
	while (isSeparator(src[startIndex])) {
		if (src[startIndex] == '\0') return false;
		startIndex++;
	}

	// Find next separator
	int sepIndex = startIndex;
	while (!isSeparator(src[sepIndex])) {
		if (src[sepIndex] == '\0') return false;
		sepIndex++;
	}

	// Allocate and copy word
	wordSize = sepIndex - startIndex;
	word = new char[wordSize + 1];
	for (int i = 0; i < wordSize; i++) {
		word[i] = src[startIndex + i];
	}
	word[wordSize] = '\0';

	index = startIndex + wordSize;
	return true;
}

bool ShaderMetaInfo::compareWords(char* word, int wordSize, const char* refWord) {
	int refWordSize = 0;
	while (refWord[refWordSize] != '\0') {
		refWordSize++;
	}
	if (refWordSize != wordSize) return false;
	for (int i = 0; i < wordSize; i++) {
		if (word[i] != refWord[i]) return false;
	}
	return true;
}

void ShaderMetaInfo::copyWord(char* word, int wordSize, char*& nWord) {
	nWord = new char[wordSize + 1];
	for (int i = 0; i < wordSize; i++) {
		nWord[i] = word[i];
	}
	nWord[wordSize] = '\0';
}

GLSLType ShaderMetaInfo::getTypeFromWord(char* word, int wordSize) {
	if (compareWords(word, wordSize, "bool")) {
		return GLSL_BOOL;
	} else if (compareWords(word, wordSize, "int")) {
		return GLSL_INT;
	} else if (compareWords(word, wordSize, "uint")) {
		return GLSL_UINT;
	} else if (compareWords(word, wordSize, "float")) {
		return GLSL_FLOAT;
	} else if (compareWords(word, wordSize, "double")) {
		return GLSL_DOUBLE;
	} else if (compareWords(word, wordSize, "bvec2")) {
		return GLSL_BVEC2;
	} else if (compareWords(word, wordSize, "ivec2")) {
		return GLSL_IVEC2;
	} else if (compareWords(word, wordSize, "uvec2")) {
		return GLSL_UVEC2;
	} else if (compareWords(word, wordSize, "vec2")) {
		return GLSL_VEC2;
	} else if (compareWords(word, wordSize, "dvec2")) {
		return GLSL_DVEC2;
	} else if (compareWords(word, wordSize, "bvec3")) {
		return GLSL_BVEC3;
	} else if (compareWords(word, wordSize, "ivec3")) {
		return GLSL_IVEC3;
	} else if (compareWords(word, wordSize, "uvec3")) {
		return GLSL_UVEC3;
	} else if (compareWords(word, wordSize, "vec3")) {
		return GLSL_VEC3;
	} else if (compareWords(word, wordSize, "dvec3")) {
		return GLSL_DVEC3;
	} else if (compareWords(word, wordSize, "bvec4")) {
		return GLSL_BVEC4;
	} else if (compareWords(word, wordSize, "ivec4")) {
		return GLSL_IVEC4;
	} else if (compareWords(word, wordSize, "uvec4")) {
		return GLSL_UVEC4;
	} else if (compareWords(word, wordSize, "vec4")) {
		return GLSL_VEC4;
	} else if (compareWords(word, wordSize, "dvec4")) {
		return GLSL_DVEC4;
	} else if (compareWords(word, wordSize, "mat2x2")) {
		return GLSL_MAT2x2;
	} else if (compareWords(word, wordSize, "mat2x3")) {
		return GLSL_MAT2x3;
	} else if (compareWords(word, wordSize, "mat2x4")) {
		return GLSL_MAT2x4;
	} else if (compareWords(word, wordSize, "mat3x2")) {
		return GLSL_MAT3x2;
	} else if (compareWords(word, wordSize, "mat3x3")) {
		return GLSL_MAT3x3;
	} else if (compareWords(word, wordSize, "mat3x4")) {
		return GLSL_MAT3x4;
	} else if (compareWords(word, wordSize, "mat4x2")) {
		return GLSL_MAT4x2;
	} else if (compareWords(word, wordSize, "mat4x3")) {
		return GLSL_MAT4x3;
	} else if (compareWords(word, wordSize, "mat4x4")) {
		return GLSL_MAT4x4;
	} else if (compareWords(word, wordSize, "mat2")) {
		return GLSL_MAT2;
	} else if (compareWords(word, wordSize, "mat3")) {
		return GLSL_MAT3;
	} else if (compareWords(word, wordSize, "mat4")) {
		return GLSL_MAT4;
	} else if (compareWords(word, wordSize, "dmat2x2")) {
		return GLSL_DMAT2x2;
	} else if (compareWords(word, wordSize, "dmat2x3")) {
		return GLSL_DMAT2x3;
	} else if (compareWords(word, wordSize, "dmat2x4")) {
		return GLSL_DMAT2x4;
	} else if (compareWords(word, wordSize, "dmat3x2")) {
		return GLSL_DMAT3x2;
	} else if (compareWords(word, wordSize, "dmat3x3")) {
		return GLSL_DMAT3x3;
	} else if (compareWords(word, wordSize, "dmat3x4")) {
		return GLSL_DMAT3x4;
	} else if (compareWords(word, wordSize, "dmat4x2")) {
		return GLSL_DMAT4x2;
	} else if (compareWords(word, wordSize, "dmat4x3")) {
		return GLSL_DMAT4x3;
	} else if (compareWords(word, wordSize, "dmat4x4")) {
		return GLSL_DMAT4x4;
	} else if (compareWords(word, wordSize, "dmat2")) {
		return GLSL_DMAT2;
	} else if (compareWords(word, wordSize, "dmat3")) {
		return GLSL_DMAT3;
	} else if (compareWords(word, wordSize, "dmat4")) {
		return GLSL_DMAT4;
	}
	return GLSL_UNKNOWN;
}



Pipeline* ShaderProgram::defaultPipeline = nullptr;
ShaderProgram* ShaderProgram::errorShader = nullptr;
unsigned int ShaderProgram::shaderCount = 0;
ShaderProgram** ShaderProgram::shaders = nullptr;

void ShaderProgram::initializeAll() {
	// Loading Error shader
	errorShader = new ShaderProgram("Error");
	errorShader->load(errorShader->loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_VS), 0, errorShader->loadShaderFromSource(GL_FRAGMENT_SHADER, SHADER_CODE_ERROR_FS));
	Material::errorMaterial = new Material(errorShader);

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
	for (unsigned int i = 0; i < shaderCount; i++) {
		ShaderProgram* sp = shaders[i];
		if (shaders[i]->path == name) {
			return shaders[i];
		}
	}
	Debug::log("ShaderProgram", ("Could not find program named " + name).c_str());
	return nullptr;
}


ShaderProgram::ShaderProgram(std::string path) : materials(4, 16) {
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
	if (info->renderPass != nullptr) {
		info->renderPass->programs.remove(idInPass);
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

void ShaderProgram::setTextureUnit(GLuint location, unsigned int unit) {
	glUniform1ui(location, unit);
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
	if (info->renderPass != nullptr) {
		idInPass = info->renderPass->programs.add(this);
	}
	Debug::log("ShaderProgram", "Program load and link done!");
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
			delete[] log;
		}
		if (type == GL_VERTEX_SHADER) {
			info = ShaderMetaInfo::extractFromSource(src);
		}
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
