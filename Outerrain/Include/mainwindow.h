#pragma once

#include <cstring>
#include "window.h"
#include "gpuheightfield.h"
#include "cameraorbiter.h"
#include "gameobject.h"
#include "meshrenderer.h"
#include "meshsetrenderer.h"
#include "terrainsettings.h"
#include "layerfield.h"
#include "app-stats.h"
#include "light.h"

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

	/* Example scenes */
	void Heightfield1Scene();
	void Heightfield2Scene();
	void NoiseField1Scene();
	void NoiseField2Scene();
	void InstanceScene();

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
	void RenderGUI();

public:
	MainWindow(int windowWidth, int windowHeight);
	~MainWindow();

	void Show();
	void Quit();

	static DirectionnalLight sceneLight;
};
