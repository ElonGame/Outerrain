#include "app.h"
#include "gameobject.h"
#include "GL/glew.h"
#include "time.h"
#include "texture.h"
#include "imgui/imgui.h"
#include "imgui_opengl.h"
#include <chrono>
#include <sstream>

/* ImGui Textures*/
static GLuint slopeTexture;
static GLuint draignageTexture;
static GLuint wetnessTexture;
static GLuint streampowerTexture;
static GLuint accessibilityTexture;
static GLuint vegetationDensityTexture;
static GLuint m_time_query;

/* ImGui Erosion */
static int streamPowerErosionIteration = 1;
static float streamPowerErosionAmplitude = 2.0f;
static int thermalErosionIteration = 1;

// In Progress :
//  -Bug fix thermal Erosion (Nathan & Axel)					==> Debug
//  -InitFromNoise() (Nathan)									==> Debug
//  -Vegetation plusieurs espèces (Thomas)						==> Todo/Debug

// To do :
//  -Routes : regarder algo article galin/peytavie et adapter
//  -Villages (?)

// Bug fix :
//  -Intégrer FlyCam de Thomas


App::App(const int& width, const int& height, const int& major, const int& minor)
{
	window = CreateWindow(width, height);
	glContext = create_context(window, major, minor);
	currentItem = 0;
	windowWidth = static_cast<float>(width);
	windowHeight = static_cast<float>(height);;
}

