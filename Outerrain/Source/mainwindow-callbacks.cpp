#include "mainwindow.h"
#include "heightfieldmesh.h"


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

	auto cpu_start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000; i++)
		hf->ThermalWeathering(1.0f);

	auto cpu_stop = std::chrono::high_resolution_clock::now();
	long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();
	std::cout << "CPU " << static_cast<int>((cpu_time / 1000000)) << "ms" << static_cast<int>(((cpu_time / 1000) % 1000)) << "us" << std::endl;

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
	settings.offsetVector = settings.offsetVector + Vector3(static_cast<float>(x), 0.0f, static_cast<float>(y));
	hf->InitFromNoise(*settings.noise, settings.amplitude, settings.frequency, settings.octaves, settings.offsetVector, settings.fractalType);
	UpdateMeshRenderer();
}

void MainWindow::GenerateTerrainFromSettings(bool gpu)
{
	if (hf != nullptr)
		delete hf;
	if (gpu)
		hf = new GPUHeightfield(settings);
	else
		hf = new HeightField(settings);
}

void MainWindow::UpdateMeshRenderer()
{
	MaterialBase* mat = nullptr;
	if (settings.shaderType == TerrainSplatmapMaterial)
		mat = MaterialBase::TerrainSplatmapMaterialInstance;
	if (settings.shaderType == DiffuseMaterial)
		mat = MaterialBase::DiffuseMaterialInstance;
	if (settings.shaderType == SingleTexturedMaterial && hf != nullptr)
		mat = MaterialBase::SingleTexturedMaterialInstance;
	if (settings.shaderType == NormalMaterial)
		mat = MaterialBase::NormalMaterialInstance;
	if (settings.shaderType == WireframeMaterial)
		mat = MaterialBase::WireframeMaterialInstance;

	// Only done once the first time
	if (hierarchy.GetAllObjects().size() == 0)
	{
		GameObject* hfObject = new GameObject();
		hfObject->SetPosition(Vector3(0));
		hfObject->AddComponent(new HeightfieldMesh(hf));
		hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<HeightfieldMesh>(), mat));
		hierarchy.AddObject(hfObject);
		return;
	}

	hierarchy.GetObject(0)->GetComponent<HeightfieldMesh>()->UpdateMeshBuffers();
	if (mat != nullptr)
		hierarchy.GetObject(0)->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::UpdateMeshMaterial()
{
	MaterialBase* mat = nullptr;
	if (settings.shaderType == TerrainSplatmapMaterial)
		mat = MaterialBase::TerrainSplatmapMaterialInstance;
	if (settings.shaderType == DiffuseMaterial)
		mat = MaterialBase::DiffuseMaterialInstance;
	if (settings.shaderType == SingleTexturedMaterial)
		mat = MaterialBase::SingleTexturedMaterialInstance;
	if (settings.shaderType == NormalMaterial)
		mat = MaterialBase::NormalMaterialInstance;
	if (settings.shaderType == WireframeMaterial)
		mat = MaterialBase::WireframeMaterialInstance;
	
	if (mat != nullptr)
		hierarchy.GetObject(0)->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	hf = nullptr;
	hierarchy.DeleteObject(0);
}
