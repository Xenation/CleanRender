#pragma once
#include <string>
#include <filesystem>
#include "ShaderMetaInfo.h"

class ShaderReader {
public:
	struct SpecializedShaderSource {
		union {
			struct {
				char* version;
				char* defines;
				char* noVersionSource;
			};
			const char* source[3];
		};

	public:
		SpecializedShaderSource() : version(nullptr), defines(nullptr), noVersionSource(nullptr) {}
		~SpecializedShaderSource() {
			delete[] version;
			delete[] defines;
			delete[] noVersionSource;
		}
	};

	ShaderProgramMetaInfo* programInfo; // !! Is not deleted by this class
	SpecializedShaderSource* vsSources = nullptr;
	SpecializedShaderSource* tcsSources = nullptr;
	SpecializedShaderSource* tesSources = nullptr;
	SpecializedShaderSource* gsSources = nullptr;
	SpecializedShaderSource* fsSources = nullptr;

	ShaderReader(std::filesystem::path shaderDir);
	ShaderReader(const ShaderReader&) = delete;
	~ShaderReader();

	bool read();
	bool generate();

private:
	struct ShaderFile {
		std::filesystem::path filePath;
		GLenum shaderType;
		std::string version;
		std::string rawSource;
		ShaderFileMetaInfo info;
	};

	std::filesystem::path shaderDirectoryPath;
	ShaderFile* vs = nullptr;
	ShaderFile* tcs = nullptr;
	ShaderFile* tes = nullptr;
	ShaderFile* gs = nullptr;
	ShaderFile* fs = nullptr;

	std::string readRawSource(std::filesystem::path filePath);
	ShaderFile* createShaderFileInfo(std::filesystem::path filePath, GLenum type, std::string rawSource);
	void removeComments(ShaderFile* shaderFile);
	void extractMetaInfo(ShaderFile* shaderFile);
	void mergeMetaInfo();
	void generateSpecializedSources(ShaderFile* shaderFile, SpecializedShaderSource*& specializedSources);
};

