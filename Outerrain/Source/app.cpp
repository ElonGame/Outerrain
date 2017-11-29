#include "app.h"
#include "GL/glew.h"

App::App(const int& width, const int& height, const int& major, const int& minor) 
		: m_window(nullptr), m_context(nullptr)
{
	m_window = create_window(width, height);
	m_context = create_context(m_window, major, minor);
}

App::~App()
{
	if (m_context)
		release_context(m_context);
	if (m_window)
		release_window(m_window);
}

int App::Init()
{
	// Default gl state
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClearDepthf(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	return 1;
}

void App::Quit()
{
}

int App::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	return 1;
}

int App::Update(const float time, const float deltaTime)
{
	return 1;
}

void App::Run()
{
	if (Init() < 0)
		return;

	glViewport(0, 0, window_width(), window_height());
	while (events(m_window))
	{
		if (Update(global_time(), delta_time()) < 0)
			break;
		if (Render() < 1)
			break;
		SDL_GL_SwapWindow(m_window);
	}

	Quit();
}
