#include "app.h"
#include "gameobject.h"
#include "GL/glew.h"
#include "mytime.h"
#include "texture.h"
#include "imgui/imgui.h"
#include "imgui_opengl.h"
#include "roads.h"

/* ImGui Textures*/
static GLuint slopeTexture;
static GLuint draignageTexture;
static GLuint wetnessTexture;
static GLuint streampowerTexture;
static GLuint accessibilityTexture;
static GLuint vegetationPineDensityTexture;
static GLuint vegetationBroadDensityTexture;

static Vector2 minMaxSlope;
static Vector2 minMaxDrainage;
static Vector2 minMaxWetness;
static Vector2 minMaxStreampower;
static Vector2 minMaxAccessibility;
static Vector2 minMaxVegetationDensity;

static GLuint m_time_query;

/* ImGui Erosion */
static int streamPowerErosionIteration = 1;
static float streamPowerErosionAmplitude = 2.0f;
static int thermalErosionIteration = 1;


// To do JV :
//  -BUG : Placement vegetation bords du terrain (Thomas, IsInsideField ?)
//  -Roads : Finir
//  -Faire une structure pour encapsuler les donn�es ImGui static de ce fichier

// To do Architecture : 
//  -Structure widget pour encapsuler imgui
//  -Structure pour les query GPU
//  -Structure diff�rente pour le rendu : Deferred Lighting, gestion des ressources diff�rentes.


App::App(const int& width, const int& height, const int& major, const int& minor)
{
	window = new Window(width, height);
	window->CreateGLContext(minor, major);

	currentItem = 0;
}

void App::Init()
{
	ImGui_OpenGL_Init(window->GetSDLWindow());
	window->SetDefaultGLState();

	// Queries to GPU
	glGenQueries(1, &m_time_query);

	InitSceneVegetationTerrain();
	//InitSceneLayerTerrain();
	//InitSceneNoiseTerrain();
}

void App::Quit()
{
	ImGui_OpenGL_Shutdown();
	delete window;
	glDeleteQueries(1, &m_time_query);
}

void App::Render()
{
	StartFrameTimeComputation();
	RenderScene();
	ComputeFrameTime();
}

