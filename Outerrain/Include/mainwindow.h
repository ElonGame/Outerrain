#pragma once

#include <cstring>
#include "window.h"
#include "heightfield.h"
#include "gpuHeightfield.h"
#include "cameraOrbiter.h"
#include "meshRenderer.h"
#include "meshSet.h"
#include "terrainSettings.h"
#include "layerfield.h"
#include "box2D.h"

class MainWindow
{
private:
	/* Window Data */
	Window* mainWindowHandler;
	CameraOrbiter orbiter;


	/* Application Data */
	GameObject* hfObject;
	Heightfield* hf;
	TerrainSettings settings;
	MeshSetRenderer* setExample;


	/* Functions */
	void InitBasicTerrain();
	void InitNoiseTerrain();
	void InitLayerTerrain();
	void InitGPUTerrain();

	void StreamPowerErosionStep();
	void HydraulicErosionStep();
	void ThermalErosionStep();
	void TranslateNoise(int, int);
	void LightingImpact();

	void GenerateTerrainFromSettings(bool gpu = false);
	void UpdateMeshMaterial();
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
