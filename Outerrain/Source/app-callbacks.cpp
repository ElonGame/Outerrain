#include "app.h"
#include "roads.h"

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

void App::GenerateRoadCallback()
{
	/*std::vector<Vector3> path = ShortestPath::FindShortestPath(vegTerrain, Vector3(30.0f, 0.0f, 30.0f), Vector3(100.0f, 0.0f, 175.0f));
	for (int i = 0; i < path.size(); i++)
	{
		GameObject* o = ShortestPath::GetNodeObject();
		o->SetPosition(path[i]);
		scene.AddChild(o);
	}*/
}

void App::InitSceneNoiseTerrain()
{
	vegTerrain = VegetationTerrain(256, 256, Vector2(-256, 256), Vector2(256, -256));
	vegTerrain.InitFromNoise(0, 150);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/TerrainShader.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	CalculateAllMaps();

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(window->Width());
	orbiter.SetFrameHeight(window->Height());
	orbiter.SetClippingPlanes(1.0f, 3000.0f);
}

void App::InitSceneVegetationTerrain()
{
	vegTerrain = VegetationTerrain(256, 256, Vector2(-256, 256), Vector2(256, -256));
	vegTerrain.InitFromFile("Data/Heightmaps/island.png", 0, 100);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/TerrainShader.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	CalculateAllMaps();

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(window->Width());
	orbiter.SetFrameHeight(window->Height());
	orbiter.SetClippingPlanes(1.0f, 3000.0f);
}

void App::InitSceneLayerTerrain()
{
	layerTerrain2D = LayerTerrain2D(256, 256, Vector2(-256, 256), Vector2(256, -256));
	layerTerrain2D.InitFromFile("Data/Heightmaps/island.png", 0, 100, 0.8f);

	Mesh* mesh = layerTerrain2D.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(window->Width());
	orbiter.SetFrameHeight(window->Height());
	orbiter.SetClippingPlanes(1.0f, 3000.0f);
}
