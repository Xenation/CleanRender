#pragma once
#include <string>
#include "HollowSet.h"

class ShaderProgram;

class RenderPass {
public:
	std::string name;
	HollowSet<ShaderProgram*> programs;

	RenderPass(const char* name);
	~RenderPass();

	virtual void preparePass();
};

class RenderPassOpaque : public RenderPass {
public:
	RenderPassOpaque(const char* name);
	~RenderPassOpaque();

	virtual void preparePass();
};

class RenderPassTransparent : public RenderPass {
public:
	RenderPassTransparent(const char* name);
	~RenderPassTransparent();

	virtual void preparePass();
};