int App::Init()
{
	// Init ImGui
	ImGui_OpenGL_Init(window);

	// Default gl state
	glClearDepthf(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Queries to GPU
	glGenQueries(1, &m_time_query);

	//InitSceneVegetationTerrain();
	//InitSceneLayerTerrain();
	InitSceneNoiseTerrain();

	return 1;
}

void App::Quit()
{
	ImGui_OpenGL_Shutdown();
	if (glContext)
		release_context(glContext);
	if (window)
		ReleaseWindow(window);
	glDeleteQueries(1, &m_time_query);
}

int App::Render()
{
	// Time query
	glBeginQuery(GL_TIME_ELAPSED, m_time_query);
	std::chrono::high_resolution_clock::time_point cpu_start = std::chrono::high_resolution_clock::now();

	// Scene Rendering
	glClearColor(0.3f, 0.55f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<GameObject*> objs = scene.GetAllChildren();
	for (int i = 0; i < objs.size(); i++)
		objs[i]->GetComponent<Mesh>()->Draw(orbiter);

	// CPU/GPU time computation
	std::chrono::high_resolution_clock::time_point cpu_stop = std::chrono::high_resolution_clock::now();
	long long int cpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(cpu_stop - cpu_start).count();
	glEndQuery(GL_TIME_ELAPSED);
	GLint64 gpu_time = 0;
	glGetQueryObjecti64v(m_time_query, GL_QUERY_RESULT, &gpu_time);
	std::stringstream cpuStr, gpuStr;
	cpuStr << "CPU " << static_cast<int>((cpu_time / 1000000)) << "ms" << static_cast<int>(((cpu_time / 1000) % 1000)) << "us";
	gpuStr << "GPU " << static_cast<int>((gpu_time / 1000000)) << "ms" << static_cast<int>(((gpu_time / 1000) % 1000)) << "us";

	// ImGui
	// Help
	ImGui::Begin("Welcome to Outerrain !");
	ImGui::Text("MOUSE : \n - Click Left to rotate \n - Click Middle to move\n - Click Right to zoom (in/out)");
	ImGui::Text("KEYBOARD : \n - Arrows to move\n - T to start Thermal Erosion\n - V to spawn vegetation");
	ImGui::End();
	// Shading
	ImGui::Begin("Shaders");
	const char* items[] = { "Diffuse", "Normal", "Wireframe" };
	ImGui::Combo("Shading", &currentItem, items, IM_ARRAYSIZE(items));
	ImGui::End();
	// Debug Image 
	ImGui::Begin("Slope Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(slopeTexture), ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Drainage Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(draignageTexture), ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Wetness Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(wetnessTexture), ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Stream Power Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(streampowerTexture), ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Accessibility Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(accessibilityTexture), ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Vegetation Density Map");
	ImGui::Image(reinterpret_cast<ImTextureID>(vegetationDensityTexture), ImVec2(150, 150));
	ImGui::End();

	// Erosion
	ImGui::Begin("Erosion");
	ImGui::Text("Stream Power Erosion");
	ImGui::SliderInt("Iterations 1", &streamPowerErosionIteration, 1, 100);
	ImGui::SliderFloat("Amplitude 1", &streamPowerErosionAmplitude, 0.1f, 10.0f);
	if (ImGui::Button("Compute") && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
	{
		vegTerrain.StreamPowerErosion(streamPowerErosionIteration, streamPowerErosionAmplitude);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(vegTerrain.GetAllVertices());
		CalculateAllMaps();
	}
	ImGui::Text("\n");
	ImGui::Text("Thermal Erosion");
	ImGui::SliderInt("Iterations 2", &thermalErosionIteration, 1, 100);
	if (ImGui::Button("Compute") && layerTerrain2D.SizeX() > 0 && layerTerrain2D.SizeY() > 0)
	{
		layerTerrain2D.ThermalErosion(thermalErosionIteration);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(layerTerrain2D.GetAllVertices());
	}
	ImGui::End();

	// Time Info
	ImGui::Begin("Rendering Time");
	ImGui::Text(cpuStr.str().data());
	ImGui::Text(gpuStr.str().data());
	ImGui::End();

	return 1;
}

int App::Update(const float time, const float deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	float mxF = static_cast<float>(mx);
	float myF = static_cast<float>(my);
	if (key_state(SDLK_LCTRL) && mb & SDL_BUTTON(1))
		orbiter.Rotation(mxF, myF);
	if (mb & SDL_BUTTON(3))
		orbiter.Move(myF);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation(mxF / windowWidth, myF / windowHeight);

	// Keyboard
	if (key_state(SDLK_PAGEUP))
		orbiter.Move(1.0f);
	if (key_state(SDLK_PAGEDOWN))
		orbiter.Move(-1.0f);
	if (key_state(SDLK_UP))
		orbiter.Translation(0.0f, 10.0f / windowHeight);
	if (key_state(SDLK_DOWN))
		orbiter.Translation(0.0f, -10.0f / windowHeight);
	if (key_state(SDLK_LEFT))
		orbiter.Translation(10.0f / windowWidth, 0.0f);
	if (key_state(SDLK_RIGHT))
		orbiter.Translation(-10.0f / windowWidth, 0.0f);

	// Thermal Erosion
	if (key_state(SDLK_t) && layerTerrain2D.SizeX() > 0 && layerTerrain2D.SizeY() > 0)
	{
		layerTerrain2D.ThermalErosion(thermalErosionIteration);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(layerTerrain2D.GetAllVertices());
	}
	// Stream Power erosion
	if (key_state(SDLK_p) && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
	{
		vegTerrain.StreamPowerErosion(streamPowerErosionIteration, 2.0f);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(vegTerrain.GetAllVertices());
		CalculateAllMaps();
	}
	// Vegetation spawn
	if (key_state(SDLK_v) && vegTerrain.SizeX() > 0 && vegTerrain.SizeY() > 0)
	{
		vegTerrain.ComputeVegetationDensities();
		std::vector<GameObject*> trees = vegTerrain.GetTreeObjects();
		for (int i = 0; i < trees.size(); i++)
			scene.AddChild(trees[i]);
		CalculateAllMaps();
	}

	// Update game objects
	UpdateObjects(time, deltaTime);
	scene.GetChildAt(0)->GetComponent<Mesh>()->SetRenderMode((RenderMode)currentItem);

	return 1;
}

void App::Run()
{
	if (Init() < 0)
		return;
	glViewport(0, 0, WindowWidth(), WindowHeight());
	while (Events(window))
	{
		ImGui_OpenGL_NewFrame(window);
		if (Update(Time::GlobalTime(), Time::DeltaTime()) < 0)
			break;
		if (Render() < 1)
			break;
		ImGui::Render();
		SDL_GL_SwapWindow(window);
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


/* Init Scenes */
void App::InitSceneNoiseTerrain()
{
	vegTerrain = VegetationTerrain(256, 256, Vector2(-256, -256), Vector2(256, 256));
	vegTerrain.InitFromNoise(0, 150);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	CalculateAllMaps();

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());
}

void App::InitSceneVegetationTerrain()
{
	vegTerrain = VegetationTerrain(256, 256, Vector2(-256, -256), Vector2(256, 256));
	vegTerrain.InitFromFile("Data/island.png", 0, 100);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	CalculateAllMaps();

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());
}

void App::InitSceneLayerTerrain()
{
	layerTerrain2D = LayerTerrain2D(256, 256, Vector2(-256, -256), Vector2(256, 256));
	layerTerrain2D.InitFromFile("Data/island.png", 0, 100, 0.8f);

	Mesh* mesh = layerTerrain2D.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Grey(), 0));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	CalculateAllMaps();

	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());
}

void App::CalculateAllMaps()
{
	vegTerrain.SlopeField().WriteImageGrayscale("Data/slope.png");
	vegTerrain.WetnessField().WriteImageGrayscale("Data/wetness.png");
	vegTerrain.StreamPowerField().WriteImageGrayscale("Data/streamPower.png");
	vegTerrain.DrainageSqrtField().WriteImageGrayscale("Data/drainageSqrt.png");
	vegTerrain.AccessibilityField().WriteImageGrayscale("Data/accessibility.png");
	vegTerrain.VegetationDensityField().WriteImageGrayscale("Data/vegetationDensity.png");
	slopeTexture = ReadTexture(0, "Data/slope.png", GL_RGB);
	draignageTexture = ReadTexture(0, "Data/drainageSqrt.png", GL_RGB);
	wetnessTexture = ReadTexture(0, "Data/wetness.png", GL_RGB);
	streampowerTexture = ReadTexture(0, "Data/streamPower.png", GL_RGB);
	accessibilityTexture = ReadTexture(0, "Data/accessibility.png", GL_RGB);
	vegetationDensityTexture = ReadTexture(0, "Data/vegetationDensity.png", GL_RGB);
}