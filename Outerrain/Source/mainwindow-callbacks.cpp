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
	if (settings.shaderType == TerrainSplatmap)
		mat = Material::TerrainTexturedMat;
	if (settings.shaderType == DiffuseGrey)
		mat = Material::DefaultDiffuseMat;
	if (settings.shaderType == SimpleTextured && hf != nullptr)
	{
		mat = Material::DefaultTexturedMat;
		glDeleteTextures(1, &mat.texture0);
		mat.texture0 = hf->DrainageArea().GetGLTexture(0);
	}
	if (settings.shaderType == Wireframe)
		mat = Material::WireframeMat;

	// Only done once the first time
	if (hfObject == nullptr)
	{
		hfObject = new GameObject();
		hfObject->SetPosition(Vector3(0));
		hfObject->AddComponent(new HeightfieldMesh(hf));
		hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<HeightfieldMesh>(), mat));
		return;
	}

	hfObject->GetComponent<HeightfieldMesh>()->UpdateMeshBuffers();
	hfObject->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::UpdateMeshMaterial()
{
	Material mat;
	if (settings.shaderType == TerrainSplatmap)
		mat = Material::TerrainTexturedMat;
	if (settings.shaderType == DiffuseGrey)
		mat = Material::DefaultDiffuseMat;
	if (settings.shaderType == SimpleTextured)
		mat = Material::DefaultTexturedMat;
	hfObject->GetComponent<MeshRenderer>()->SetMaterial(mat);
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	if (hfObject != nullptr)
		delete hfObject;
	hf = nullptr;
	hfObject = nullptr;
}
