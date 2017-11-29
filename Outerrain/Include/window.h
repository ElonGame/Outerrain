#pragma once

#include <SDL2\SDL.h>
#undef main SDLMain

typedef SDL_Window *Window;
typedef SDL_GLContext Context;

Window create_window(const int width, const int height);
void release_window(Window w);

Context create_context(Window window, const int major = 3, const int minor = 2);
void release_context(Context context);

int window_width();
int window_height();

int key_state(const SDL_Keycode key);
void clear_key_state(const SDL_Keycode key);

SDL_KeyboardEvent key_event();
void clear_key_event();

SDL_MouseButtonEvent button_event();
void clear_button_event();

SDL_MouseWheelEvent wheel_event();
void clear_wheel_event();

SDL_TextInputEvent text_event();
void clear_text_event();

float global_time();
float delta_time();

int events(Window window);
