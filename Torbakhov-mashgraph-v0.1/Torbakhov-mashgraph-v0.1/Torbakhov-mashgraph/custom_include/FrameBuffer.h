#pragma once

#include "common.h"
#include "Texture.h"


#define SHADOWMAP_WIDTH 2048
#define SHADOWMAP_HEIGHT 2048


class Framebuffer {
	unsigned fbo;
public:
	Framebuffer() : fbo(0)
	{
	}
	~Framebuffer()
	{
		glDeleteFramebuffers(1, &fbo);
	}

	void initializeTextures(Texture* depthTexture, Texture* texture0 = NULL, Texture* texture1 = NULL);

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};