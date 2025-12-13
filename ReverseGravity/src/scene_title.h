#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include <SDL2/SDL.h>

// ==================================================
// 시작화면 생성 및 렌더링
// ==================================================
void title_init(void);
void title_update(void);
void title_render(void);

// 엔딩화면
void InitEnding(void);
void DrawEnding(void);

#endif
