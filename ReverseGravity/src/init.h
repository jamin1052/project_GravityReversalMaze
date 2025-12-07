#pragma once

#include <SDL2/SDL.h>

extern SDL_Renderer* renderer;
void QuitSDL(void);

/**
 * @brief 전역 변수를 0으로 초기화합니다.
 */
void InitMemorySet(void);

/**
 * @brief SDL 라이브러리, 창, 렌더러 및 SDL_image를 초기화합니다.
 */
void InitSDL(void);

/**
 * @brief 맵 타일 텍스처를 로드합니다.
 */
void InitTiles(void);

/**
 * @brief 플레이어 객체를 초기 위치에 설정하고 텍스처를 로드합니다.
 */
void InitPlayer(void);

// Dodge 게임에만 필요했던 InitTTF(), InitGameOver(), InitBullet() 등은 제거되었습니다.


