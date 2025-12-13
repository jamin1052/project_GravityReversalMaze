#ifndef ACTION_H
#define ACTION_H

#include "init.h"
#include "defs.h"

/* main.h에 정의된 전역 변수 사용 */
extern App app;
extern Entity player;

void ActGame(void);
void ActGameOver(void);

#endif
