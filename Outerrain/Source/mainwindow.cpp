#include "mainwindow.h"
#include "mytime.h"


MainWindow::MainWindow(const int& windowWidth, const int& windowHeight)
{
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

void MainWindow::Update(const float& time, const float& deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	float mxF = static_cast<float>(mx);
	float myF = static_cast<float>(my);
	if (mainWindowHandler->KeyState(SDLK_LCTRL) && mb & SDL_BUTTON(1))
		orbiter.Rotation(mxF, myF);
	if (mb & SDL_BUTTON(3))
		orbiter.Move(myF);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation(mxF / mainWindowHandler->Width(), myF / mainWindowHandler->Height());

	hfObject->UpdateTransformIfNeeded();
}

void MainWindow::Render()
{
	glClearColor(0.3f, 0.55f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hfObject->GetComponent<MeshRenderer>()->Render(orbiter);
	//hfObject->GetComponent<Mesh>()->Draw(orbiter);
}
