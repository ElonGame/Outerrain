#ifndef _IMAGE_H
#define _IMAGE_H

#include <vector>
#include <cassert>

#include "color.h"

struct ImageData
{
	ImageData() : data(), width(0), height(0), channels(0), size(0) {}
	ImageData(const int w, const int h, const int c, const int s = 1) : data(w*h*c*s), width(w), height(h), channels(c), size(s) {}

	std::size_t offset(const int x, const int y) { return y * width * channels * size + x * channels * size; }
	const void *buffer() const { return &data.front(); }
	void *buffer() { return &data.front(); }

	void ReadImageData(const char *filename);
	int WriteImageData(const char *filename);

	std::vector<unsigned char> data;

	int width;
	int height;
	int channels;
	int size;
};

class Image
{
protected:
	std::vector<Color> data;
	int width;
	int height;

public:
	Image() : data(), width(0), height(0) {}
	Image(const int w, const int h, const Color& color = Color::Black()) : data(w*h, color), width(w), height(h) {}

	Color& operator() (const int x, const int y)
	{
		std::size_t offset = y * width + x;
		assert(offset < data.size());
		return data[offset];
	}

	Color operator() (const int x, const int y) const
	{
		std::size_t offset = y * width + x;
		assert(offset < data.size());
		return data[offset];
	}

	const void* Buffer() const
	{
		assert(!data.empty());
		return &data.front();
	}

	void ReadImage(const char *filename, bool flipY);
	int WriteImage(const char *filename, bool flipY);

	int Width() const { return width; }
	int Height() const { return height; }
	std::size_t Size() const { return width * height; }

	static Image& Error()
	{
		static Image image;
		return image;
	}

	bool operator==(const Image& im) const
	{
		return (this == &im);
	}
};

#endif