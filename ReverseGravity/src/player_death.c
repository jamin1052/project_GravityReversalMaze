#include "defs.h"
#include "init.h"
#include "player_death.h"
#include "map.h"    // LoadRoom()
#include "input.h"  // ResetInputFlags()

// ==================================================
// 플레이어 사망 처리
// ==================================================
void kill_player(void)
{
    // 생명 상태
    player.health = 0;

    // 이동 정지
    player.v_x = 0;
    player.v_y = 0;

    // 사망 효과음
    if (death_effect)
        Mix_PlayChannel(-1, death_effect, 0);

    // 게임 오버 상태로 전환
    game_state = STATE_GAMEOVER;
}

// ==================================================
// 체크포인트 또는 시작위치로 복구
// R 키 리스폰에 사용
// ==================================================
void reset_player_to_checkpoint(void)
{
    // 체크포인트가 저장된 방으로 이동
    LoadRoom(player.checkpoint_room_row,
             player.checkpoint_room_col);

    // 위치 복원
    player.pos.x = player.checkpoint_x;
    player.pos.y = player.checkpoint_y;

    // 물리 상태 초기화
    player.v_x = 0.0;
    player.v_y = 0.0;
    player.is_grounded = 0;

    // 중력 및 스킨 초기화
    player.gravity_inverted = 0;
    player.texture = player_texture_normal;

    // 생명 복구
    player.health = 1;

    // 중력 실 재트리거 방지용 초기화
    player.last_string_row = -1;
    player.last_string_col = -1;

    // 입력 상태 초기화 (특히 key_r 연타 방지)
    ResetInputFlags();

    // 게임 상태 복귀
    game_state = STATE_GAME;
}
