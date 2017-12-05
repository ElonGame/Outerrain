#pragma once

#include <SDL2\SDL.h>
#undef main SDLMain


SDL_Window* CreateWindow(const int width, const int height);
void ReleaseWindow(SDL_Window* w);

SDL_GLContext create_context(SDL_Window* window, const int major = 3, const int minor = 2);
void release_context(SDL_GLContext context);

int WindowWidth();
int WindowHeight();

int key_state(const SDL_Keycode key);
void ClearKeyState(const SDL_Keycode key);

SDL_KeyboardEvent key_event();
void ClearKeyEvent();

SDL_MouseButtonEvent button_event();
void ClearButtonEvent();

SDL_MouseWheelEvent wheel_event();
void ClearWheelEvent();

SDL_TextInputEvent text_event();
void ClearTextEvent();


float GlobalTime();
float DeltaTime();

int Events(SDL_Window* window);
