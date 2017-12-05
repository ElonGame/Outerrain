#pragma once

#include "Window.h"
#include "mesh.h"
#include "cameraOrbiter.h"
#include "fields.h"

class App
{
protected:
	Window m_window;
	Context m_context;
	Mesh mesh;
	CameraOrbiter orbiter;

	// Heightfield Data
	Terrain2D terrain2D;
	LayerTerrain2D layerTerrain2D;

public:
	App(const int&, const int&, const int&, const int&);
	virtual ~App();

	virtual int Init();
	virtual int Update(const float, const float);
	virtual int Render();
	virtual void Run();
	virtual void Quit();
};
