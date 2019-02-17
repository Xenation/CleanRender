#pragma once
#include <gl3w.h>
class Texture {
public:
	Texture();
	~Texture();

	void setTextureFormat(GLenum format);
	void loadFromFile(const char* filePath);
	void setTextureData(unsigned char* data);
	void deleteLocal();
	void uploadToGL();
	void deleteFromGL();
	inline bool isLoadedToGL() const { return loadedToGL; }
	inline bool isCachedInLocal() const { return cachedInLocal; }

private:
	GLuint textureID = 0;
	bool loadedToGL = false;
	bool cachedInLocal = false;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* textureData = nullptr;
	GLenum pixelFormat = GL_RGBA;
};

