#include "scene_title.h"
#include "scene_story.h"
#include "defs.h"
#include "init.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// ==================================================
// 외부 전역 변수 호출
// ==================================================
extern App app;
extern TTF_Font* font_normal;
extern TTF_Font* font_selected;
extern Mix_Music* interface_bgm;

// ==================================================
// 타이틀 리소스
// ==================================================
SDL_Texture* title_image   = NULL;
SDL_Texture* ending_texture = NULL;

// 메뉴 상태
// 0 = START
// 1 = EXIT
int menu_index = 0;

// ==================================================
// 타이틀 초기화
// ==================================================
void title_init(void)
{
    title_image = IMG_LoadTexture(app.renderer, "gfx/titlescreen.png");
    if (!title_image)
    {
        printf("Title image load failed: %s\n", IMG_GetError());
        exit(1);
    }

    menu_index = 0;

    // 타이틀 BGM 반복 재생
    Mix_PlayMusic(interface_bgm, -1);
}

// ==================================================
// 타이틀 업데이트 (입력 처리)
// ==================================================
void title_update(void)
{
    // 메뉴 이동
    if (app.menu_up)
    {
        if (menu_index > 0)
            menu_index--;

        app.menu_up = 0;
    }

    if (app.menu_down)
    {
        if (menu_index < 1)
            menu_index++;

        app.menu_down = 0;
    }


    // 메뉴 선택
    if (app.menu_enter || app.key_space)
    {
        if (menu_index == 0)
        {
            // START
            Mix_HaltMusic();
            ResetInputFlags();

            story_init();
            game_state = STATE_STORY;
        }
        else
        {
            // EXIT
            game_state = STATE_EXIT;
        }

        app.menu_enter = 0;
        app.key_space  = 0;
    }
}

// ==================================================
// 타이틀 렌더링
// ==================================================
void title_render(void)
{
    // 배경 출력
    SDL_RenderCopy(app.renderer, title_image, NULL, NULL);

    // 메뉴 색상 설정
    SDL_Color normal   = {255, 255, 255, 255};
    SDL_Color selected = {255, 255, 0,   255};

    // START 버튼
    TTF_Font* font_s = (menu_index == 0) ? font_selected : font_normal;
    SDL_Color col_s  = (menu_index == 0) ? selected      : normal;

    SDL_Surface* s_start =
        TTF_RenderText_Solid(font_s, "START", col_s);
    SDL_Texture* tex_start =
        SDL_CreateTextureFromSurface(app.renderer, s_start);

    // EXIT 버튼
    TTF_Font* font_e = (menu_index == 1) ? font_selected : font_normal;
    SDL_Color col_e  = (menu_index == 1) ? selected      : normal;

    SDL_Surface* s_exit =
        TTF_RenderText_Solid(font_e, "EXIT", col_e);
    SDL_Texture* tex_exit =
        SDL_CreateTextureFromSurface(app.renderer, s_exit);

    // 위치 & 크기 (2배 확대)
    SDL_Rect r_start;
    r_start.w = s_start->w * 2;
    r_start.h = s_start->h * 2;
    r_start.x = (SCREEN_WIDTH - r_start.w) / 2;
    r_start.y = 700;

    SDL_Rect r_exit;
    r_exit.w = s_exit->w * 2;
    r_exit.h = s_exit->h * 2;
    r_exit.x = (SCREEN_WIDTH - r_exit.w) / 2;
    r_exit.y = 900;

    // 출력
    SDL_RenderCopy(app.renderer, tex_start, NULL, &r_start);
    SDL_RenderCopy(app.renderer, tex_exit,  NULL, &r_exit);

    // 메모리 해제
    SDL_FreeSurface(s_start);
    SDL_FreeSurface(s_exit);
    SDL_DestroyTexture(tex_start);
    SDL_DestroyTexture(tex_exit);
}

// ==================================================
// 엔딩 화면 초기화
// ==================================================
void InitEnding(void)
{
    ending_texture = IMG_LoadTexture(app.renderer, "gfx/ending.png");
    if (!ending_texture)
    {
        printf("Error loading ending.png: %s\n", IMG_GetError());
    }
}

// ==================================================
// 엔딩 화면 렌더링
// ==================================================
void DrawEnding(void)
{
    SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(app.renderer, ending_texture, NULL, &dest);
}
