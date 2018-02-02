#pragma once

#include <cstring>
#include "window.h"
#include "heightfield.h"
#include "cameraOrbiter.h"
#include "meshRenderer.h"

class MainWindow
{
private:
	/* Window Data */
	Window* mainWindowHandler;
	CameraOrbiter orbiter;

	/* Application Data */
	Heightfield* hf;
	MeshRenderer* hfRenderer;


	/* Functions */
	void InitBasicTerrain();
	void ClearScene();

	void Init();
	void MainLoop();
	void Update(const float& time, const float& deltaTime);
	void Render();

public:
	MainWindow(const int& windowWidth, const int& windowHeight);
	~MainWindow();

	void Show();
	void Quit();
};