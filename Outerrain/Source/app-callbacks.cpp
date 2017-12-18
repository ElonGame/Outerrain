#include "app.h"

void App::ThermalErosionCallback(int stepCount)
{
	// On layer terrain
	if (layerTerrain2D.SizeX() > 0 && layerTerrain2D.SizeY() > 0)
	{
		layerTerrain2D.ThermalErosion(stepCount);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(layerTerrain2D.GetAllVertices());

		/*for (int i = 0; i < layerTerrain2D.SizeX(); i++)
		{
			for (int j = 0; j < layerTerrain2D.SizeY(); j++)
			{
				float h = layerTerrain2D.Height(i, j);
				if (h > 100)
				{
					std::cout << h << std::endl;
				}
			}
		}*/
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
