#include "mainwindow.h"

void MainWindow::InitBasicTerrain()
{
	delete hfObject;
	hfObject = nullptr;

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
	delete hfObject;
	hfObject = nullptr;

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
	delete hfObject;
	hfObject = nullptr;

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.nx = 512;
	settings.ny = 512;
	settings.bottomLeft = Vector2(-1024, -1024);
	settings.topRight = Vector2(1024, 1024);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.002f;
	settings.octaves = 8;
	settings.amplitude = 150.0f;
	settings.fractalType = FractalType::MusgraveHybridMultifractal;
	settings.shaderType = ShaderType::TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitLayerTerrain()
{
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
