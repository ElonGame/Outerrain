#pragma once

#include "window.h"
#include "mesh.h"
#include "cameraOrbiter.h"
#include "terrain.h"
#include "gameobject.h"

#include "heightfield.h"

#include <chrono>
#include <sstream>

class App
{
protected:
	bool alreadyInit = false;

	Window* window;
	CameraOrbiter orbiter;

	Heightfield hf;

	// Terrain Data
	LayerTerrain2D layerTerrain2D;
	VegetationTerrain vegTerrain;

	// Scene Data
	GameObject scene;
	Mesh* terrainMesh;

	// GUI
	int currentItem;
	Uint64 lastTime;
	Uint64 newTime;
	std::stringstream cpuStr, gpuStr;
	std::chrono::high_resolution_clock::time_point cpu_start;
	std::chrono::high_resolution_clock::time_point cpu_stop;


	void InitHeightfieldNew();

	void InitSceneNoiseTerrain();
	void InitSceneVegetationTerrain();
	void InitSceneLayerTerrain();

	float windowWidth, windowHeight;

	// Callbacks
	void ThermalErosionCallback(int stepCount);
	void StreamPowerErosionCallback(int stepCount, float amplitude);
	void SpawnVegetationCallback();
	void GenerateRoadCallback();

public:
	App(int, int, int, int);

	virtual void Init();
	virtual int Update(const float, const float);
	virtual void Render();
	virtual void RenderScene();
	virtual void RenderGUI();
	virtual void Run();
	virtual void Quit();

	void StartFrameTimeComputation();
	void ComputeFrameTime();
	
	void CalculateAllMaps();
	void UpdateObjects(const float time, const float delta);
};
