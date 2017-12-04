#include "app.h"
#include "GL/glew.h"
#include "fields.h"

App::App(const int& width, const int& height, const int& major, const int& minor) 
		: m_window(nullptr), m_context(nullptr)
{
	m_window = CreateWindow(width, height);
	m_context = create_context(m_window, major, minor);
}

App::~App()
{
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

	int size = 2048;
	Heightfield hf(64, 64, Vector2(-size, -size), Vector2(size, size));
	hf.InitFromNoise(0, 100);
	mesh = hf.GetMesh();
	//mesh.read_mesh("Data/bigguy.obj");

	Shader shader;
	shader.InitFromFile("Shaders/Diffuse.glsl");
	mesh.SetShader(shader);
	
	orbiter.LookAt(mesh.GetBounds());
	orbiter.SetFrameWidth(WindowWidth());
	orbiter.SetFrameHeight(WindowHeight());

	return 1;
}

void App::Quit()
{
}

int App::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
