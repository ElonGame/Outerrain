#include "mainwindow.h"
#include "heightfieldMeshModel.h"

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
	settings.filePath = std::string("Data/Heightmaps/dome-2048.png");
	settings.minAltitude = 0.0f;
	settings.maxAltitude = 250.0f;
	settings.shaderType = TerrainSplatmap;

	GenerateTerrainFromSettings();
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

void MainWindow::StreamPowerErosionStep()
{
	if (hf == nullptr)
		return;
	hf->StreamPowerErosion(2.0);
	UpdateMeshRenderer();
}

void MainWindow::ThermalErosionStep()
{
	if (hf == nullptr)
		return;
	for (int i = 0; i < 1000; i++)
		hf->ThermalWeathering(0.6f);

	std::cout << "1000 iterations of thermal erosion" << std::endl;
	UpdateMeshRenderer();
}

void MainWindow::HydraulicErosionStep()
{
	if (hf == nullptr)
		return;
	hf->HydraulicErosion();
	UpdateMeshRenderer();
}

void MainWindow::TranslateNoise(int y, int x)
{
	if (settings.terrainType != TerrainType::NoiseFieldTerrain)
		return;
	settings.offsetVector = settings.offsetVector + Vector3((float)x, 0.0f, (float)y);
	hf->InitFromNoise(*settings.noise, settings.amplitude, settings.frequency, settings.octaves, settings.offsetVector, settings.fractalType);
	UpdateMeshRenderer();
}

void MainWindow::GenerateTerrainFromSettings()
{
	if (hf != nullptr)
		delete hf;
	hf = new Heightfield(settings);
}

void MainWindow::UpdateMeshRenderer()
{
	MaterialModel mat;
	if (settings.shaderType == TerrainSplatmap)
		mat = MaterialModel::TerrainTexturedMat;
	if (settings.shaderType == DiffuseGrey)
		mat = MaterialModel::DefaultDiffuseMat;
	if (settings.shaderType == SimpleTextured)
	{
		mat = MaterialModel::DefaultTexturedMat;
		glDeleteTextures(1, &mat.texture0);
		mat.texture0 = hf->DrainageArea().GetGLTexture(0);
	}

	// Only done once the first time
	if (hfObject == nullptr)
	{
		hfObject = new GameObject();
		hfObject->SetPosition(Vector3(0));
		hfObject->AddComponent(new HeightfieldMeshModel(hf));
		hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<HeightfieldMeshModel>(), mat));
		return;
	}

	hfObject->GetComponent<HeightfieldMeshModel>()->UpdateMeshBuffers();
	hfObject->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::UpdateMeshMaterial()
{
	MaterialModel mat;
	if (settings.shaderType == TerrainSplatmap)
		mat = MaterialModel::TerrainTexturedMat;
	if (settings.shaderType == DiffuseGrey)
		mat = MaterialModel::DefaultDiffuseMat;
	if (settings.shaderType == SimpleTextured)
		mat = MaterialModel::DefaultTexturedMat;
	hfObject->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	if (gpuHeightfield != nullptr)
		delete gpuHeightfield;
	if (hfObject != nullptr)
		delete hfObject;
}
