#include "texture2D.h"
#include <cassert>

Texture2D::Texture2D(const std::string& filePath)
{
	tex = IMG_Load(filePath.c_str());
	if (tex == NULL)
	{
		std::cout << "Error loading image " << filePath << " - SDL_Image failed" << std::endl;
	}
}

void* Texture2D::PixelBuffer() const
{
	assert(tex != NULL);
	return tex->pixels;
}

int Texture2D::Width() const
{
	assert(tex != NULL);
	return tex->w;
}

int Texture2D::Height() const
{
	assert(tex != NULL);
	return tex->h;
}

int Texture2D::BytesPerPixel() const
{
	assert(tex != NULL);
	return tex->format->BytesPerPixel;
}