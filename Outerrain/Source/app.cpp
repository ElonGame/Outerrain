#include "app.h"
#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui_opengl.h"

App::App(const int& width, const int& height, const int& major, const int& minor)
	: m_window(nullptr), m_context(nullptr)
{
	m_window = CreateWindow(width, height);
	m_context = create_context(m_window, major, minor);
	InitImGUI();
}

App::~App()
{
	ImGui_OpenGL_Shutdown();
	if (m_context)
		release_context(m_context);
	if (m_window)
		ReleaseWindow(m_window);
}

int App::Init()
{
	// Default gl state
	glClearDepthf(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	terrain2D = Terrain2D(256, 32, Vector2(-64, -64), Vector2(64, 64));
	terrain2D.InitFromFile("Data/circuit.png", 0.0f, 7.0f);
	mesh = terrain2D.GetMesh();

	// Init Mesh
	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh.SetShader(shader);

	// Init Shader
	orbiter.LookAt(mesh.GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());

	ImGui::Begin("Bienvenue dans Outerrain !");
	ImGui::End();

	return 1;
}

void App::InitImGUI()
{
	ImGui_OpenGL_Init(m_window);
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_OpenGL_ProcessEvent(&event);
	}
	ImGui_OpenGL_NewFrame(m_window);
}

void App::Quit()
{
}

int App::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Render();
	mesh.Draw(orbiter);
	return 1;
}

int App::Update(const float time, const float deltaTime)
{
	// Deplace la camera
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

	return 1;
}

void App::Run()
{
	if (Init() < 0)
		return;

	glViewport(0, 0, WindowWidth(), WindowHeight());
	while (Events(m_window))
	{
		if (Update(GlobalTime(), DeltaTime()) < 0)
			break;
		if (Render() < 1)
			break;
		SDL_GL_SwapWindow(m_window);
	}

	Quit();
}
