#include "mainwindow.h"
#include "mytime.h"
#include <iostream>
using namespace std;

MainWindow::MainWindow(int windowWidth, int windowHeight)
{
	hfObject = nullptr;
	hf = nullptr;
	mainWindowHandler = new Window(windowWidth, windowHeight);
	mainWindowHandler->CreateGLContext(4, 3);
	Init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Quit()
{
	MaterialModel::ReleaseStaticMaterials();
	if (hf)
	{
		delete hf;
		hf = nullptr;
	}
	if (hfObject)
	{
		delete hfObject;
		hfObject = nullptr;
	}
	if (mainWindowHandler)
	{
		mainWindowHandler->ReleaseGLContext();
		delete mainWindowHandler;
		mainWindowHandler = nullptr;
	}
}

void MainWindow::Show()
{
	MainLoop();
}

void MainWindow::Init()
{
	mainWindowHandler->SetDefaultGLState();
	MaterialModel::InitStaticMaterials();

	orbiter.SetFrameWidth(mainWindowHandler->Width());
	orbiter.SetFrameHeight(mainWindowHandler->Height());
	orbiter.SetClippingPlanes(1.0f, 5000.0f);

	//InitNoiseTerrain();
	InitBasicTerrain();
	//InitLayerTerrain();
	//InitGPUTerrain();

	orbiter.LookAt(hfObject->GetComponent<MeshModel>()->GetBounds());
}

void MainWindow::MainLoop()
{
	while (mainWindowHandler->UpdateEvents())
	{
		Update(Time::GlobalTime(), Time::DeltaTime());
		Render();
		mainWindowHandler->SwapWindow();
	}
	Quit();
}

void MainWindow::Update(float time, float deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	float mxF = static_cast<float>(mx);
	float myF = static_cast<float>(my);
	if (mb & SDL_BUTTON(1))
		orbiter.Rotation(mxF, myF);
	if (mainWindowHandler->WheelEvent().y != 0)
		orbiter.Move(mainWindowHandler->WheelEvent().y * 10.0f);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation(mxF / mainWindowHandler->Width(), myF / mainWindowHandler->Height());

	/* Erosion Callbacks */
	if (mainWindowHandler->KeyState(SDLK_F1))
		StreamPowerErosionStep();
	if (mainWindowHandler->KeyState(SDLK_F2))
		ThermalErosionStep();
	if (mainWindowHandler->KeyState(SDLK_F3))
		HydraulicErosionStep();

	if (mainWindowHandler->ButtonEvent().button == SDL_BUTTON_LEFT && mainWindowHandler->KeyState(SDLK_LCTRL))
	{
		SDL_GetMouseState(&mx, &my);
		Ray ray = orbiter.PixelToRay(Vector2i(mx, my));
		Hit hit;
		if (hf->Intersect(ray, hit))
		{
			Vector2i v = hf->ToIndex2D(Vector2(hit.position.x, hit.position.z));
			hf->Add(v.x, v.y, 50.0);
			UpdateMeshRenderer();
		}
	}

	/* Example Scenes */
	if (mainWindowHandler->KeyState(SDLK_F8))
		InitBasicTerrain();
	if (mainWindowHandler->KeyState(SDLK_F9))
		InitNoiseTerrain();

	/* LayerField Callbacks */
	if (mainWindowHandler->KeyState(SDLK_F4))
		LightingImpact();

	/* Noise Callbacks */
	if (mainWindowHandler->KeyState(SDLK_UP))
		TranslateNoise(1, 0);
	if (mainWindowHandler->KeyState(SDLK_DOWN))
		TranslateNoise(-1, 0);
	if (mainWindowHandler->KeyState(SDLK_LEFT))
		TranslateNoise(0, 1);
	if (mainWindowHandler->KeyState(SDLK_RIGHT))
		TranslateNoise(0, -1);

	// Changing shader
	if (mainWindowHandler->KeyState(SDLK_k))
	{
		settings.shaderType = ShaderType::TerrainSplatmap;
		UpdateMeshRenderer();
	}
	else if (mainWindowHandler->KeyState(SDLK_l))
	{
		settings.shaderType = ShaderType::DiffuseGrey;
		UpdateMeshRenderer();
	}
	else if (mainWindowHandler->KeyState(SDLK_m))
	{
		settings.shaderType = ShaderType::SimpleTextured;
		UpdateMeshRenderer();
	}

	mainWindowHandler->ClearButtonEvent();
	mainWindowHandler->ClearAllKeyStates();
	mainWindowHandler->ClearWheelEvent();
	hfObject->UpdateTransformIfNeeded();
}

void MainWindow::Render()
{
	glClearColor(0.11f, 0.42f, 0.66f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hfObject->GetComponent<MeshRenderer>()->Render(orbiter);
}
