#include <sys/types.h>
#include <sys/stat.h>
#include <cassert>
#include <cstdio>

#include <set>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "GL/glew.h"
#include "window.h"


int Window::KeyState(const SDL_Keycode& key)
{
	SDL_Scancode code = SDL_GetScancodeFromKey(key);
	assert((size_t)code < key_states.size());
	return (int)key_states[code];
}

void Window::ClearKeyState(const SDL_Keycode& key)
{
	SDL_Scancode code = SDL_GetScancodeFromKey(key);
	assert((size_t)code < key_states.size());
	key_states[code] = 0;
}

SDL_KeyboardEvent Window::KeyEvent()
{
	return last_key;
}

void Window::ClearKeyEvent()
{
	last_key.type = 0;
	last_key.keysym.sym = 0;
}

SDL_TextInputEvent Window::TextEvent()
{
	return last_text;
}

void Window::ClearTextEvent()
{
	last_text.text[0] = 0;
}

SDL_MouseButtonEvent Window::ButtonEvent()
{
	return last_button;
}

void Window::ClearButtonEvent()
{
	last_button.state = 0;
}

SDL_MouseWheelEvent Window::WheelEvent()
{
	return last_wheel;
}

void Window::ClearWheelEvent()
{
	last_wheel.x = 0;
	last_wheel.y = 0;
}

int Window::UpdateEvents()
{
	// gestion des evenements
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				width = event.window.data1;
				height = event.window.data2;
				SDL_SetWindowSize(windowSDL, width, height);
				glViewport(0, 0, width, height);
			}
			break;

		case SDL_TEXTINPUT:
			last_text = event.text;
			break;

		case SDL_KEYDOWN:
			if ((size_t)event.key.keysym.scancode < key_states.size())
			{
				key_states[event.key.keysym.scancode] = 1;
				last_key = event.key;
			}

			if (event.key.keysym.sym == SDLK_ESCAPE)
				stop = 1;
			break;

		case SDL_KEYUP:
			if ((size_t)event.key.keysym.scancode < key_states.size())
			{
				key_states[event.key.keysym.scancode] = 0;
				last_key = event.key;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			last_button = event.button;
			break;

		case SDL_MOUSEWHEEL:
			last_wheel = event.wheel;
			break;

		case SDL_QUIT:
			stop = 1;
			break;
		}
	}

	return 1 - stop;
}


#ifndef NO_GLEW
#ifndef GK_RELEASE
static
void GLAPIENTRY debug(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
	const char *message, const void *userParam)
{
	static std::set<std::string> log;
	if (log.insert(message).second == false)
		// le message a deja ete affiche, pas la peine de recommencer 60 fois par seconde.
		return;

	if (severity == GL_DEBUG_SEVERITY_HIGH)
		printf("[openGL error]\n%s\n", message);
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
		printf("[openGL warning]\n%s\n", message);
	else
		printf("[openGL message]\n%s\n", message);
}
#endif
#endif

Window::Window(const int& w, const int& h) : width(width), height(h), stop(0)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("[error] SDL_Init() failed:\n%s\n", SDL_GetError());
		return;
	}

	atexit(SDL_Quit);

	windowSDL = SDL_CreateWindow("Outerrain",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (windowSDL == NULL)
	{
		printf("[error] SDL_CreateWindow() failed.\n");
		return;
	}

	int keys;
	const unsigned char *state = SDL_GetKeyboardState(&keys);
	key_states.assign(state, state + keys);

	SDL_SetWindowDisplayMode(windowSDL, NULL);
	SDL_StartTextInput();
	SDL_GetWindowSize(windowSDL, &width, &height);
}

Window::~Window()
{
	ReleaseGLContext();
	SDL_StopTextInput();
	SDL_DestroyWindow(windowSDL);
}

void Window::CreateGLContext(const int& major, const int& minor)
{
	if (windowSDL == NULL)
		return;

	// configure la creation du contexte opengl core profile, debug profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
#ifndef GK_RELEASE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 15);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glContext = SDL_GL_CreateContext(windowSDL);
	if (glContext == NULL)
	{
		printf("[error] creating openGL context.\n");
		return;
	}

	SDL_GL_SetSwapInterval(1);

#ifndef NO_GLEW
	// initialise les extensions opengl
	glewExperimental = 1;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("[error] loading extensions\n%s\n", glewGetErrorString(err));
		SDL_GL_DeleteContext(glContext);
		return;
	}

	// purge les erreurs opengl generees par glew !
	while (glGetError() != GL_NO_ERROR) { ; }

#ifndef GK_RELEASE
	// configure l'affichage des messages d'erreurs opengl, si l'extension est disponible
	if (GLEW_ARB_debug_output)
	{
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallbackARB(debug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
#endif
#endif
}

void Window::ReleaseGLContext()
{
	SDL_GL_DeleteContext(glContext);
}
