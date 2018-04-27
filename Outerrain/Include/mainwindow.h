#pragma once

#include <cstring>
#include "window.h"
#include "gpuHeightfield.h"
#include "camera-orbiter.h"
#include "meshRenderer.h"
#include "terrainSettings.h"
#include "layerfield.h"
#include "app-stats.h"
#include "light.h"
#include "scene-hierarchy.h"

class MainWindow
{
private:
	/* Window Data */
	Window* mainWindowHandler;
	CameraOrbiter orbiter;

	/* Application Data */
	SceneHierarchy hierarchy;
	HeightField* hf;
	TerrainSettings settings;

	/* Example scenes */
	void AddCube(const Vector3& p = Vector3(0), float s = 1.0);
	void Heightfield1Scene();
	void Heightfield2Scene();
	void NoiseField1Scene();
	void NoiseField2Scene();

	/* Functions */
	void InitBasicTerrain();
	void InitNoiseTerrain();
	void InitLayerTerrain();
	void InitGPUTerrain();

	void StreamPowerErosionStep();
	void HydraulicErosionStep();
	void ThermalErosionStep();
	void TranslateNoise(int, int);

	void GenerateTerrainFromSettings(bool gpu = false);
	void UpdateMeshMaterial();
	void UpdateMeshRenderer();
	void ClearScene();

	void Init();
	void MainLoop();
	void Update();
	void Render();
	void RenderGUI();

public:
	MainWindow(int windowWidth, int windowHeight);
	~MainWindow();

	void Show();
	void Quit();

	static DirectionnalLight sceneLight;
};
