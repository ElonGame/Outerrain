#include "texture2D.h"
#include <cassert>
#include <cmath>

/*
\brief 2D Texture class. Loads and saves using SDL functions. Conversion to OpenGL texture available as static and member functions.
*/

/*
\brief Constructs a texture from a file.
\param filePath relative file to load
*/
Texture2D::Texture2D(const std::string& filePath)
{
	SDL_Surface* tex = IMG_Load(filePath.c_str());
	if (tex == NULL)
		std::cout << "Error loading image " << filePath << " - SDL_Image failed" << std::endl;
	
	const SDL_PixelFormat format = *tex->format;
	data.resize(tex->h * tex->w);
	frameWidth = tex->w;
	frameHeight = tex->h;
	bitsPerPixel = format.BitsPerPixel;
	bytesPerPixel = format.BytesPerPixel;
	for (int y = 0; y < frameHeight; y++)
	{
		int py = (tex->h - 1) - y;
		Uint8* pixel = (Uint8*)tex->pixels + py * tex->pitch;
		for (int x = 0; x < frameWidth; x++)
		{
			if (bitsPerPixel == 32)
			{
				float r = float(pixel[format.Rshift / 8]);
				float g = float(pixel[format.Gshift / 8]);
				float b = float(pixel[format.Bshift / 8]);
				float a = float(pixel[format.Ashift / 8]);

				int index = y * tex->w + x;
				data[index] = Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
				pixel += bytesPerPixel;
			}
			else if (bitsPerPixel == 24)
			{
				float r = float(pixel[format.Rshift / 8]);
				float g = float(pixel[format.Gshift / 8]);
				float b = float(pixel[format.Bshift / 8]);

				int index = y * tex->w + x;
				data[index] = Color(r / 255.f, g / 255.f, b / 255.f);
				pixel += bytesPerPixel;
			}
			else if (bitsPerPixel == 16)
			{
				float r = float(pixel[format.Rshift / 8]);
				float g = float(pixel[format.Gshift / 8]);

				int index = y * tex->w + x;
				data[index] = Color(r / 255.f, g / 255.f, 0.0f);
				pixel += bytesPerPixel;
			}
			else if (bitsPerPixel == 8)
			{
				float r = float(pixel[format.Rshift / 8]);
				int index = y * tex->w + x;
				data[index] = Color(r / 255.f, r / 255.f, r / 255.f);
				pixel += bytesPerPixel;
			}
		}
	}
	SDL_FreeSurface(tex);
}

/*
\brief Constructs an empty texture.
*/
Texture2D::Texture2D(int w, int h)
{
	frameWidth = w;
	frameHeight = h;
	data.resize(w * h);
	bitsPerPixel = 32;
}

/*
\brief Destructor
*/
Texture2D::~Texture2D()
{
}

/*
\brief Set color to pixel
\param x
\param y
\param c
*/
void Texture2D::SetPixel(int x, int y, const Color& c)
{
	data[y * frameWidth + x] = c;
}

/*
\brief Get Pixel value
\param x
\param y
*/
Color Texture2D::Pixel(int x, int y) const
{
	return data[y * frameWidth + x];
}

/*
\brief Get pixel buffer.
*/
void* Texture2D::PixelBuffer() const
{
	return (void*)&data.front();
}

/*
\brief Get texture width.
*/
int Texture2D::Width() const
{
	return frameWidth;
}

/*
\brief Get texture height.
*/
int Texture2D::GetValueBilinear() const
{
	return frameHeight;
}

/*
\brief Get bytes per pixel.
*/
int Texture2D::BytesPerPixel() const
{
	return bytesPerPixel;
}

/*
\brief Save the current texture with SDL functions.
\param filePath relative path
*/
void Texture2D::Save(const std::string& filePath) const
{
	if (filePath.rfind(".png") == std::string::npos)
	{
		std::cout << "Error : writing color image, not a .png image : " << filePath << std::endl;
		return;
	}

	// Creates the bytes vector
	std::vector<Uint8> byteData(frameWidth * frameHeight * 4);
	int byteArrayIndex = 0;
	for (int y = 0; y < frameHeight; y++)
	{
		for (int x = 0; x < frameWidth; x++)
		{
			Color color = data[y * frameWidth + x];
			Uint8 r = (Uint8)Math::Min(std::floor(color.r * 255.f), 255.f);
			Uint8 g = (Uint8)Math::Min(std::floor(color.g * 255.f), 255.f);
			Uint8 b = (Uint8)Math::Min(std::floor(color.b * 255.f), 255.f);
			Uint8 a = (Uint8)Math::Min(std::floor(color.a * 255.f), 255.f);

			byteData[byteArrayIndex] = r;
			byteData[byteArrayIndex + 1] = g;
			byteData[byteArrayIndex + 2] = b;
			byteData[byteArrayIndex + 3] = a;
			byteArrayIndex = byteArrayIndex + 4;
		}
	}

	// Todo : support multiple pixel format
	Uint32 format = SDL_PIXELFORMAT_RGBA32;
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(&byteData.front(), frameWidth, frameHeight, 32, frameWidth * 4, format);
	if (IMG_SavePNG(surface, filePath.c_str()) < 0)
		std::cout << "Error : writing color image " << filePath << " : " << SDL_GetError() << std::endl;
	SDL_FreeSurface(surface);
}

/*
\brief Build the opengl texture and returns it
\param unit gl texture unit id
\param mipmap generates mipmap for the gl texture if true
*/
GLuint Texture2D::GetGLTexture(int unit, bool mipmap) const
{
	if (data.empty())
		return 0;

	GLint texelType;
	if (bitsPerPixel == 32)
		texelType = GL_RGBA;
	else if (bitsPerPixel == 24)
		texelType = GL_RGB;
	else
		texelType = 0;

	// Todo : support this
	//GLenum format;
	//switch (im.channels)
	//{
	//case 1: format = GL_RED; break;
	//case 2: format = GL_RG; break;
	//case 3: format = GL_RGB; break;
	//case 4: format = GL_RGBA; break;
	//default: format = GL_RGBA;
	//}

	//GLenum type;
	//switch (im.size)
	//{
	//case 1: type = GL_UNSIGNED_BYTE; break;
	//case 4: type = GL_FLOAT; break;
	//default: type = GL_UNSIGNED_BYTE;
	//}

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, texelType, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, PixelBuffer());
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

/*
\brief Build an opengl texture from a file and returns it.
\param filePath relative path
\param unit gl texture unit id
\param mipmap generates mipmap for the gl texture if true
*/
GLuint Texture2D::MakeGLTexture(const std::string& filePath, int unit, bool mipmap)
{
	Texture2D tex = Texture2D(filePath);
	return tex.GetGLTexture(unit, mipmap);
}
