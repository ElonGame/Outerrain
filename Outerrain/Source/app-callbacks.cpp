#include "app.h"

void App::ThermalErosionCallback(int stepCount)
{
	// On layer terrain
	if (layerTerrain2D.SizeX() > 0 && layerTerrain2D.SizeY() > 0)
	{
		layerTerrain2D.ThermalErosion(stepCount);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(layerTerrain2D.GetAllVertices());
	}
	// On simple terrain
	if (vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
	{
		vegTerrain.ThermalErosion(stepCount);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(vegTerrain.GetAllVertices());
		CalculateAllMaps();
	}
}

void App::StreamPowerErosionCallback(int stepCount, float amplitude)
{
	vegTerrain.StreamPowerErosion(stepCount, amplitude);
	scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(vegTerrain.GetAllVertices());
	CalculateAllMaps();
}

void App::SpawnVegetationCallback()
{
	vegTerrain.ComputeVegetationDensities();
	std::vector<GameObject*> trees = vegTerrain.GetTreeObjects();
	for (int i = 0; i < trees.size(); i++)
		scene.AddChild(trees[i]);
	CalculateAllMaps();
}
