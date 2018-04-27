#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#undef main

class Window
{
private:
	SDL_GLContext glContext;
	SDL_Window* windowSDL;
	int frameWidth;
	int frameHeight;

	std::vector<unsigned char> key_states;
	SDL_MouseButtonEvent last_button;
	SDL_KeyboardEvent last_key;
	SDL_TextInputEvent last_text;
	SDL_MouseWheelEvent last_wheel;
	int stop;

public:
	Window(int frameWidth, int frameHeight);
	~Window();
	void CreateGLContext(int major, int minor);
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
	void ClearAllKeyStates();
	void ClearKeyState(const SDL_Keycode& key);
	void ClearWheelEvent();
	void ClearButtonEvent();
	void ClearKeyEvent();

	SDL_Window* GetSDLWindow() const;
	int Width() const;
	int GetValueBilinear() const;
};