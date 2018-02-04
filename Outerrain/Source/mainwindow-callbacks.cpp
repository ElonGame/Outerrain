#include "mainwindow.h"


void MainWindow::InitBasicTerrain()
{
	const int res = 128;
	hf = new Heightfield(std::string("Data/Heightmaps/island.png"), 0, 50, res, res, Vector2(-128, 128), Vector2(128, -128));

	hfObject = new GameObject();
	hfObject->AddComponent(hf->GetMeshModel());
	hfObject->AddComponent(new MeshRenderer(hfObject->GetComponent<MeshModel>(), MaterialModel::DefaultDiffuse()));
	orbiter.LookAt(hfObject->GetComponent<MeshModel>()->GetBounds());
}

void MainWindow::ClearScene() 
{
	if (hf)
		delete hf;
	if (hfObject)
		delete hfObject;
}
