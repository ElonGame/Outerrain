#include "mainwindow.h"

void MainWindow::InitBasicTerrain()
{
	const int res = 128;
	hf = new Heightfield(std::string("Data/Heightmaps/island.png"), 0, 50, res, res, Vector2(-128, 128), Vector2(128, -128));
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
	hf->Thermal(1, 2.0);
	UpdateMeshRenderer();
}

void MainWindow::UpdateMeshRenderer()
{
	delete hfObject;

	hfObject = new GameObject();
	hfObject->AddComponent(hf->GetMeshModel());
	hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<MeshModel>(), MaterialModel::DefaultDiffuse()));
}

void MainWindow::ClearScene() 
{
	if (hf)
		delete hf;
	if (hfObject)
		delete hfObject;
}
