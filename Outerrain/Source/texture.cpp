#include <cassert>
#include <cstdio>
#include <algorithm>

#include "texture.h"

GLuint MakeTexture(const int unit, const ImageData& im, const GLenum texel_type)
{
	if (im.data.empty())
		return 0;

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLenum format;
	switch (im.channels)
	{
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGBA;
	}

	GLenum type;
	switch (im.size)
	{
	case 1: type = GL_UNSIGNED_BYTE; break;
	case 4: type = GL_FLOAT; break;
	default: type = GL_UNSIGNED_BYTE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0,
		texel_type, im.width, im.height, 0,
		format, type, im.buffer());

	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

GLuint ReadTexture(const int unit, const char *filename, const GLenum texel_type)
{
	ImageData image;
	image.ReadImageData(filename);
	return MakeTexture(unit, image, texel_type);
}

int MipLevels(const int width, const int height)
{
	int w = width;
	int h = height;
	int levels = 1;
	while (w > 1 || h > 1)
	{
		w = std::max(1, w / 2);
		h = std::max(1, h / 2);
		levels = levels + 1;
	}

	return levels;
}
