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
	//for (int i = 0; i < 1000; i++)
		hf->ThermalWeathering(1.0f);

	//std::cout << "1000 iterations of thermal erosion" << std::endl;
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
	setExample = lf->GetRockInstances();
	UpdateMeshRenderer();
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
}
