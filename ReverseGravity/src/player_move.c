#include "defs.h"
#include "init.h"
#include "player_move.h"
#include "player_collision.h"
#include "map.h"

// ==================================================
// 기본 이동 속도
// player_move.h에 정의되어 있지 않으면 기본값 사용
// ==================================================
#ifndef MOVE_SPEED
#define MOVE_SPEED 600.0
#endif

// ==================================================
// 수평 이동 처리
// ==================================================
void move_player_horzontal(double dt)
{
    int dir = 0;

    if (app.key_left && !app.key_right)
        dir = -1;
    else if (app.key_right && !app.key_left)
        dir = 1;

        
    // 이동 속도 계산
    double current_speed = MOVE_SPEED;

    // 가속 타일
    if (is_on_speed_tile())
        current_speed *= 2.0;

    // 감속 타일
    if (is_on_slow_tile())
        current_speed *= 0.5;

    player.v_x = dir * current_speed;

    // 위치 이동
    int old_x = player.pos.x;
    player.pos.x += (int)(player.v_x * dt);

    // 벽 충돌 시 이동 취소
    if (check_wall_collision())
        player.pos.x = old_x;
}

// ==================================================
// 플레이어 위치 기반 방 전환 처리
// ==================================================
void UpdatePlayer(void)
{
    int screen_w = MAP_WIDTH * TILE_SIZE;
    int screen_h = MAP_HEIGHT * TILE_SIZE;

    // 오른쪽 / 왼쪽 방 이동
    if (player.pos.x >= screen_w)
    {
        ChangeRoom(0, +1);
        player.pos.x = 0;
    }
    else if (player.pos.x + player.pos.w <= 0)
    {
        ChangeRoom(0, -1);
        player.pos.x = screen_w - player.pos.w;
    }

    // 아래 / 위 방 이동
    if (player.pos.y >= screen_h)
    {
        ChangeRoom(+1, 0);
        player.pos.y = 0;
    }
    else if (player.pos.y + player.pos.h <= 0)
    {
        ChangeRoom(-1, 0);
        player.pos.y = screen_h - player.pos.h;
    }
}
