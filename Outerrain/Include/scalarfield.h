#pragma once

#include "valueField.h"

class Scalarfield2D : public ValueField<float>
{
public:
	Scalarfield2D();
	Scalarfield2D(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight);
	Scalarfield2D(const int& nx, const int& ny, const Vector2& bottomLeft, const Vector2& topRight, const float& value);

	Vector3 Vertex(const int& i, const int& j) const;
	Vector3 Vertex(const Vector2i& v) const;

	Vector2 Gradient(const int& i, const int& j) const;
	void NormalizeField();
	void NormalizeField(const float& min, const float& max);
	float AverageValue() const;

	void WriteImageGrayscale(const char* path);
	void ReadImageGrayscale(const char* file, int blackAltitude, int whiteAltitude);
};
