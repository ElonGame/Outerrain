#include "poissonTile2D.h"
#include "random.h"

using Random = effolkronium::random_static;


PoissonTile2D::PoissonTile2D() : r(0), tileSize(0), maxTries(0)
{

}

PoissonTile2D::PoissonTile2D(int r, float tileSize, int maxTries) : r(r), tileSize(tileSize), maxTries(maxTries)
{
	Generate();
}

void PoissonTile2D::Generate()
{
	Vector2 tileCenter = Vector2(tileSize / 2.0f, tileSize / 2.0f);
	int i = 0;
	while (i < maxTries)
	{
		// Get random sample point
		float randX = Random::get() / float(Random::max());
		float randY = Random::get() / float(Random::max());
		Vector2 point = Vector2(randX * tileSize, randY * tileSize);
		i++;

		// Test point against local neighbours
		bool canAdd = true;
		for (size_t k = 0; k < poissonPoints.size(); k++)
		{
			if (Magnitude(point - poissonPoints[k]) <= r)
			{
				canAdd = false;
				break;
			}
		}
		if (canAdd == false)
			continue;

		// Test point against all possible rotated tiled neighbours left and right
		if (canAdd == true && (point.x - r < 0.0f || point.x + r > tileSize))
		{
			for (size_t k = 0; k < poissonPoints.size(); k++)
			{
				float branch = point.x - r < 0.0f ? -1.0f : 1.0f;
				Vector2 neighbourPoint = poissonPoints[k] + (Vector2(tileSize, 0.0f) * branch);
				if (Magnitude(point - neighbourPoint) <= r)
				{
					canAdd = false;
					break;
				}
			}
			if (canAdd == false)
				continue;
		}

		// Test point against all possible rotated tiled neighbours down and up
		if (canAdd == true && (point.y - r < 0.0f || point.y + r > tileSize))
		{
			for (size_t k = 0; k < poissonPoints.size(); k++)
			{
				float branch = point.y - r < 0.0f ? -1.0f : 1.0f;
				Vector2 neighbourPoint = poissonPoints[k] + (Vector2(0.0f, tileSize) * branch);
				if (Magnitude(point - neighbourPoint) <= r)
				{
					canAdd = false;
					break;
				}
			}
			if (canAdd == false)
				continue;
		}

		// If all tests have passed, add points at 4 possible rotations
		if (canAdd == true)
		{
			poissonPoints.push_back(point);
			poissonPoints.push_back(RotateAround(point, tileCenter, 90.0f));
			poissonPoints.push_back(RotateAround(point, tileCenter, 180.0f));
			poissonPoints.push_back(RotateAround(point, tileCenter, 270.0f));
		}
	}
}

void PoissonTile2D::Randomize()
{
	Generate();
}

std::vector<Vector2> PoissonTile2D::GetPoints() const
{
	return poissonPoints;
}
