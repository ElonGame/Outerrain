#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <SDL2/SDL_image.h>

#include "image.h"


Image::Image() : data(), width(0), height(0) 
{
}

Image::Image(int w, int h, const Color& color) : data(w*h, color), width(w), height(h) 
{
}

Image::Image(const char* filename)
{
	ReadImage(filename, false);
}

GLuint Image::GetGLTexture(int unit, GLenum texel_type)
{
	if (data.empty())
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
	switch (channels)
	{
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGBA;
	}

	GLenum type;
	int size = 1; // Todo : find what exactly is size.
	switch (size)
	{
	case 1: type = GL_UNSIGNED_BYTE; break;
	case 4: type = GL_FLOAT; break;
	default: type = GL_UNSIGNED_BYTE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, texel_type, width, height, 0, format, type, Buffer());

	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

void Image::ReadImage(const char *filename, bool flipY)
{
	SDL_Surface *surface = IMG_Load(filename);
	if (surface == NULL)
	{
		printf("[error] loading image '%s'... sdl_image failed.\n", filename);
		return;
	}

	const SDL_PixelFormat format = *surface->format;
	if (format.BitsPerPixel != 24 && format.BitsPerPixel != 32)
	{
		printf("[error] loading image '%s'... format failed. (bpp %d)\n", filename, format.BitsPerPixel);
		SDL_FreeSurface(surface);
		return;
	}

	width = surface->w;
	height = surface->h;
	channels = (format.BitsPerPixel == 32) ? 4 : 3;
	data.resize(width * height, Color::Black());

	if (format.BitsPerPixel == 32)
	{
		int py = 0;
		for (int y = height - 1; y >= 0; y--, py++)
		{
			Uint8 *pixel = (Uint8 *)surface->pixels + py * surface->pitch;

			for (int x = 0; x < width; x++)
			{
				float r = static_cast<float>(pixel[format.Rshift / 8]);
				float g = static_cast<float>(pixel[format.Gshift / 8]);
				float b = static_cast<float>(pixel[format.Bshift / 8]);
				float a = static_cast<float>(pixel[format.Ashift / 8]);

				int index = (flipY ? height - y - 1 : y) * width + x;
				data[index] = Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
				pixel = pixel + format.BytesPerPixel;
			}
		}
	}
	else if (format.BitsPerPixel == 24)
	{
		int py = 0;
		for (int y = height - 1; y >= 0; y--, py++)
		{
			Uint8 *pixel = (Uint8 *)surface->pixels + py * surface->pitch;

			for (int x = 0; x < surface->w; x++)
			{
				float r = static_cast<float>(pixel[format.Rshift / 8]);
				float g = static_cast<float>(pixel[format.Gshift / 8]);
				float b = static_cast<float>(pixel[format.Bshift / 8]);

				int index = (flipY ? height - y - 1 : y) * width + x;
				data[index] = Color(r / 255.f, g / 255.f, b / 255.f);
				pixel = pixel + format.BytesPerPixel;
			}
		}
	}

	SDL_FreeSurface(surface);
}

int Image::WriteImage(const char *filename, bool flipY)
{
	if (std::string(filename).rfind(".png") == std::string::npos && std::string(filename).rfind(".bmp") == std::string::npos)
	{
		printf("[error] writing color image '%s'... not a .png / .bmp image.\n", filename);
		return -1;
	}

	// flip de l'image : Y inverse entre GL et BMP
	std::vector<Uint8> flip(width * height * 4);

	int p = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			Color color = this->operator()(x, flipY ? height - y - 1 : y);
			Uint8 r = (Uint8)std::min(std::floor(color.r * 255.f), 255.f);
			Uint8 g = (Uint8)std::min(std::floor(color.g * 255.f), 255.f);
			Uint8 b = (Uint8)std::min(std::floor(color.b * 255.f), 255.f);
			Uint8 a = (Uint8)std::min(std::floor(color.a * 255.f), 255.f);

			flip[p] = r;
			flip[p + 1] = g;
			flip[p + 2] = b;
			flip[p + 3] = a;
			p = p + 4;
		}

	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)&flip.front(), width, height,
		32, width * 4,
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000
	);

	int code = -1;
	if (std::string(filename).rfind(".png") != std::string::npos)
		code = IMG_SavePNG(surface, filename);
	else if (std::string(filename).rfind(".bmp") != std::string::npos)
		code = SDL_SaveBMP(surface, filename);

	SDL_FreeSurface(surface);
	if (code < 0)
		printf("[error] writing color image '%s'...\n%s\n", filename, SDL_GetError());
	return code;
}

Color& Image::operator() (int x, int y)
{
	std::size_t offset = y * width + x;
	assert(offset < data.size());
	return data[offset];
}

Color Image::operator() (int x, int y) const
{
	std::size_t offset = y * width + x;
	assert(offset < data.size());
	return data[offset];
}

const void* Image::Buffer() const
{
	assert(!data.empty());
	return &data.front();
}

int Image::Width() const
{ 
	return width;
}

int Image::Height() const
{ 
	return height;
}

std::size_t Image::Size() const
{ 
	return width * height; 
}