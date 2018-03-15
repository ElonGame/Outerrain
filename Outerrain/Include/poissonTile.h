#pragma once
#include <vector>
#include "vec.h"

class PoissonTile
{
protected:
	int r;
	float tileSize;
	int maxTries;
	std::vector<Vector2> poissonPoints;

	void Generate();

public:
	PoissonTile();
	PoissonTile(int r, float tileSize, int maxTries);

	void Randomize();
};
