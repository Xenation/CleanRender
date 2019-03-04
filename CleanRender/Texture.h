#pragma once
#include <gl3w.h>
class Texture {
public:
	Texture();
	Texture(const Texture&) = delete;
	~Texture();

	Texture* copy();
	void createEmpty(unsigned int width, unsigned int height, GLenum format, unsigned int mulisamples = 0, bool noalloc = false);
	void loadFromFile(const char* filePath);
	void setTextureData(unsigned char* data, unsigned int dataSize, GLenum format);
	void deleteLocal();
	void uploadToGL();
	void deleteFromGL();
	inline bool isLoadedToGL() const { return loadedToGL; }
	inline bool isCachedInLocal() const { return cachedInLocal; }
	inline GLuint getTextureID() const { return textureID; }

private:
	GLuint textureID = 0;
	bool loadedToGL = false;
	bool cachedInLocal = false;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* textureData = nullptr;
	unsigned int textureDataSize = 0;
	GLenum pixelFormat = GL_RGBA;
	GLenum target = GL_TEXTURE_2D;
	unsigned int samples = 0; // 0: no multisamples
};

