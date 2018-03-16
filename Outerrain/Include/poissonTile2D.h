#pragma once
#include <vector>
#include "vec.h"

class PoissonTile2D
{
protected:
	int r;
	float tileSize;
	int maxTries;
	std::vector<Vector2> poissonPoints;

	void Generate();

public:
	PoissonTile2D();
	PoissonTile2D(int r, float tileSize, int maxTries);

	void Randomize();
	std::vector<Vector2> GetPoints() const;
};
