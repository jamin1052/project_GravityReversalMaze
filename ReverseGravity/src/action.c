#include "player_move.h"
#include "gravity.h"
#include "player_collision.h"
#include "player_death.h"
#include "player_draw.h"
#include "defs.h"
#include "action.h"
#include "map.h"


#define JUMP_SPEED 900.0

void ActGame(void)
{
    const double dt = 1.0 / 60.0;

    // ---------------------------------------
    // 중력 반전 (SPACE 사용)
    // ---------------------------------------
    if (app.key_space && player.is_grounded)
    {
        player.gravity_inverted = !player.gravity_inverted;

        if (player.gravity_inverted)
            player.texture = player_texture_reverse;
        else
            player.texture = player_texture_normal;

        app.key_space = 0;   // SPACE 입력 초기화
    }
    // ★ [추가] 중력 반전 쿨타임 감소
    if (player.gravity_cooldown > 0.0) {
        player.gravity_cooldown -= dt;
    }
    gravity_inverted(dt);
    resolve_vertical_collision();

    check_spike_collision();
    check_goal_reach();
    check_interactive_tiles(); // ★ [추가] 상호작용 체크 호출
    // ... (카메라 처리 유지) ...

    
    // ---------------------------------------
    // 가로 이동
    // ---------------------------------------
    move_player_horzontal(dt);

    // ---------------------------------------
    // 점프 (DOWN 사용)
    // ---------------------------------------
    if (app.key_down && player.is_grounded)
    {
        double jump_dir = player.gravity_inverted ? 1.0 : -1.0;
        player.v_y = JUMP_SPEED * jump_dir;
        player.is_grounded = 0;

        app.key_down = 0;  // 점프 입력 초기화
    }

    // ---------------------------------------
    // 중력 및 충돌 처리
    // ---------------------------------------
    gravity_inverted(dt);
    resolve_vertical_collision();

    check_spike_collision();
    check_goal_reach();
    // ---------------------------------------
    // 카메라 방 이동 처리
    // ---------------------------------------
    
    // ★ 방 바깥 나갔는지 체크
    UpdatePlayer();

    // ★ 카메라 끄기(고정)
    camera.x = 0;
    camera.y = 0;
}

void ActGameOver(void) {
    if(app.key_r){
        // ★ [수정] InitPlayer()를 부르면 텍스처를 또 로드하니까, 
        // 변수만 초기화하는 방식으로 변경하는 게 좋아.
        LoadRoom(player.checkpoint_room_row, player.checkpoint_room_col);
        // 1. 위치를 저장된 체크포인트로!
        player.pos.x = player.checkpoint_x;
        player.pos.y = player.checkpoint_y;
        
        // 2. 물리 상태 초기화
        player.v_x = 0.0;
        player.v_y = 0.0;
        player.is_grounded = 0;
        player.gravity_inverted = 0; // 부활하면 중력은 다시 정상으로
        player.texture = player_texture_normal; // 이미지도 정상으로
        player.gravity_cooldown = 0.0;
        player.health = 1;

        ResetInputFlags();
        game_state = STATE_GAME;
    }
}
