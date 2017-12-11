#include "app.h"
#include "GL/glew.h"
#include "time.h"

#include "imgui/imgui.h"
#include "imgui_opengl.h"

App::App(const int& width, const int& height, const int& major, const int& minor)
	: window(nullptr), glContext(nullptr)
{
	window = CreateWindow(width, height);
	glContext = create_context(window, major, minor);
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
	vegTerrain.InitFromFile("Data/circuit.png", 0.0f, 7.0f);

	Mesh* mesh = vegTerrain.GetMesh();
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh->SetShader(shader);

	GameObject* obj = new GameObject();
	obj->AddComponent(mesh);
	scene.AddChild(obj);

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
	ImGui::Begin("Welcome to Outerrain !");
	ImGui::Text("MOUSE : \n - Click Left to rotate \n - Click Middle to move\n - Click Right to zoom (in/out)");
	ImGui::Text("KEYBOARD : \n - Arrows to move\n - T to start Thermal Erosion");
	ImGui::End();

	return 1;
}

int App::Update(const float time, const float deltaTime)
{
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	if (mb & SDL_BUTTON(1))
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
		layerTerrain2D.ThermalErosion(1);
	// Vegetation spawn
	if (key_state(SDLK_v))
	{
		vegTerrain.ComputeDensities();
		vegTerrain.ComputeInstances();
		std::vector<GameObject*> trees = vegTerrain.GetTreeObjects();
		for (int i = 0; i < trees.size(); i++)
			scene.AddChild(trees[i]);
	}

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
