#pragma once
#include <string>
#include "HollowSet.h"

class SpecializedShaderProgram;

class RenderPass {
public:
	std::string name;
	HollowSet<SpecializedShaderProgram*> programs;

	RenderPass(const char* name);
	~RenderPass();

	virtual void prepare();
	virtual void render();
};

class RenderPassOpaque : public RenderPass {
public:
	RenderPassOpaque(const char* name);
	~RenderPassOpaque();

	virtual void prepare() override;
};

class RenderPassTransparent : public RenderPass {
public:
	RenderPassTransparent(const char* name);
	~RenderPassTransparent();

	virtual void prepare() override;
};

