#pragma once

#include "valueField.h"

struct LayerStack
{
public:
	float bedrock;
	float sand;

	LayerStack(float bedrock)
	{
		this->bedrock = bedrock;
		this->sand = 0.0f;
	}

	float Height()
	{
		return bedrock + sand;
	}
};

class LayerField : public ValueField<LayerStack>
{
public:
	LayerField(int nx, int ny, const Box2D& bbox);
	LayerField(int nx, int ny, const Box2D& bbox, float value);
	LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox);

	Vector3 Vertex(int i, int j) const;
};
