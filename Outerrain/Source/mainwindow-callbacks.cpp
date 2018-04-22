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

void MainWindow::LightingImpact()
{
	if (!hf)
		return;
	LayerField* lf = dynamic_cast<LayerField*>(hf);
	lf->LightingEventSimulate(5.0, 100, 5);
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

void MainWindow::UpdateMeshRenderer()
{
	Material mat;
	if (settings.shaderType == TerrainSplatmapMaterial)
		mat = Material::TerrainSplatmapMaterialInstance;
	if (settings.shaderType == DiffuseMaterial)
		mat = Material::DiffuseMaterialInstance;
	if (settings.shaderType == TexturedMaterial && hf != nullptr)
	{
		mat = Material::TexturedMaterialInstance;
		glDeleteTextures(1, &mat.texture0);
		mat.texture0 = hf->Slope().GetGLTexture(0);
	}
	if (settings.shaderType == NormalMaterial)
		mat = Material::NormalMaterialInstance;
	if (settings.shaderType == WireframeMaterial)
		mat = Material::WireframeMaterialInstance;

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
	hierarchy.GetObject(0)->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::UpdateMeshMaterial()
{
	Material mat;
	if (settings.shaderType == TerrainSplatmapMaterial)
		mat = Material::TerrainSplatmapMaterialInstance;
	if (settings.shaderType == DiffuseMaterial)
		mat = Material::DiffuseMaterialInstance;
	if (settings.shaderType == TexturedMaterial)
		mat = Material::TexturedMaterialInstance;
	if (settings.shaderType == NormalMaterial)
		mat = Material::NormalMaterialInstance;
	if (settings.shaderType == WireframeMaterial)
		mat = Material::WireframeMaterialInstance;
	hierarchy.GetObject(0)->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	hf = nullptr;
	hierarchy.DeleteObject(0);
}
