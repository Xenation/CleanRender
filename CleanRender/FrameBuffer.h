#pragma once
#include <gl3w.h>
#include "XMath.h"

class Texture;

class FrameBuffer {
public:
	Color clearColor = Color::clear;

	FrameBuffer(unsigned int width, unsigned int height, unsigned int attachmentCount = 1, unsigned int samples = 0);
	FrameBuffer(const FrameBuffer&) = delete;
	~FrameBuffer();

	FrameBuffer* copy();
	void createColorAttachment(int index, GLenum format);
	void createDepthAttachment();
	void createDepthStencilAttachment();
	
	void bind();
	void unbind();
	void blitTo(FrameBuffer* frameBuffer);

private:
	GLuint fbo = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int samples = 0;
	GLenum* attachmentFormats = nullptr;
	GLenum* attachmentPoints = nullptr;
	Texture** attachments = nullptr;
	unsigned int attachmentCount = 0;

	void createAttachment(int index, GLenum format, GLenum attachPoint);
};

