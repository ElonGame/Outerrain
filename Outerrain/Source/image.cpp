#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <SDL2/SDL_image.h>

#include "image.h"


void Image::ReadImage(const char *filename)
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
	data.resize(width * height, Color::Black());

	if (format.BitsPerPixel == 32)
	{
		int py = 0;
		for (int y = height - 1; y >= 0; y--, py++)
		{
			Uint8 *pixel = (Uint8 *)surface->pixels + py * surface->pitch;

			for (int x = 0; x < width; x++)
			{
				Uint8 r = pixel[format.Rshift / 8];
				Uint8 g = pixel[format.Gshift / 8];
				Uint8 b = pixel[format.Bshift / 8];
				Uint8 a = pixel[format.Ashift / 8];

				int index = y * width + x;
				data[index] = Color(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, static_cast<float>(a) / 255.f);
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
				const Uint8 r = pixel[format.Rshift / 8];
				const Uint8 g = pixel[format.Gshift / 8];
				const Uint8 b = pixel[format.Bshift / 8];

				int index = y * width + x;
				data[index] = Color(static_cast<float>(r)/ 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f);
				pixel = pixel + format.BytesPerPixel;
			}
		}
	}

	SDL_FreeSurface(surface);
}

int Image::WriteImage(const char *filename)
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
			Color color = this->operator()(x, height - y - 1);
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

	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)&flip.front(), width, height,
		32, width * 4,
#if 0
		0xFF000000,
		0x00FF0000,
		0x0000FF00,
		0x000000FF
#else
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000
#endif
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

void ImageData::ReadImageData(const char *filename)
{
	SDL_Surface *surface = IMG_Load(filename);
	if (surface == NULL)
	{
		printf("[error] loading image '%s'... sdl_image failed.\n%s\n", filename, SDL_GetError());
		return;
	}

	const SDL_PixelFormat format = *surface->format;
	if (format.BitsPerPixel != 24 && format.BitsPerPixel != 32)
	{
		printf("[error] loading image '%s'... format failed. (bpp %d)\n", filename, format.BitsPerPixel);
		SDL_FreeSurface(surface);
		return;
	}

	int c = (format.BitsPerPixel == 32) ? 4 : 3;

	width = surface->w;
	height = surface->h;
	channels = c;
	size = 1;
	data.resize(width * height * c * size, 0);

	if (format.BitsPerPixel == 32)
	{
		int py = 0;
		for (int y = height - 1; y >= 0; y--, py++)
		{
			Uint8 *pixel = (Uint8 *)surface->pixels + py * surface->pitch;

			for (int x = 0; x < width; x++)
			{
				Uint8 r = pixel[format.Rshift / 8];
				Uint8 g = pixel[format.Gshift / 8];
				Uint8 b = pixel[format.Bshift / 8];
				Uint8 a = pixel[format.Ashift / 8];

				std::size_t off = offset(x, y);
				data[off] = r;
				data[off + 1] = g;
				data[off + 2] = b;
				data[off + 3] = a;
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
				const Uint8 r = pixel[format.Rshift / 8];
				const Uint8 g = pixel[format.Gshift / 8];
				const Uint8 b = pixel[format.Bshift / 8];

				std::size_t off = offset(x, y);
				data[off] = r;
				data[off + 1] = g;
				data[off + 2] = b;
				pixel = pixel + format.BytesPerPixel;
			}
		}
	}

	SDL_FreeSurface(surface);
}

int ImageData::WriteImageData(const char *filename)
{
	if (std::string(filename).rfind(".png") == std::string::npos && std::string(filename).rfind(".bmp") == std::string::npos)
	{
		printf("[error] writing color image '%s'... not a .png / .bmp image.\n", filename);
		return -1;
	}

	if (size != 1)
	{
		printf("[error] writing color image '%s'... not an 8 bits image.\n", filename);
		return -1;
	}

	// flip de l'image : origine en bas a gauche
	std::vector<Uint8> flip(width * height * 4);

	int p = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			std::size_t off = offset(x, height - y - 1);
			Uint8 r = data[off];
			Uint8 g = data[off + 1];
			Uint8 b = data[off + 2];
			Uint8 a = 255;
			if (channels > 3)
				a = data[off + 3];

			flip[p] = r;
			flip[p + 1] = g;
			flip[p + 2] = b;
			flip[p + 3] = a;
			p = p + 4;
		}

	// construit la surface sdl
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)&flip.front(), width, height,
		32, width * 4,
#if 0
		0xFF000000,
		0x00FF0000,
		0x0000FF00,
		0x000000FF
#else
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000
#endif
	);

	// enregistre le fichier
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
