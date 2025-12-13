#include "defs.h"
#include "init.h"
#include "gravity.h"
#include "player_collision.h"


// 고정 낙하 속도
#define FALL_SPEED 2000.0 


void gravity_inverted(double dt)
{
    // 중력 방향 결정
    // gravity_inverted == 0 : 아래로 낙하
    // gravity_inverted == 1 : 위로 낙하
    double g_dir = player.gravity_inverted ? -1.0 : 1.0;

    // 등속 낙하를 위한 기본 속도
    double current_speed = FALL_SPEED;

    // ----------------------------------------
    // 타일 효과에 따른 낙하 속도 보정
    // ----------------------------------------

    if (is_on_speed_tile()) {
        current_speed *= 1.5;
    }

    if (is_on_slow_tile()) {
        current_speed *= 0.5;
    }

    // ----------------------------------------
    // 공중 상태에서만 중력 적용
    // ----------------------------------------
    if (!player.is_grounded) {

        // 가속도 방식이 아니므로 += 가 아니라 =
        player.v_y = current_speed * g_dir;
    }

    // 위치 업데이트
    player.pos.y += (int)(player.v_y * dt);
}

void resolve_vertical_collision(void)
{
    
    // 실제 충돌 로직은 player_collision.c 에서 처리
    resolve_vertical_tile_collision();
}
