#define MOVE_SPEED 450.0

#ifndef PLAYER_MOVE_H
#define PLAYER_MOVE_H

#include "defs.h"  // 여기 안에 Entity 구조체가 정의되어 있을 것임

extern Entity player;

void UpdatePlayer();

#endif


void move_player_horzontal(double dt);