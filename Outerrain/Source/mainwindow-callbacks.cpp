#include "mainwindow.h"


void MainWindow::InitBasicTerrain()
{
	const int res = 1024;

	hf = new Heightfield(std::string("Data/Heightmaps/island.png"), 0, 100, res, res, Vector2(0, 0), Vector2(res, res));
	hfRenderer = new MeshRenderer(hf->GetMeshModel());
	orbiter.LookAt(hfRenderer->GetMeshModel().GetBounds());
}

void MainWindow::ClearScene() 
{
	if (hf)
		delete hf;
}
