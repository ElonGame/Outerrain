#include "app.h"
#include "GL/glew.h"
#include "time.h"
#include "texture.h"
#include "imgui/imgui.h"
#include "imgui_opengl.h"

static GLuint draignageTexture;
static GLuint wetnessTexture;
static GLuint streampowerTexture;
static GLuint accessibilityTexture;

// In Progress :
//  -Thermal Erosion (Nathan & Axel)							==> Debug
//  -StreamPower Erosion (Axel)									==> Todo
//  -Accessibility Field (Vincent)								==> Debug
//  -InitFromNoise() (Nathan)									==> Debug
//  -Vegetation bug fix placement + plusieurs esp�ces (Thomas)	==> Todo/Debug

// To do :
//  -Routes
//  -Villages (?)

// Bug fix :
//  -Affichage texture dans imgui
//  -Release CameraOrbiter:: compile errors

App::App(const int& width, const int& height, const int& major, const int& minor)
	: window(nullptr), glContext(nullptr)
{
	window = CreateWindow(width, height);
	glContext = create_context(window, major, minor);
	currentItem = 0;
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

	vegTerrain = VegetationTerrain(256, 256, Vector2(-64, -64), Vector2(64, 64));
	vegTerrain.InitFromFile("Data/island.png", 0.0f, 20.0);
	//layerTerrain2D = LayerTerrain2D(256, 256, Vector2(-64, -64), Vector2(64, 64));
	//layerTerrain2D.InitFromFile("Data/island.png", 0.0f, 20.0f, 0.8f);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);
	mesh->SetMaterial(Material(Color::Blue(), 32));
	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

	// Maps
	vegTerrain.WetnessField().WriteImageGrayscale("Data/wetness.png");
	vegTerrain.StreamPowerField().WriteImageGrayscale("Data/streamPower.png");
	vegTerrain.DrainageSqrtField().WriteImageGrayscale("Data/drainageSqrt.png");
	vegTerrain.AccessibilityField().WriteImageGrayscale("Data/accessibility.png");
	draignageTexture = ReadTexture(0, "Data/drainageSqrt.png", GL_RGB);
	wetnessTexture = ReadTexture(0, "Data/wetness.png", GL_RGB);
	streampowerTexture = ReadTexture(0, "Data/streamPower.png", GL_RGB);
	accessibilityTexture = ReadTexture(0, "Data/accessibility.png", GL_RGB);

	// Init Shader
	orbiter.LookAt(mesh->GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());

	return 1;
}

void App::Quit()
{
	ImGui_OpenGL_Shutdown();
	if (glContext)
		release_context(glContext);
	if (window)
		ReleaseWindow(window);
}

int App::Render()
{
	// Clear
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Scene
	std::vector<GameObject*> objs = scene.GetAllChildren();
	for (int i = 0; i < objs.size(); i++)
		objs[i]->GetComponent<Mesh>()->Draw(orbiter);

	// ImGui
	// Help
	ImGui::Begin("Welcome to Outerrain !");
	ImGui::Text("MOUSE : \n - Click Left to rotate \n - Click Middle to move\n - Click Right to zoom (in/out)");
	ImGui::Text("KEYBOARD : \n - Arrows to move\n - T to start Thermal Erosion");
	ImGui::End();

	// Shading
	ImGui::Begin("Shaders"); 
	const char* items[] = { "Diffuse", "Normal", "Wireframe" };
	ImGui::Combo("Shading", &currentItem, items, IM_ARRAYSIZE(items));
	ImGui::End();

	// Debug Image 
	ImGui::Begin("Drainage Map");
	ImGui::Image((void*)draignageTexture, ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Wetness Map");
	ImGui::Image((GLuint*)wetnessTexture, ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Stream Power Map");
	ImGui::Image((void*)streampowerTexture, ImVec2(150, 150));
	ImGui::End();
	ImGui::Begin("Accessibility Image");
	ImGui::Image((void*)accessibilityTexture, ImVec2(150, 150));
	ImGui::End();
	return 1;
}

int App::Update(const float time, const float deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	if (key_state(SDLK_LCTRL) && mb & SDL_BUTTON(1))
		orbiter.Rotation(mx, my);
	if (mb & SDL_BUTTON(3))
		orbiter.Move(my);
	if (mb & SDL_BUTTON(2))
		orbiter.Translation((float)mx / (float)WindowWidth(), (float)my / (float)WindowHeight());

	// Keyboard
	if (key_state(SDLK_PAGEUP))
		orbiter.Move(1.0f);
	if (key_state(SDLK_PAGEDOWN))
		orbiter.Move(-1.0f);
	if (key_state(SDLK_UP))
		orbiter.Translation(0.0f, 10.0f / (float)WindowHeight());
	if (key_state(SDLK_DOWN))
		orbiter.Translation(0.0f, -10.0f / (float)WindowHeight());
	if (key_state(SDLK_LEFT))
		orbiter.Translation(10.0f / (float)WindowWidth(), 0.0f);
	if (key_state(SDLK_RIGHT))
		orbiter.Translation(-10.0f / (float)WindowWidth(), 0.0f);

	// Thermal Erosion
	if (key_state(SDLK_t) && layerTerrain2D.SizeX() > 0 && layerTerrain2D.SizeY() > 0)
	{
		layerTerrain2D.ThermalErosion(100);
		scene.GetChildAt(0)->GetComponent<Mesh>()->SetVertices(layerTerrain2D.GetAllVertices());
	}
	// Vegetation spawn
	if (key_state(SDLK_v))
	{
		vegTerrain.ComputeVegetationDensities();
		std::vector<GameObject*> trees = vegTerrain.GetTreeObjects();
		for (int i = 0; i < trees.size(); i++)
			scene.AddChild(trees[i]);
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
	float delta2 = (double)((newTime - lastTime) * 1000) / SDL_GetPerformanceFrequency();
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
