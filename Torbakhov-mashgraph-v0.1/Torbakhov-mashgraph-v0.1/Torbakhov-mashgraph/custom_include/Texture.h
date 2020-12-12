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