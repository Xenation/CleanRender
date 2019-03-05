#include "FrameBuffer.h"

#include "Debug.h"
#include "Texture.h"
#include "GLUtils.h"



FrameBuffer::FrameBuffer(std::string name, uint width, uint height, uint samples)
	: name(name), width(width), height(height), samples(samples), attachmentCount(attachmentCount) {
	glGenFramebuffers(1, &fbo);
	std::string fullName = "Framebuffer " + name;
	glObjectLabel(GL_FRAMEBUFFER, fbo, fullName.size(), fullName.c_str());
}

FrameBuffer::~FrameBuffer() {
	if (attachments != nullptr) {
		for (unsigned int i = 0; i < attachmentCount; i++) {
			delete attachments[i].texture;
		}
		delete[] attachments;
	}
	glDeleteFramebuffers(1, &fbo);
}


FrameBuffer* FrameBuffer::copy() {
	FrameBuffer* fb = new FrameBuffer(name, width, height, samples);
	Attachment* fbAttachments = new Attachment[attachmentCount];
	for (uint i = 0; i < attachmentCount; i++) {
		fbAttachments[i] = attachments[i]; // Copies attachments definition
	}
	fb->createAttachments(attachmentCount, fbAttachments);
	return fb;
}

void FrameBuffer::createAttachments(uint count, Attachment* attachments) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	this->attachmentCount = count;
	this->attachments = attachments;
	for (uint i = 0; i < attachmentCount; i++) {
		createAttachment(i);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::logError("FrameBuffer", "Framebuffer " + glFramebufferStatusString(status));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::createAttachment(int index) {
	Attachment& attachment = attachments[index];
	Texture* texture = new Texture(name + "/Attachment/" + glAttachmentString(attachment.slot)); // TODO maybe add format to name?
	texture->createEmpty(width, height, attachment.format, glGetDefaultInternalFormat(attachment.format), samples, true, false); // TODO implement a way to manually specify internal format
	texture->uploadToGL();
	attachment.texture = texture;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.slot, (samples == 0) ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, texture->getTextureID(), 0);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blitTo(FrameBuffer* frameBuffer) {
	GLuint otherFBO = 0; // 0 to blit to default when target framebuffer is null
	if (frameBuffer != nullptr) { // Blit to default
		otherFBO = frameBuffer->fbo;
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_LINEAR);
}
