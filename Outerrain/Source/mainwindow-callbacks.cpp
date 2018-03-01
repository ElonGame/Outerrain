#include "mainwindow.h"


void MainWindow::InitBasicTerrain()
{
	/*settings.type = TerrainType::NoiseFieldTerrain;
	settings.nx = 128;
	settings.ny = 128;
	settings.bottomLeft = Vector2(-128, 128);
	settings.topRight = Vector2(128, -128);
	settings.offsetVector = Vector3(0);
	settings.noise = new PerlinNoise();
	settings.maxAltitude = 100;
	settings.frequency = 0.005;
	settings.octaves = 8;
	settings.fractalType = FractalType::Ridge;*/

	settings.terrainType = TerrainType::HeightFieldTerrain;
	settings.nx = 128;
	settings.ny = 128;
	settings.bottomLeft = Vector2(-128, 128);
	settings.topRight = Vector2(128, -128);
	settings.offsetVector = Vector3(0);
	settings.filePath = std::string("Data/Heightmaps/island.png");
	settings.minAltitude = 0;
	settings.maxAltitude = 50;

	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
	orbiter.LookAt(hfObject->GetComponent<MeshModel>()->GetBounds());
}

void MainWindow::StreamPowerErosionStep()
{
	if (hf == nullptr)
		return;
	hf->StreamPowerErosion(1, 2.0);
	UpdateMeshRenderer();
}

void MainWindow::ThermalErosionStep()
{
	if (hf == nullptr)
		return;
	hf->ThermalWeathering(1, 0.08f);
	UpdateMeshRenderer();
}

void MainWindow::TranslateNoise(int y, int x)
{
	if (settings.terrainType != TerrainType::NoiseFieldTerrain)
		return;
	settings.offsetVector = settings.offsetVector + Vector3(x, 0, y);
	GenerateTerrainFromSettings();
	UpdateMeshRenderer();
}

void MainWindow::GenerateTerrainFromSettings()
{
	if (settings.terrainType == TerrainType::NoiseFieldTerrain)
		hf = new Heightfield(settings.nx, settings.ny, settings.bottomLeft, settings.topRight, *settings.noise, settings.maxAltitude, settings.frequency, settings.octaves, settings.offsetVector, settings.fractalType);
	else
		hf = new Heightfield(settings.filePath, settings.minAltitude, settings.maxAltitude, settings.nx, settings.ny, settings.bottomLeft, settings.topRight);
}

void MainWindow::UpdateMeshRenderer()
{
	if (hfObject != nullptr)
		delete hfObject;
	hfObject = new GameObject();
	hfObject->AddComponent(hf->GetMeshModel());
	hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<MeshModel>(), MaterialModel::TerrainTexturedMat));
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	if (hfObject != nullptr)
		delete hfObject;
}
