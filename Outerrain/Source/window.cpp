#include <sys/types.h>
#include <sys/stat.h>

#include <cassert>
#include <cstdio>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "GL/glew.h"
#include "window.h"


static int width = 0;
static int height = 0;
int window_width()
{
	return width;
}
int window_height()
{
	return height;
}

static std::vector<unsigned char> key_states;
int key_state(const SDL_Keycode key)
{
	SDL_Scancode code = SDL_GetScancodeFromKey(key);
	assert((size_t)code < key_states.size());
	return (int)key_states[code];
}
void clear_key_state(const SDL_Keycode key)
{
	SDL_Scancode code = SDL_GetScancodeFromKey(key);
	assert((size_t)code < key_states.size());
	key_states[code] = 0;
}

static SDL_KeyboardEvent last_key;
SDL_KeyboardEvent key_event()
{
	return last_key;
}
void clear_key_event()
{
	last_key.type = 0;
	last_key.keysym.sym = 0;
}

static SDL_TextInputEvent last_text;
SDL_TextInputEvent text_event()
{
	return last_text;
}
void clear_text_event()
{
	last_text.text[0] = 0;
}

static std::string last_drop;
const char *drop_event()
{
	return last_drop.c_str();
}
void clear_drop_event()
{
	last_drop.clear();
}

static SDL_MouseButtonEvent last_button;
SDL_MouseButtonEvent button_event()
{
	return last_button;
}
void clear_button_event()
{
	last_button.state = 0;
}

static SDL_MouseWheelEvent last_wheel;
SDL_MouseWheelEvent wheel_event()
{
	return last_wheel;
}
void clear_wheel_event()
{
	last_wheel.x = 0;
	last_wheel.y = 0;
}


static unsigned int last_time = 0;
static unsigned int last_delta = 1;
float global_time()
{
	unsigned int now = SDL_GetTicks();
	if (now <= last_time)
		now = last_time + 1;
	last_delta = now - last_time;
	last_time = now;
	return (float)last_time;
}
float delta_time()
{
	return (float)last_delta;
}

static int stop = 0;
Window create_window(const int w, const int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("[error] SDL_Init() failed:\n%s\n", SDL_GetError());
		return NULL;
	}

	atexit(SDL_Quit);

	// creer la fenetre
	Window window = SDL_CreateWindow("Outerrain",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		printf("[error] SDL_CreateWindow() failed.\n");
		return NULL;
	}

	int keys;
	const unsigned char *state = SDL_GetKeyboardState(&keys);
	key_states.assign(state, state + keys);

	SDL_SetWindowDisplayMode(window, NULL);
	SDL_StartTextInput();
	SDL_GetWindowSize(window, &width, &height);

	return window;
}
void release_window(Window window)
{
	SDL_StopTextInput();
	SDL_DestroyWindow(window);
}
int events(Window window)
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
				SDL_SetWindowSize(window, width, height);
				glViewport(0, 0, width, height);
			}
			break;

		case SDL_DROPFILE:
			last_drop.assign(event.drop.file);
			SDL_free(event.drop.file);
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

Context create_context(Window window, const int major, const int minor)
{
	if (window == NULL)
		return NULL;

	// configure la creation du contexte opengl core profile, debug profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
#ifndef GK_RELEASE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 15);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Context context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		printf("[error] creating openGL context.\n");
		return NULL;
	}

	SDL_GL_SetSwapInterval(1);

#ifndef NO_GLEW
	// initialise les extensions opengl
	glewExperimental = 1;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("[error] loading extensions\n%s\n", glewGetErrorString(err));
		SDL_GL_DeleteContext(context);
		return NULL;
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

	return context;
}
void release_context(Context context)
{
	SDL_GL_DeleteContext(context);
}
