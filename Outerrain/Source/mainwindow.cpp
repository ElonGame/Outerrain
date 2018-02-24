#include "mainwindow.h"
#include "mytime.h"

/*
\todolist
	-Support texture in terrain shader
	-Integrate ImGui && make a basic interface
	-Ridge noise, and refactor to make a base abstract class Noise
	-Fix memory leak
	-Continue Vegetation support
*/

MainWindow::MainWindow(int windowWidth, int windowHeight)
{
	hfObject = nullptr;
	hf = nullptr;
	mainWindowHandler = new Window(windowWidth, windowHeight);
	mainWindowHandler->CreateGLContext(4, 4);
	Init();
}

MainWindow::~MainWindow()
{
	Quit();
}

void MainWindow::Quit()
{
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

	orbiter.SetFrameWidth(mainWindowHandler->Width());
	orbiter.SetFrameHeight(mainWindowHandler->Height());
	orbiter.SetClippingPlanes(1.0f, 3000.0f);

	InitBasicTerrain();
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

	if (mainWindowHandler->KeyState(SDLK_F1))
		StreamPowerErosionStep();
	if (mainWindowHandler->KeyState(SDLK_F2))
		ThermalErosionStep();

	mainWindowHandler->ClearKeyState(SDLK_a);
	mainWindowHandler->ClearWheelEvent();
	hfObject->UpdateTransformIfNeeded();
}

void MainWindow::Render()
{
	glClearColor(0.11f, 0.42f, 0.66f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hfObject->GetComponent<MeshRenderer>()->Render(orbiter);
}
