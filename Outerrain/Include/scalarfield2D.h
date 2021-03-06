#pragma once

#include "GL/glew.h"
#include "valueField.h"

typedef struct ScalarValue
{
	int x, y;
	float value;
	ScalarValue() { }
	ScalarValue(int a, int b, float h) : x(a), y(b), value(h) { }
} ScalarValue;

class ScalarField2D : public ValueField<float>
{
public:
	ScalarField2D();
	ScalarField2D(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox);
	ScalarField2D(const ScalarField2D& field);
	ScalarField2D(int nx, int ny, const Box2D& bbox);
	ScalarField2D(int nx, int ny, const Box2D& bbox, float value);
	~ScalarField2D();

	void Add(int i, int j, float v);
	void Remove(int i, int j, float v);

	Vector3 Vertex(const Vector2& v) const;
	Vector3 Vertex(int i, int j) const;
	Vector3 Vertex(const Vector2i& v) const;

	Vector2 CellSize() const;
	Vector2 Gradient(int i, int j) const;
	ScalarField2D Normalized() const;
	void NormalizeField();
	void NormalizeField(float min, float max);
	float Average() const;

	std::vector<ScalarValue> FilterSuperiorTo(float threshold) const;
	std::vector<ScalarValue> FilterInferiorTo(float threshold) const;
	std::vector<ScalarValue> FilterBetween(float min, float max) const;

	void SaveAsImage(const std::string& filePath);
	void ReadFromImage(const std::string& filePath, float, float);
	GLuint GetGLTexture(int unit) const;
};
