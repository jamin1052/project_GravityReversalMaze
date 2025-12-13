#include "player_move.h"
#include "gravity.h"
#include "player_collision.h"
#include "player_death.h"
#include "player_draw.h"
#include "defs.h"
#include "action.h"
#include "map.h"
#include "projectile.h" // 사용 중: UpdateProjectiles()
#include <SDL2/SDL_mixer.h>

#define JUMP_SPEED 900.0

static int game_bgm_playing = 0;

void ActGame(void)
{
    const double dt = 1.0 / 60.0;

    if (!game_bgm_playing)
    {
        Mix_PlayMusic(gameplayBGM, -1);
        game_bgm_playing = 1;
    }

    // 살아있을 때도 R 누르면 즉시 체크포인트로
    if (app.key_r)
    {
        reset_player_to_checkpoint();
        return; // 이 프레임 나머지 물리/충돌 계산 스킵
    }


    // 중력 반전 (SPACE 사용)
    if (app.key_space && player.is_grounded)
    {
        Mix_PlayChannel(-1, spin_jump_effect, 0);

        player.gravity_inverted = !player.gravity_inverted;

        if (player.gravity_inverted)
            player.texture = player_texture_reverse;
        else
            player.texture = player_texture_normal;

        app.key_space = 0;
    }

    // 투사체 업데이트 (사용 중)
    UpdateProjectiles(dt);

    move_player_horzontal(dt);
    gravity_inverted(dt);
    resolve_vertical_collision();

    check_spike_collision();
    check_goal_reach();
    check_interactive_tiles(); // 사용 중 (상호작용 타일)


    gravity_inverted(dt);

    
    // 방 이동 / 플레이어 상태 업데이트
    UpdatePlayer();

    // 카메라 고정
    camera.x = 0;
    camera.y = 0;
}

void ActGameOver(void)
{
    if (game_bgm_playing)
    {
        Mix_HaltMusic();
        game_bgm_playing = 0;
    }

    if (app.key_r)
    {
        LoadRoom(player.checkpoint_room_row, player.checkpoint_room_col);

        player.pos.x = player.checkpoint_x;
        player.pos.y = player.checkpoint_y;

        player.v_x = 0.0;
        player.v_y = 0.0;
        player.is_grounded = 0;
        player.gravity_inverted = 0;
        player.texture = player_texture_normal;

        player.health = 1;

        ResetInputFlags();
        game_state = STATE_GAME;
    }
}