void App::RenderScene()
{
	// Scene Rendering
	glClearColor(0.3f, 0.55f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<GameObject*> objs = scene.GetAllChildren();
	for (int i = 0; i < objs.size(); i++)
		objs[i]->GetComponent<Mesh>()->Draw(orbiter);
}

void App::RenderGUI()
{
	// ImGui
	// Help
	ImGui::Begin("Welcome to Outerrain !");
	ImGui::Text("MOUSE : \n - Click Left to rotate \n - Click Middle to move\n - Click Right to zoom (in/out)");
	ImGui::Text("KEYBOARD : \n - Arrows to move\n - T to start Thermal Erosion\n - V to spawn vegetation");
	ImGui::End();
	// Shading
	ImGui::Begin("Shaders");
	const char* items[] = { "Diffuse", "Normal", "Wireframe", "Slope Texture", "Drainage Texture", "Wetness Texture", "Accessibility Texture", "Stream Power Texture" };
	ImGui::Combo("Shading", &currentItem, items, IM_ARRAYSIZE(items));
	ImGui::End();
	// Debug Image 
	ImGui::Begin("Maps");
	ImGui::SetNextWindowContentWidth(1000);
	ImGui::BeginChild("", ImVec2(0, 190), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Columns(7);
	ImGui::Text("Slope");
	ImGui::Image(reinterpret_cast<ImTextureID>(slopeTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxSlope.x);
	ImGui::Value("White", minMaxSlope.y);
	ImGui::SetColumnWidth(0, 135);
	ImGui::NextColumn();
	ImGui::Text("Drainage");
	ImGui::Image(reinterpret_cast<ImTextureID>(draignageTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxDrainage.x);
	ImGui::Value("White", minMaxDrainage.y);
	ImGui::SetColumnWidth(1, 135);
	ImGui::NextColumn();
	ImGui::Text("Wetness");
	ImGui::Image(reinterpret_cast<ImTextureID>(wetnessTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxWetness.x);
	ImGui::Value("White", minMaxWetness.y);
	ImGui::SetColumnWidth(2, 135);
	ImGui::NextColumn();
	ImGui::Text("Stream Power");
	ImGui::Image(reinterpret_cast<ImTextureID>(streampowerTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxStreampower.x);
	ImGui::Value("White", minMaxStreampower.y);
	ImGui::SetColumnWidth(3, 135);
	ImGui::NextColumn();
	ImGui::Text("Accessibility");
	ImGui::Image(reinterpret_cast<ImTextureID>(accessibilityTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxAccessibility.x);
	ImGui::Value("White", minMaxAccessibility.y);
	ImGui::SetColumnWidth(4, 135);
	ImGui::NextColumn();
	ImGui::Text("Pine Density");
	ImGui::Image(reinterpret_cast<ImTextureID>(vegetationPineDensityTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxVegetationDensity.x);
	ImGui::Value("White", minMaxVegetationDensity.y);
	ImGui::SetColumnWidth(5, 135);
	ImGui::NextColumn();
	ImGui::Text("Broadleaf Density");
	ImGui::Image(reinterpret_cast<ImTextureID>(vegetationBroadDensityTexture), ImVec2(120, 120), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Value("Black", minMaxVegetationDensity.x);
	ImGui::Value("White", minMaxVegetationDensity.y);
	ImGui::SetColumnWidth(6, 135);
	ImGui::Columns(1);
	ImGui::EndChild();
	ImGui::End();
	// Erosion
	ImGui::Begin("Erosion");
	ImGui::Text("Stream Power Erosion");
	ImGui::SliderInt("Iterations 1", &streamPowerErosionIteration, 1, 100);
	ImGui::SliderFloat("Amplitude 1", &streamPowerErosionAmplitude, 0.1f, 10.0f);
	if (ImGui::Button("Compute SP Erosion") && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
	{
		StreamPowerErosionCallback(streamPowerErosionIteration, streamPowerErosionAmplitude);
	}
	ImGui::Text("\n");
	ImGui::Text("Thermal Erosion");
	ImGui::SliderInt("Iterations 2", &thermalErosionIteration, 1, 100);
	if (ImGui::Button("Compute Thermal Erosion"))
	{
		ThermalErosionCallback(thermalErosionIteration);
	}
	ImGui::End();

	// Time Info
	ImGui::Begin("Rendering Time");
	//ImGui::Text(cpuStr.str().data());
	ImGui::Text(gpuStr.str().data());
	ImGui::End();

	// Call ImGui renderer
	ImGui::Render();
}

void App::StartFrameTimeComputation()
{
	glBeginQuery(GL_TIME_ELAPSED, m_time_query);
	//cpu_start = std::chrono::high_resolution_clock::now();
}

void App::ComputeFrameTime()
{
	// CPU/GPU time computation
	/*cpu_stop = std::chrono::high_resolution_clock::now();
	long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();
	*/
	glEndQuery(GL_TIME_ELAPSED);
	GLint64 gpu_time = 0;
	glGetQueryObjecti64v(m_time_query, GL_QUERY_RESULT, &gpu_time);

	//cpuStr.str("");
	gpuStr.str("");
	//cpuStr << "CPU " << static_cast<int>((cpu_time / 1000000)) << "ms" << static_cast<int>(((cpu_time / 1000) % 1000)) << "us";
	gpuStr << "GPU " << static_cast<int>((gpu_time / 1000000)) << "ms" << static_cast<int>(((gpu_time / 1000) % 1000)) << "us";
}

int App::Update(const float time, const float deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	float mxF = static_cast<float>(mx);
	float myF = static_cast<float>(my);
	if (window->KeyState(SDLK_LCTRL) && mb & SDL_BUTTON(1))
		orbiter.Rotation(mxF, myF);
	if (mb & SDL_BUTTON(3))
		orbiter.Move(myF);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation(mxF / windowWidth, myF / windowHeight);

	// Keyboard
	if (window->KeyState(SDLK_PAGEUP))
		orbiter.Move(1.0f);
	if (window->KeyState(SDLK_PAGEDOWN))
		orbiter.Move(-1.0f);
	if (window->KeyState(SDLK_UP))
		orbiter.Translation(0.0f, 10.0f / windowHeight);
	if (window->KeyState(SDLK_DOWN))
		orbiter.Translation(0.0f, -10.0f / windowHeight);
	if (window->KeyState(SDLK_LEFT))
		orbiter.Translation(10.0f / windowWidth, 0.0f);
	if (window->KeyState(SDLK_RIGHT))
		orbiter.Translation(-10.0f / windowWidth, 0.0f);

	// Thermal Erosion
	if (window->KeyState(SDLK_t))
		ThermalErosionCallback(thermalErosionIteration);

	// Stream Power erosion
	if (window->KeyState(SDLK_p) && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
		StreamPowerErosionCallback(streamPowerErosionIteration, streamPowerErosionAmplitude);

	// Vegetation spawn
	if (window->KeyState(SDLK_v) && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
		SpawnVegetationCallback();

	// Roads
	if (window->KeyState(SDLK_r) && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
		GenerateRoadCallback();

	// Update game objects
	UpdateObjects(time, deltaTime);
	RenderMode r = (currentItem > 3) ? Texture : (RenderMode)currentItem;
	scene.GetChildAt(0)->GetComponent<Mesh>()->SetRenderMode(r);
	switch (currentItem)
	{
	case 0:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(accessibilityTexture);
		break;
	case 3:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(slopeTexture);
		break;
	case 4:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(draignageTexture);
		break;
	case 5:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(wetnessTexture);
		break;
	case 6:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(accessibilityTexture);
		break;
	case 7:
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetTexture(streampowerTexture);
		break;
	}

	return 1;
}

void App::Run()
{
	Init();
	glViewport(0, 0, window->Width(), window->Height());
	while (window->UpdateEvents())
	{
		ImGui_OpenGL_NewFrame(window->GetSDLWindow());
		if (Update(Time::GlobalTime(), Time::DeltaTime()) < 0)
			break;
		Render();
		RenderGUI();
		SDL_GL_SwapWindow(window->GetSDLWindow());
	}
	Quit();
}

void App::UpdateObjects(const float time, const float delta)
{
	newTime = SDL_GetPerformanceCounter();
	float delta2 = static_cast<float>(((newTime - lastTime) * 1000.0f)) / SDL_GetPerformanceFrequency();
	std::vector<GameObject*> objs = scene.GetAllChildren();
	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->UpdateTransformIfNeeded();
		std::vector<Component*> components = objs[i]->GetAllComponents();
		for (int j = 0; j < components.size(); j++)
			components[j]->Update(delta2);
	}
	lastTime = SDL_GetPerformanceCounter();
}

void App::CalculateAllMaps()
{
	ScalarField2D field = vegTerrain.SlopeField();
	field.WriteImageGrayscale("Data/Maps/slope.png");
	minMaxSlope = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.WetnessField();
	field.WriteImageGrayscale("Data/Maps/wetness.png");
	minMaxWetness = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.StreamPowerField();
	field.WriteImageGrayscale("Data/Maps/streamPower.png");
	minMaxStreampower = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.DrainageField();
	field.WriteImageGrayscale("Data/Maps/drainageSqrt.png");
	minMaxDrainage = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.AccessibilityField();
	field.WriteImageGrayscale("Data/Maps/accessibility.png");
	minMaxAccessibility = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.VegetationDensityField(0);
	field.WriteImageGrayscale("Data/Maps/vegetationPineDensity.png");
	minMaxVegetationDensity = Vector2(field.MinValue(), field.MaxValue());

	field = vegTerrain.VegetationDensityField(0);
	field.WriteImageGrayscale("Data/Maps/vegetationBroadDensity.png");
	minMaxVegetationDensity = Vector2(field.MinValue(), field.MaxValue());

	slopeTexture = ReadTexture(0, "Data/Maps/slope.png", GL_RGB);
	draignageTexture = ReadTexture(0, "Data/Maps/drainageSqrt.png", GL_RGB);
	wetnessTexture = ReadTexture(0, "Data/Maps/wetness.png", GL_RGB);
	streampowerTexture = ReadTexture(0, "Data/Maps/streamPower.png", GL_RGB);
	accessibilityTexture = ReadTexture(0, "Data/Maps/accessibility.png", GL_RGB);
	vegetationPineDensityTexture = ReadTexture(0, "Data/Maps/vegetationPineDensity.png", GL_RGB);
	vegetationBroadDensityTexture = ReadTexture(0, "Data/Maps/vegetationBroadDensity.png", GL_RGB);
}
