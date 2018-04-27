#include "mainwindow.h"
#include "mytime.h"
#include "imgui/imgui.h"
#include "imgui_opengl.h"


MainWindow::MainWindow(int windowWidth, int windowHeight)
{
	hf = nullptr;
	mainWindowHandler = new Window(windowWidth, windowHeight);
	Init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Quit()
{
	MaterialBase::ReleaseStaticMaterials();
	ImGui_OpenGL_Shutdown();
	AppStatistics::Release();
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
	ImGui_OpenGL_Init(mainWindowHandler->GetSDLWindow());
	std::cout << "Dear ImGui Version : " << ImGui::GetVersion() << std::endl;
	MaterialBase::InitStaticMaterials();

	orbiter.SetFrameWidth(mainWindowHandler->Width());
	orbiter.SetFrameHeight(mainWindowHandler->GetValueBilinear());
	orbiter.SetClippingPlanes(0.1f, 10000.0f);

	InitBasicTerrain();
	orbiter.LookAt(hierarchy.GetObject(0)->GetComponent<Mesh>()->GetBounds());
}

void MainWindow::MainLoop()
{
	while (mainWindowHandler->UpdateEvents())
	{
		ImGui_OpenGL_NewFrame(mainWindowHandler->GetSDLWindow());
		AppStatistics::StartClock();
		Update();
		Render();
		mainWindowHandler->SwapWindow();
	}
	Quit();
}

void MainWindow::Update()
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	float mxF = float(mx);
	float myF = float(my);
	if (mb & SDL_BUTTON(1) && mainWindowHandler->KeyState(SDLK_LCTRL))
		orbiter.Rotation(mxF, myF);
	if (mainWindowHandler->WheelEvent().y != 0)
		orbiter.Move(mainWindowHandler->WheelEvent().y * 10.0f);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation(mxF / mainWindowHandler->Width(), myF / mainWindowHandler->GetValueBilinear());

	/* Erosion Callbacks */
	if (mainWindowHandler->KeyState(SDLK_F1))
		StreamPowerErosionStep();
	if (mainWindowHandler->KeyState(SDLK_F2))
		ThermalErosionStep();
	if (mainWindowHandler->KeyState(SDLK_F3))
		HydraulicErosionStep();

	if (mainWindowHandler->ButtonEvent().button == SDL_BUTTON_LEFT && mainWindowHandler->KeyState(SDLK_LCTRL) && mainWindowHandler->KeyState(SDLK_LSHIFT))
	{
		SDL_GetMouseState(&mx, &my);
		Ray ray = orbiter.PixelToRay(Vector2i(mx, my));
		std::cout << "Ray : " << ray << std::endl;
		Box b = hierarchy.GetObject(0)->GetComponent<Mesh>()->GetBounds();
		float tmin, tmax;
		if (b.Intersect(ray, tmin, tmax))
		{
			AddCube(ray.At(tmin), 0.2);
		}
	}

	/* Example Scenes */
	if (mainWindowHandler->KeyState(SDLK_F8))
		InitBasicTerrain();
	if (mainWindowHandler->KeyState(SDLK_F9))
		InitGPUTerrain();

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
		settings.shaderType = MaterialType::TerrainSplatmapMaterial;
		UpdateMeshMaterial();
	}
	else if (mainWindowHandler->KeyState(SDLK_2))
	{
		settings.shaderType = MaterialType::DiffuseMaterial;
		UpdateMeshMaterial();
	}
	else if (mainWindowHandler->KeyState(SDLK_3))
	{
		settings.shaderType = MaterialType::SingleTexturedMaterial;
		UpdateMeshMaterial();
	}
	else if (mainWindowHandler->KeyState(SDLK_4))
	{
		settings.shaderType = MaterialType::NormalMaterial;
		UpdateMeshMaterial();
	}
	else if (mainWindowHandler->KeyState(SDLK_5))
	{
		settings.shaderType = MaterialType::WireframeMaterial;
		UpdateMeshMaterial();
	}

	mainWindowHandler->ClearButtonEvent();
	mainWindowHandler->ClearWheelEvent();
	auto objs = hierarchy.GetAllObjects();
	for (size_t i = 0; i < objs.size(); i++)
		objs[i]->UpdateTransformIfNeeded();
}

void MainWindow::Render()
{
	// World
	glClearColor(0.11f, 0.42f, 0.66f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto objs = hierarchy.GetAllObjects();
	for (size_t i = 0; i < objs.size(); i++)
	{
		MeshRenderer* renderer = objs[i]->GetComponent<MeshRenderer>();
		if (renderer != nullptr)
			renderer->Render(orbiter);
	}
		
	// GUI
	RenderGUI();
}
