#include "mainwindow.h"
#include "mytime.h"
#include "imgui\imgui.h"
#include "imgui_opengl.h"


MainWindow::MainWindow(int windowWidth, int windowHeight)
{
	hfObject = nullptr;
	hf = nullptr;
	instanceRenderer = nullptr;
	mainWindowHandler = new Window(windowWidth, windowHeight);
	mainWindowHandler->CreateGLContext(4, 3);
	Init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Quit()
{
	Material::ReleaseStaticMaterials();
	ImGui_OpenGL_Shutdown();
	AppTime::Release();
	if (hf)
	{
		delete hf;
		hf = nullptr;
	}
	if (instanceRenderer)
	{
		delete instanceRenderer;
		instanceRenderer = nullptr;
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
	ImGui_OpenGL_Init(mainWindowHandler->GetSDLWindow());
	std::cout << "Dear ImGui Version : " << ImGui::GetVersion() << std::endl;
	Material::InitStaticMaterials();

	orbiter.SetFrameWidth(mainWindowHandler->Width());
	orbiter.SetFrameHeight(mainWindowHandler->Height());
	orbiter.SetClippingPlanes(1.0f, 5000.0f);

	//InstanceScene();
	InitGPUTerrain();
}

void MainWindow::MainLoop()
{
	while (mainWindowHandler->UpdateEvents())
	{
		ImGui_OpenGL_NewFrame(mainWindowHandler->GetSDLWindow());
		AppTime::StartClock();
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
	if (mb & SDL_BUTTON(1) && mainWindowHandler->KeyState(SDLK_LCTRL))
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
		//SDL_GetMouseState(&mx, &my);
		//Ray ray = orbiter.PixelToRay(Vector2i(mx, my));
		//Hit hit;
		//if (hf->Intersect(ray, hit))
		//{
		//	Vector2i v = hf->ToIndex2D(Vector2(hit.position.x, hit.position.z));
		//	hf->Add(v.x, v.y, 50.0);
		//	UpdateMeshRenderer();
		//	cout << "Hit at " << hit.position << endl;
		//}
	}

	/* Example Scenes */
	if (mainWindowHandler->KeyState(SDLK_F8))
		InitBasicTerrain();
	if (mainWindowHandler->KeyState(SDLK_F9))
		InitGPUTerrain();

	/* LayerField Callbacks */
	if (mainWindowHandler->KeyState(SDLK_F4))
		LightingImpact();

	/* Noise Callbacks */
	int offset = 10;
	if (mainWindowHandler->KeyState(SDLK_UP))
		TranslateNoise(offset, 0);
	if (mainWindowHandler->KeyState(SDLK_DOWN))
		TranslateNoise(-offset, 0);
	if (mainWindowHandler->KeyState(SDLK_LEFT))
		TranslateNoise(0, offset);
	if (mainWindowHandler->KeyState(SDLK_RIGHT))
		TranslateNoise(0, -offset);

	// Changing shader
	if (mainWindowHandler->KeyState(SDLK_1))
	{
		settings.shaderType = ShaderType::TerrainSplatmap;
		UpdateMeshRenderer();
	}
	else if (mainWindowHandler->KeyState(SDLK_2))
	{
		settings.shaderType = ShaderType::DiffuseGrey;
		UpdateMeshRenderer();
	}
	else if (mainWindowHandler->KeyState(SDLK_3))
	{
		settings.shaderType = ShaderType::SimpleTextured;
		UpdateMeshRenderer();
	}
	else if (mainWindowHandler->KeyState(SDLK_4))
	{
		settings.shaderType = ShaderType::Wireframe;
		UpdateMeshRenderer();
	}

	mainWindowHandler->ClearButtonEvent();
	mainWindowHandler->ClearWheelEvent();
	hfObject->UpdateTransformIfNeeded();
}

void MainWindow::Render()
{
	// World
	glClearColor(0.11f, 0.42f, 0.66f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hfObject->GetComponent<MeshRenderer>()->Render(orbiter);
	instanceRenderer->Render(orbiter);
	
	// GUI
	RenderGUI();
}
