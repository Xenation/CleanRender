#include "RenderPass.h"

#include <gl3w.h>



RenderPass::RenderPass(const char* name) : name(name), programs(4, 16) {}

RenderPass::~RenderPass() {}


void RenderPass::preparePass() {
	
}



RenderPassOpaque::RenderPassOpaque(const char* name) : RenderPass(name) {}

RenderPassOpaque::~RenderPassOpaque() {}

void RenderPassOpaque::preparePass() {
	glDisable(GL_BLEND);
}



RenderPassTransparent::RenderPassTransparent(const char* name) : RenderPass(name) {}

RenderPassTransparent::~RenderPassTransparent() {}

void RenderPassTransparent::preparePass() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
