#pragma once

#include <SDL2\SDL.h>
#undef main SDLMain

typedef SDL_Window *Window;
typedef SDL_GLContext Context;

Window CreateWindow(const int width, const int height);
void ReleaseWindow(Window w);

Context create_context(Window window, const int major = 3, const int minor = 2);
void release_context(Context context);

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

int Events(Window window);
