#include "mainwindow.h"


void MainWindow::InitBasicTerrain()
{
	const int res = 128;
	hf = new Heightfield(std::string("Data/Heightmaps/island.png"), 0, 50, res, res, Vector2(-res, res), Vector2(res, -res));
	//PerlinNoise n;
	//hf = new Heightfield(res, res, Vector2(-128, 128), Vector2(128, -128), 25.0, 0.005, 8);
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
	hf->ThermalWeathering(1, 0.08);
	UpdateMeshRenderer();
}

void MainWindow::UpdateMeshRenderer()
{
	if (hfObject != nullptr)
		delete hfObject;

	hfObject = new GameObject();
	hfObject->AddComponent(hf->GetMeshModel());
	hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<MeshModel>(), MaterialModel::DefaultDiffuse()));
}

void MainWindow::ClearScene() 
{
	if (hf != nullptr)
		delete hf;
	if (hfObject != nullptr)
		delete hfObject;
}
