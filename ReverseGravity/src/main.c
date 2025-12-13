#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "defs.h"
#include "init.h"
#include "draw.h"
#include "action.h"
#include "input.h"
#include "scene_title.h"
#include "scene_story.h"
#include "map.h"
#include "player_death.h"


// effect는 다른 모듈에서 실제 사용
extern Mix_Chunk *death_effect;

// ==================================================
// 게임 상태
// ==================================================
GameState game_state = STATE_TITLE;

int main(void)
{
    InitMemorySet();
    InitSDL();


    // 플레이어 초기화 전에 방을 먼저 로드해야 StartPoint 위치를 정확히 찾을 수 있음
    LoadRoom(0, 0);
    InitPlayer();

    // 타이틀 씬 초기화
    title_init();

    // ==================================================
    // 메인 루프
    // ==================================================
    for (;;)
    {
        ClearWindow();

        // 엔딩 상태에서는 입력 제한
        if (game_state == STATE_ENDING)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    game_state = STATE_EXIT;
                }
                if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    game_state = STATE_EXIT;
                }
            }
        }
        else
        {
            GetInput();
        }

        switch (game_state)
        {
            case STATE_TITLE:
                title_update();
                title_render();
                break;

            case STATE_STORY:
                story_update();
                story_render();
                break;

            case STATE_GAME:
                ActGame();
                DrawGame();
                break;

            case STATE_GAMEOVER:
                ActGameOver();
                DrawGameOver();
                break;

            case STATE_ENDING:
                DrawEnding();
                break;

            case STATE_EXIT:
                QuitSDL();
                exit(0);
                break;

            default:
                break;
        }

        ShowWindow();
        SDL_Delay(16); // 약 60 FPS
    }

    return 0;
}
