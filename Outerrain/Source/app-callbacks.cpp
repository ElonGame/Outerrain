#include "app.h"
#include "graph.h"

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
	for (size_t i = 0; i < trees.size(); i++)
		scene.AddChild(trees[i]);
	CalculateAllMaps();
}

void App::GenerateRoadCallback()
{
	Vector2 begin(200, 20);
	Vector2 end(20, 200);

	// Eight
	std::list<vertex_t>::const_iterator iterator;
	std::list<vertex_t> path = ShortestPath::FindPath(vegTerrain, begin.x, begin.y, end.x, end.y, NeighborhoodType::EIGHT);
	for (iterator = path.begin(); iterator != path.end(); ++iterator)
	{
		GameObject* o = ShortestPath::GetNodeObject();
		int x, y;
		vegTerrain.Index2D(*iterator, x, y);
		Vector3 pos = vegTerrain.Vertex(x, y);
		o->SetPosition(pos);
		scene.AddChild(o);
	}

	// Four
	path = ShortestPath::FindPath(vegTerrain, begin.x, begin.y, end.x, end.y, NeighborhoodType::FOUR);
	for (iterator = path.begin(); iterator != path.end(); ++iterator)
	{
		GameObject* o = ShortestPath::GetNodeObject();
		o->GetComponent<Mesh>()->SetMaterial(Material(Color::Blue(), 0));
		int x, y;
		vegTerrain.Index2D(*iterator, x, y);
		Vector3 pos = vegTerrain.Vertex(x, y);
		o->SetPosition(pos);
		scene.AddChild(o);
	}

	// Eight extended
	path = ShortestPath::FindPath(vegTerrain, begin.x, begin.y, end.x, end.y, NeighborhoodType::EXTENDED);
	for (iterator = path.begin(); iterator != path.end(); ++iterator)
	{
		GameObject* o = ShortestPath::GetNodeObject();
		o->GetComponent<Mesh>()->SetMaterial(Material(Color::Green(), 0));
		int x, y;
		vegTerrain.Index2D(*iterator, x, y);
		Vector3 pos = vegTerrain.Vertex(x, y);
		o->SetPosition(pos);
		scene.AddChild(o);
	}
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

	//CalculateAllMaps();

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

	//CalculateAllMaps();

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
