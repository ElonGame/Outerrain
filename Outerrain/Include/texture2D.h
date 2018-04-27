#pragma once
#include "vec.h"
#include "color.h"

#include <GL/glew.h>
#include <string>
#include <vector>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

class Texture2D
{
protected:
	std::vector<Color> data;
	int frameWidth;
	int frameHeight;
	int bitsPerPixel;
	int bytesPerPixel;

public:
	Texture2D(const std::string& filePath);
	Texture2D(int frameWidth, int frameHeight);
	~Texture2D();

	void SetPixel(int x, int y, const Color& c);
	Color Pixel(int x, int y) const;
	void* PixelBuffer() const;
	int Width() const;
	int GetValueBilinear() const;
	int BytesPerPixel() const;
	void Save(const std::string& filePath) const;

	GLuint GetGLTexture(int unit, bool mipmap) const;
	static GLuint MakeGLTexture(const std::string& filePath, int unit, bool mipmap);
};
