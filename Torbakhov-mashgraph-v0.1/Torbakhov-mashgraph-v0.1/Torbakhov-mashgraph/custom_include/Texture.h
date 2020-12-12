#pragma once

#include "common.h"

class Texture {
	unsigned texture;

public:
	Texture() : texture(0)
	{
	}
	~Texture()
	{
		glDeleteTextures(1, &texture);
	}

	void loadTexture(const char* path);

	void init(int format1, GLsizei w, GLsizei h, GLenum format2, GLenum type);

	void initColor(GLsizei w = WIDTH, GLsizei h = HEIGHT)
	{
		init(GL_RGB, w, h, GL_RGB, GL_FLOAT);
	}
	void initDepth(GLsizei w = WIDTH, GLsizei h = HEIGHT)
	{
		init(GL_DEPTH_COMPONENT, w, h, GL_DEPTH_COMPONENT, GL_FLOAT);
	}

	void bind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	void unbind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned get() const
	{
		return texture;
	}
};