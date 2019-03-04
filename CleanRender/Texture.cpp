#include "Texture.h"

#include <vector>
#include <lodepng.h>
#include "Debug.h"
#include "GLUtils.h"



Texture::Texture() {}

Texture::~Texture() {
	if (cachedInLocal) {
		deleteLocal();
	}
	if (loadedToGL) {
		deleteFromGL();
	}
}


Texture* Texture::copy() {
	Texture* texture = new Texture();
	if (textureData != nullptr) {
		texture->width = width;
		texture->height = height;
		texture->target = target;
		// samples are 0 if texture has local data
		texture->setTextureData(new unsigned char[textureDataSize], textureDataSize, pixelFormat);
	} else {
		texture->createEmpty(width, height, pixelFormat, samples, true);
	}
	if (loadedToGL) {
		texture->uploadToGL();
	}
	return texture;
}

void Texture::createEmpty(unsigned int width, unsigned int height, GLenum format, unsigned int multisamples, bool noalloc) {
	if (cachedInLocal) {
		deleteLocal();
	}
	pixelFormat = format;
	this->width = width;
	this->height = height;
	if (multisamples != 0) {
		target = GL_TEXTURE_2D_MULTISAMPLE;
	} else {
		if (!noalloc) {
			textureDataSize = glFormatByteSize(pixelFormat, width * height);
			textureData = new unsigned char[textureDataSize];
		}
	}
}

void Texture::loadFromFile(const char* filePath) {
	unsigned char* pngData;
	size_t pngDataSize;

	LodePNGState state;
	unsigned int error;
	error = lodepng_load_file(&pngData, &pngDataSize, filePath);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png load error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}
	error = lodepng_inspect(&width, &height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png inspect error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	switch (state.info_png.color.colortype) {
	case LodePNGColorType::LCT_GREY:
		switch (state.info_png.color.bitdepth) {
		case 1:
		case 2:
		case 4:
		case 8:
			pixelFormat = GL_R8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelFormat = GL_R16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_GREY_ALPHA:
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelFormat = GL_RG8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelFormat = GL_RG16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_PALETTE:
		Debug::log("Texture", "Unable to load texture from file: palette png format not supported!");
		break;
	case LodePNGColorType::LCT_RGB:
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelFormat = GL_RGB8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelFormat = GL_RGB16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_RGBA:
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelFormat = GL_RGBA8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelFormat = GL_RGBA16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	}
	state.info_raw.colortype = state.info_png.color.colortype;

	error = lodepng_decode(&textureData, &width, &height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png decode error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	textureDataSize = glFormatByteSize(pixelFormat, width * height);

	delete[] pngData;

	cachedInLocal = true;
}

void Texture::setTextureData(unsigned char* data, unsigned int dataSize, GLenum format) {
	textureData = data;
	textureDataSize = dataSize;
	pixelFormat = format;
	cachedInLocal = true;
}

void Texture::deleteLocal() {
	if (textureData != nullptr) {
		delete[] textureData;
		textureData = nullptr;
	}
	cachedInLocal = false;
}

void Texture::uploadToGL() {
	glGenTextures(1, &textureID);

	glBindTexture(target, textureID);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (target == GL_TEXTURE_2D_MULTISAMPLE) {
		glTexImage2DMultisample(target, samples, pixelFormat, width, height, GL_FALSE);
	} else {
		glTexImage2D(target, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, textureData);
	}
	glGenerateMipmap(target);

	glBindTexture(target, 0);

	loadedToGL = true;
}

void Texture::deleteFromGL() {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	loadedToGL = false;
}
