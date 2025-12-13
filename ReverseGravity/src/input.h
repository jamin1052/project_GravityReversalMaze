#pragma once

#include <SDL2/SDL.h>

// Input 모듈
//  - SDL 이벤트 폴링
//  - 키 입력 상태 갱신

void GetInput(void);
void ResponseKeyUp(SDL_KeyboardEvent *event);
void ResponseKeyDown(SDL_KeyboardEvent *event);