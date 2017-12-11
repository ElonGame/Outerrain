#pragma once

#include "GL/glew.h"
#include "image.h"

GLuint MakeTexture(const int unit, const ImageData& im, const GLenum texel_type = GL_RGBA);
GLuint ReadTexture(const int unit, const char *filename, const GLenum texel_type = GL_RGBA);

int MipLevels(const int width, const int height);
