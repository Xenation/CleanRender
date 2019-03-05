#pragma once
#include <string>
#include <gl3w.h>
#include "XTypes.h"
#include "XMath.h"

class Texture;

class FrameBuffer {
public:
	struct Attachment {
		GLenum slot;
		GLenum format;
		Texture* texture;

	public:
		Attachment() : slot(0), format(GL_RGBA), texture(nullptr) {}
		Attachment(GLenum attachPoint, GLenum format) : slot(attachPoint), format(format), texture(nullptr) {}
	};

	Color clearColor = Color::clear;

	FrameBuffer(std::string name, uint width, uint height, uint samples = 0);
	FrameBuffer(const FrameBuffer&) = delete;
	~FrameBuffer();

	FrameBuffer* copy();
	void createAttachments(uint count, Attachment* attachments);
	
	void bind();
	void unbind();
	void blitTo(FrameBuffer* frameBuffer);

private:
	std::string name;
	GLuint fbo = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int samples = 0;
	Attachment* attachments = nullptr;
	unsigned int attachmentCount = 0;

	void createAttachment(int index);
};

