#include "mainwindow.h"

void MainWindow::InitBasicTerrain()
{
	delete hfObject;
	hfObject = nullptr;

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 1025;
	settings.ny = 1025;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/wild.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitGPUTerrain()
{
	delete hfObject;
	hfObject = nullptr;

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 1025;
	settings.ny = 1025;
	settings.bottomLeft = Vector2(-1024);
	settings.topRight = Vector2(1024);
	settings.offsetVector = Vector3(0.0f);
	settings.filePath = std::string("Data/Heightmaps/wild.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = TerrainSplatmap;

	GenerateTerrainFromSettings(true);
	UpdateMeshRenderer();
}

void MainWindow::InitNoiseTerrain()
{
	delete hfObject;
	hfObject = nullptr;

	settings.terrainType = TerrainType::NoiseFieldTerrain;
	settings.nx = 256;
	settings.ny = 256;
	settings.bottomLeft = Vector2(-128, -128);
	settings.topRight = Vector2(128, 128);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.frequency = 0.005f;
	settings.octaves = 8;
	settings.amplitude = 50.0f;
	settings.fractalType = FractalType::fBm;
	settings.shaderType = TerrainSplatmap;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::InitLayerTerrain()
{
	settings.shaderType = TerrainSplatmap;
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
