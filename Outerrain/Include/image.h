#ifndef _IMAGE_H
#define _IMAGE_H

#include <vector>
#include <cassert>

#include "color.h"
#include "GL\glew.h"

class Image
{
protected:
	std::vector<Color> data;
	int width;
	int height;
	int channels;

public:
	Image();
	Image(int w, int h, const Color& color = Color::Black());
	Image(const char*);

	Color& operator() (int x, int y);
	Color operator() (int x, int y) const;
	GLuint GetGLTexture(int unit, GLenum texel_type);
	const void* Buffer() const;
	void ReadImage(const char *filename, bool flipY);
	int WriteImage(const char *filename, bool flipY);
	int Width() const;
	int Height() const;
	std::size_t Size() const;
};

#endif
