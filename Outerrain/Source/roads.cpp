#include "roads.h"
#include "gameobject.h"

std::vector<Vector3> ShortestPath::FindShortestPath(const VegetationTerrain& terrain, const Vector3& start, const Vector3& end)
{
	std::vector<Vector3> path;
	path.push_back(Vector3(start.x, terrain.Height(Vector2(start.x, start.z)), start.z));

	Vector3 dest = Vector3(end.x, terrain.Height(Vector2(end.x, end.z)), end.z);
	Vector3 currentPosition = path[0];
	ScalarField2D slope = terrain.SlopeField();
	float step = 50.0f;
	int candidateCount = 10;
	while (currentPosition != dest)
	{
		if (Magnitude(currentPosition - dest) < 10.0f)
		{
			path.push_back(dest);
			break;
		}

		Vector3 choosenNextPosition;
		float currentMinScore = 100000.0f;
		Vector3 bestDir = Normalize(dest - currentPosition);
		for (int i = 0; i < candidateCount; i++)
		{
			float offsetX = rand() / static_cast<float>(RAND_MAX) * 0.05f;
			float offsetZ = rand() / static_cast<float>(RAND_MAX) * 0.05f;
			Vector3 direction = bestDir * Vector3(offsetX, 1.0f, offsetZ);

			Vector3 candidateNextPosition = currentPosition + (direction * step);
			float candidateScore = GetPositionScore(currentPosition, candidateNextPosition, slope, terrain);
			if (candidateScore < currentMinScore)
			{
				choosenNextPosition = candidateNextPosition;
				currentMinScore = candidateScore;
			}
		}

		choosenNextPosition.y = terrain.Height(Vector2(choosenNextPosition.x, choosenNextPosition.z));
		path.push_back(choosenNextPosition);
		currentPosition = choosenNextPosition;
	}

	return path;
}

float ShortestPath::GetPositionScore(const Vector3& currentPosition, const Vector3& candidate, const ScalarField2D& slope, const VegetationTerrain& terrain)
{
	float heightDifference = terrain.Height(Vector2(currentPosition.x, currentPosition.z)) - terrain.Height(Vector2(candidate.x, candidate.z));
	float slopeDifference = slope.GetValueBilinear(Vector2(currentPosition.x, currentPosition.z)) - slope.GetValueBilinear(Vector2(candidate.x, candidate.z));
	return heightDifference + slopeDifference;
}

GameObject* ShortestPath::GetNodeObject()
{
	Mesh* m = new Mesh(GL_TRIANGLES);
	m->ReadMesh("Data/Objs/cube.obj");
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	m->SetShader(shader);
	m->SetMaterial(Material(Color::Red(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(m);
	return obj;
}