// src/input.h (이 내용으로 덮어씌워주세요!)
#pragma once

#include <SDL2/SDL.h>

void GetInput(void);
void ResponseKeyUp(SDL_KeyboardEvent *event);
void ResponseKeyDown(SDL_KeyboardEvent *event);
void handle_input(void);
