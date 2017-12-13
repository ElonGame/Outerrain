#pragma once

#include "Window.h"
#include "mesh.h"
#include "cameraOrbiter.h"
#include "terrain.h"
#include "gameobject.h"

class App
{
protected:
	bool alreadyInit = false;

	SDL_Window* window;
	SDL_GLContext glContext;
	CameraOrbiter orbiter;

	// Terrain Data
	LayerTerrain2D layerTerrain2D;
	VegetationTerrain vegTerrain;

	// Scene Data
	GameObject scene;

	// GUI
	int currentItem;
	Uint64 lastTime;
	Uint64 newTime;

	void InitSceneVegetationTerrain();
	void InitSceneLayerTerrain();

	float windowWidth, windowHeight;

public:
	App(const int&, const int&, const int&, const int&);

	virtual int Init();
	virtual int Update(const float, const float);
	virtual int Render();
	virtual void Run();
	virtual void Quit();

	void UpdateObjects(const float time, const float delta);
};
