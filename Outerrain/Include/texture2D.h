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
	// Rendre mipmap param�trable pour la texture GL
	// Faire le destructeur qui lib�re la m�moire
	// Virer toutes les r�f�rences � Image:: ImageData:: et Texture::, MakeTexture etc..

	void* PixelBuffer() const;
	int Width() const;
	int Height() const;
	int BytesPerPixel() const;
};
