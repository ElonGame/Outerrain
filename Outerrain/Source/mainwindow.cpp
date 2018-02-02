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
		delete hf;
	if (mainWindowHandler)
	{
		mainWindowHandler->ReleaseGLContext();
		delete mainWindowHandler;
	}
}

void MainWindow::Show()
{

}


void MainWindow::Init()
{
	mainWindowHandler->SetDefaultGLState();

	orbiter.SetFrameWidth(mainWindowHandler->Width());
	orbiter.SetFrameHeight(mainWindowHandler->Height());
	orbiter.SetClippingPlanes(1.0f, 3000.0f);
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
}

void MainWindow::Render()
{

}