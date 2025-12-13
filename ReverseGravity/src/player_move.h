#ifndef PLAYER_MOVE_H
#define PLAYER_MOVE_H

#include "defs.h"

// ==================================================
// 플레이어 이동 관련 상수
// ==================================================
#define MOVE_SPEED 450.0

// 전역 플레이어 객체
extern Entity player;

// 수평 이동
void move_player_horzontal(double dt);

// 방 전환 포함 위치 업데이트
void UpdatePlayer(void);

#endif
