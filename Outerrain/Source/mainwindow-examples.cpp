#include "mainwindow.h"

void MainWindow::InitBasicTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 1024;
	settings.ny = 1024;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/canyon-513.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitGPUTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 256;
	settings.ny = 256;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/wild.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings(true);
	UpdateMeshRenderer();
}

void MainWindow::InitNoiseTerrain()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.nx = 512;
	settings.ny = 512;
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

void MainWindow::GenerateTerrainFromSettings(bool gpu)
{
	if (hf != nullptr)
		delete hf;
	if (gpu)
		hf = new GPUHeightfield(settings);
	else
		hf = new Heightfield(settings);
}


/* Heightfield 1 Scene */
void MainWindow::ExampleScene1()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 1024;
	settings.ny = 1024;
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

/* Heightfield 2 Scene */
void MainWindow::ExampleScene2()
{
	ClearScene();

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 1024;
	settings.ny = 1024;
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

/* NoiseField 1 Scene */
void MainWindow::ExampleScene3()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.nx = 1024;
	settings.ny = 1024;
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

/* NoiseField 2 Scene */
void MainWindow::ExampleScene4()
{
	ClearScene();

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.nx = 1024;
	settings.ny = 1024;
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