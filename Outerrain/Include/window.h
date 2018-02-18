#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "GL/glew.h"

#undef main

class Window
{
private:
	SDL_GLContext glContext;
	SDL_Window* windowSDL;
	int width;
	int height;

	std::vector<unsigned char> key_states;
	SDL_MouseButtonEvent last_button;
	SDL_KeyboardEvent last_key;
	SDL_TextInputEvent last_text;
	SDL_MouseWheelEvent last_wheel;
	int stop;

public:
	Window(int width, int height);
	~Window();
	void CreateGLContext(int, int);
	void SetDefaultGLState();
	void ReleaseGLContext();
	int UpdateEvents();
	void SwapWindow();

	int KeyState(const SDL_Keycode& key);
	SDL_KeyboardEvent KeyEvent();
	SDL_MouseButtonEvent ButtonEvent();
	SDL_MouseWheelEvent WheelEvent();
	SDL_TextInputEvent TextEvent();

	void ClearTextEvent();
	void ClearKeyState(const SDL_Keycode& key);
	void ClearWheelEvent();
	void ClearButtonEvent();
	void ClearKeyEvent();

	SDL_Window* GetSDLWindow() const { return windowSDL; }
	int Width() const { return width; }
	int Height() const { return height; }
};