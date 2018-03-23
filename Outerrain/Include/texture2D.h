#pragma once
#include "vec.h"
#include "color.h"

#include <string>
#include <vector>
#include <SDL2/SDL_image.h>

class Texture2D
{
protected:
	SDL_Surface* tex;

public:
	Texture2D(const std::string& filePath);

	// TODO :
	// GetGLTexture()
	// Pixel(int, int)
	// Rendre mipmap paramétrable pour la texture GL
	// Faire le destructeur qui libère la mémoire
	// Virer toutes les références à Image:: ImageData:: et Texture::, MakeTexture etc..

	void* PixelBuffer() const;
	int Width() const;
	int Height() const;
	int BytesPerPixel() const;
};
