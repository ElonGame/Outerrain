#pragma once
#include "scalarfield.h"

class LayerField
{
protected:
	Scalarfield2D bedrock;
	Scalarfield2D sediments;
	Scalarfield2D water;
	Scalarfield2D snow;
	Scalarfield2D vegetation;

public:
	LayerField(int nx, int ny, const Box2D& bbox);
	LayerField(int nx, int ny, const Box2D& bbox, float value);
	LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox);
};
