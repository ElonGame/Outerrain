#include "mainwindow.h"
#include "random.h"

DirectionnalLight MainWindow::sceneLight = DirectionnalLight(Vector3(0.707f, -0.707f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(0.1, 0.1f, 0.1f), 0.8f);

void MainWindow::InitBasicTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 1024;
	settings.bottomLeft = Vector2(-2048, -2048);
	settings.topRight = Vector2(2048, 2048);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitGPUTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 4096;
	settings.bottomLeft = Vector2(-4096, -4096);
	settings.topRight = Vector2(4096, 4096);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings(true);
	UpdateMeshRenderer();
}

void MainWindow::InitNoiseTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024, -1024);
	settings.topRight = Vector2(1024, 1024);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100.0f;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitLayerTerrain()
{
	ClearScene();
	settings.shaderType = ShaderType::TerrainSplatmap;
	hf = new LayerField(std::string("Data/Heightmaps/island.png"), 0, 250, 256, 256, Box2D(Vector2(-512), Vector2(512)));
	UpdateMeshRenderer();
}

void MainWindow::Heightfield1Scene()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/heightmap1.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::Heightfield2Scene()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/island.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 350.0f;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::NoiseField1Scene()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024, -1024);
	settings.topRight = Vector2(1024, 1024);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100.0f;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::NoiseField2Scene()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024, -1024);
	settings.topRight = Vector2(1024, 1024);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100.0f;
	settings.fractalType = FractalType::MusgraveHeteroTerrain;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InstanceScene()
{
	hfObject = new GameObject();
	hfObject->AddComponent(new MeshRenderer());

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.resolution = 128;
	settings.bottomLeft = Vector2(-1024, -1024);
	settings.topRight = Vector2(1024, 1024);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 100.0f;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	instanceRenderer = new MeshSetRenderer(new Mesh("Data/Objs/cube.obj"));
	instanceRenderer->SetMaterial(Material::DefaultDiffuseMat);
	using Random = effolkronium::random_static;
	Box box = Box(Vector3(0), 100);
	for (int i = 0; i < 10000; i++)
	{
		float x = Random::get(box.Vertex(0).x, box.Vertex(1).x);
		float y = Random::get(box.Vertex(0).y, box.Vertex(1).y);
		float z = Random::get(box.Vertex(0).z, box.Vertex(1).z);
		Frame f;
		f.SetPosition(Vector3(x, y, z));
		instanceRenderer->AddFrame(f);
	}

	hfMesh = new Mesh();
	orbiter.LookAt(instanceRenderer->GetBounds());
}
