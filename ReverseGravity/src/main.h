#ifndef MAIN_H
#define MAIN_H

#include "defs.h"
#include "init.h"
#include "input.h"
#include "action.h"
#include "draw.h"

// 전역 객체
extern App app;          // 프로그램 전체 상태
extern Entity player;    // 플레이어

// 사망 효과음은 여전히 다른 모듈에서 사용
extern Mix_Chunk *death_effect;

#endif
