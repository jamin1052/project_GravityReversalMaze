#include <SDL2/SDL.h>
#include <stdlib.h> // exit()
#include "defs.h"
#include "input.h"



// 내부 전용: 즉시 종료 (임시 처리)
static void QuitSDL(void)
{
    exit(0);
}

// ==================================================
// KeyUp 처리
// ==================================================
void ResponseKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat != 0)
        return;

    SDL_Scancode key_input = event->keysym.scancode;

    switch (key_input)
    {
        case SDL_SCANCODE_LEFT:
            app.key_left = 0;
            break;

        case SDL_SCANCODE_RIGHT:
            app.key_right = 0;
            break;

        case SDL_SCANCODE_R:
            app.key_r = 0;
            break;

        case SDL_SCANCODE_SPACE:
            app.key_space = 0;
            break;

        case SDL_SCANCODE_UP:

            // 게임 중 key_up은 사용하지 않음 (중력/점프 충돌 방지)
            if (game_state == STATE_TITLE)
                app.menu_up = 0;
            
            break;

        case SDL_SCANCODE_DOWN:
            if (game_state == STATE_TITLE)
                app.menu_down = 0;
            else
                app.key_down = 0;
            break;

        case SDL_SCANCODE_RETURN:
            if (game_state == STATE_TITLE)
                app.menu_enter = 0;
            else
                app.key_enter = 0;
            break;

        default:
            break;
    }
}

// ==================================================
// KeyDown 처리
// ==================================================
void ResponseKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat != 0)
        return;

    SDL_Scancode key_input = event->keysym.scancode;

    switch (key_input)
    {
        case SDL_SCANCODE_LEFT:
            app.key_left = 1;
            break;

        case SDL_SCANCODE_RIGHT:
            app.key_right = 1;
            break;

        case SDL_SCANCODE_R:
            if (game_state != STATE_TITLE)
                app.key_r = 1;
            break;

        case SDL_SCANCODE_SPACE:
            app.key_space = 1;
            break;

        case SDL_SCANCODE_ESCAPE:
            QuitSDL();
            break;

        case SDL_SCANCODE_UP:

            // 게임 중 key_up 미사용
            if (game_state == STATE_TITLE)
                app.menu_up = 1;
            
            break;

        case SDL_SCANCODE_DOWN:
            if (game_state == STATE_TITLE)
                app.menu_down = 1;
            
            break;

        case SDL_SCANCODE_RETURN:
            if (game_state == STATE_TITLE)
                app.menu_enter = 1;
            else
                app.key_enter = 1;
            break;

        default:
            break;
    }
}

// ==================================================
// SDL 이벤트 폴링
// ==================================================
void GetInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                QuitSDL();
                break;

            case SDL_KEYDOWN:
                ResponseKeyDown(&event.key);
                break;

            case SDL_KEYUP:
                ResponseKeyUp(&event.key);
                break;

            default:
                break;
        }
    }
}

// ==================================================
// 입력 플래그 초기화
// ==================================================
void ResetInputFlags(void)
{
    app.key_up = 0;
    app.key_down = 0;
    app.key_left = 0;
    app.key_right = 0;
    app.key_r = 0;
    app.key_space = 0;
    app.key_enter = 0;

    app.menu_up = 0;
    app.menu_down = 0;
    app.menu_enter = 0;
}