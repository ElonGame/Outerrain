#pragma once

#include "Window.h"
#include "mesh.h"
#include "cameraOrbiter.h"
#include "terrain.h"

class App
{
protected:
	SDL_Window* window;
	SDL_GLContext glContext;
	Mesh mesh;
	CameraOrbiter orbiter;

	// Heightfield Data
	Terrain2D terrain2D;
	LayerTerrain2D layerTerrain2D;

public:
	App(const int&, const int&, const int&, const int&);
	virtual ~App();

	virtual int Init();
	virtual void InitImGUI();
	virtual int Update(const float, const float);
	virtual int Render();
	virtual void Run();
	virtual void Quit();
};
