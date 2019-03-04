#include "FrameBuffer.h"

#include "Texture.h"



FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int attachmentCount, unsigned int samples)
	: width(width), height(height), samples(samples), attachmentCount(attachmentCount), attachments(new Texture*[attachmentCount]), attachmentFormats(new GLenum[attachmentCount]), attachmentPoints(new GLenum[attachmentCount]) {
	glGenFramebuffers(1, &fbo);
}

FrameBuffer::~FrameBuffer() {
	if (attachments != nullptr) {
		for (unsigned int i = 0; i < attachmentCount; i++) {
			delete attachments[i];
		}
		delete[] attachments;
		delete[] attachmentFormats;
		delete[] attachmentPoints;
	}
	glDeleteFramebuffers(1, &fbo);
}


FrameBuffer* FrameBuffer::copy() {
	FrameBuffer* fb = new FrameBuffer(width, height, attachmentCount, samples);
	for (unsigned int i = 0; i < attachmentCount; i++) {
		fb->createAttachment(i, attachmentFormats[i], attachmentPoints[i]);
	}
	return fb;
}

void FrameBuffer::createColorAttachment(int index, GLenum format) {
	createAttachment(index, format, GL_COLOR_ATTACHMENT0 + index);
}

void FrameBuffer::createDepthAttachment() {
	createAttachment(attachmentCount - 1, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
}

void FrameBuffer::createDepthStencilAttachment() {
	createAttachment(attachmentCount - 1, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
}

void FrameBuffer::createAttachment(int index, GLenum format, GLenum attachPoint) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	Texture* texture = new Texture();
	texture->createEmpty(width, height, format, samples, true);
	texture->uploadToGL();
	attachments[index] = texture;
	attachmentFormats[index] = format;
	attachmentPoints[index] = attachPoint;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, (samples == 0) ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, texture->getTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blitTo(FrameBuffer* frameBuffer) {
	if (frameBuffer == nullptr) { // Blit to default
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, attachments[0]->getTextureID());
		glBindTextureUnit(GL_TEXTURE0, attachments[0]->getTextureID());
	} else {

	}
}
