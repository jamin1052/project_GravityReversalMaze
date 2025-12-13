#ifndef DRAW_H
#define DRAW_H

#include "defs.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern Entity player;

void ClearWindow(void);
void ShowWindow(void);
void DrawGame(void);
void DrawGameOver(void);
void RenderEntity(Entity *object);
#endif
