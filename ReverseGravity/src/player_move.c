#include "defs.h"
#include "init.h"
#include "player_move.h"
#include "player_collision.h"
#include "map.h"

// MOVE_SPEED 기본값 정의 (혹시 없으면 600.0으로 설정)
#ifndef MOVE_SPEED
#define MOVE_SPEED 600.0 
#endif

void move_player_horzontal(double dt)
{
    int dir = 0;

    if (app.key_left && !app.key_right)
        dir = -1;
    else if (app.key_right && !app.key_left)
        dir = 1;

    // ★ [핵심] 수평 속도 계산
    double current_speed = MOVE_SPEED;

    // 가속 타일 (2배)
    if (is_on_speed_tile()) {
        current_speed *= 2.0;
    }
    
    // 감속 타일 (0.5배)
    if (is_on_slow_tile()) {
        current_speed *= 0.5;
    }

    player.v_x = dir * current_speed;

    int old_x = player.pos.x;
    player.pos.x += (int)(player.v_x * dt);

    if (check_wall_collision())
    {
        player.pos.x = old_x;
    }
}

extern Entity player;

void UpdatePlayer()
{
    int screen_w = MAP_WIDTH * TILE_SIZE;
    int screen_h = MAP_HEIGHT * TILE_SIZE;

    // 방 전환 처리
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