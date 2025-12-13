#ifndef MAP_H
#define MAP_H

#include "defs.h"

// ==================================================
// 맵 / 방 관리
// ==================================================

// 전체 맵 크기
#define ROOM_ROWS 3
#define ROOM_COLS 4

// 현재 방의 좌표
extern int current_room_row;
extern int current_room_col;

// 현재 활성화된 방의 타일 데이터
extern int g_map_data[MAP_HEIGHT][MAP_WIDTH];

// 방 로드(절대 좌표)
void LoadRoom(int row, int col);

// 방 이동(상대 좌표)
void ChangeRoom(int drow, int dcol);

#endif
