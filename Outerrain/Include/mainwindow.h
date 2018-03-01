#pragma once

#include <cstring>
#include "window.h"
#include "heightfield.h"
#include "cameraOrbiter.h"
#include "meshRenderer.h"
#include "terrainSettings.h"

class MainWindow
{
private:
	/* Window Data */
	Window* mainWindowHandler;
	CameraOrbiter orbiter;

	/* Application Data */
	Heightfield* hf;
	GameObject* hfObject;
	TerrainSettings settings;

	/* Functions */
	void InitBasicTerrain();
	void StreamPowerErosionStep();
	void ThermalErosionStep();
	void TranslateNoise(int, int);

	void GenerateTerrainFromSettings();
	void UpdateMeshRenderer();
	void ClearScene();

	void Init();
	void MainLoop();
	void Update(float time, float deltaTime);
	void Render();

public:
	MainWindow(int windowWidth, int windowHeight);
	~MainWindow();

	void Show();
	void Quit();
};
