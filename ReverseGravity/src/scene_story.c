#include "scene_story.h"
#include "defs.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "map.h"
#include "init.h"

// ==================================================
// 스토리 씬
//  - 타이틀 이후 한 번만 출력
//  - 스토리 및 조작법 설명
// ==================================================
extern App app;
extern GameState game_state;
extern Camera camera;
extern TTF_Font* font_normal;

// 스토리 이미지
static SDL_Texture* story_img = NULL;

// 안내 텍스트
static SDL_Texture* hint_tex = NULL;
static SDL_Rect hint_rect;

// 씬 진입 시각 (입력 씹힘 방지용)
static Uint32 story_enter_ticks = 0;

// ==================================================
// 리소스 해제
// ==================================================
static void story_destroy(void)
{
    if (story_img)
    {
        SDL_DestroyTexture(story_img);
        story_img = NULL;
    }

    if (hint_tex)
    {
        SDL_DestroyTexture(hint_tex);
        hint_tex = NULL;
    }
}

// ==================================================
// 스토리 씬 초기화
// ==================================================
void story_init(void)
{
    story_destroy();

    // 배경 이미지 로드
    story_img = IMG_LoadTexture(app.renderer, "gfx/scene_story.jpg");

    // 이미지가 없으면 스토리 스킵
    if (!story_img)
    {
        printf("Story image load failed: %s\n", IMG_GetError());
        game_state = STATE_GAME;
        return;
    }

    // ------------------------------
    // 말풍선 영역 설정 (비율 기반)
    // ------------------------------
    SDL_Rect bubble =
    {
        (int)(SCREEN_WIDTH  * 0.35f),
        (int)(SCREEN_HEIGHT * 0.30f),
        (int)(SCREEN_WIDTH  * 0.68f),
        (int)(SCREEN_HEIGHT * 0.30f)
    };

    int padding = 40;

    // ------------------------------
    // 안내 텍스트 생성
    // ------------------------------
    SDL_Color white = {255, 255, 255, 255};

    const char* hint =
        "\n"
        "   Hello, player~. I'm virus!\n"
        "   We're going to hack this computer. Let's go~\n"
        "   If you want to start, press spacebar!\n"
        "\n"
        "   -> : Right\n"
        "   <- : Left\n"
        "   Spacebar : Gravity Reverse\n";

    // 말풍선 폭에 맞춰 자동 줄바꿈
    int wrap = bubble.w - padding * 2;

    SDL_Surface* s =
        TTF_RenderUTF8_Blended_Wrapped(font_normal, hint, white, wrap);

    if (!s)
    {
        printf("TTF render failed: %s\n", TTF_GetError());
    }
    else
    {
        hint_tex = SDL_CreateTextureFromSurface(app.renderer, s);

        if (!hint_tex)
        {
            printf("CreateTextureFromSurface failed: %s\n", SDL_GetError());
        }
        else
        {
            // 말풍선 중앙 정렬
            hint_rect.w = s->w;
            hint_rect.h = s->h;
            hint_rect.x = bubble.x + (bubble.w - hint_rect.w) / 2;
            hint_rect.y = bubble.y + (bubble.h - hint_rect.h) / 2;
        }

        SDL_FreeSurface(s);
    }

    // 진입 시각 기록
    story_enter_ticks = SDL_GetTicks();

    // 이전 씬 입력 잔여 제거
    ResetInputFlags();
}

// ==================================================
// 스토리 씬 업데이트
// ==================================================
void story_update(void)
{
    // 씬 진입 직후 입력 무시 (즉시 스킵 방지)
    if (SDL_GetTicks() - story_enter_ticks < 200)
        return;

    if (app.key_space || app.menu_enter)
    {
        ResetInputFlags();

        // 게임 시작 준비
        LoadRoom(0, 0);
        InitPlayer();

        camera.x = 0;
        camera.y = 0;

        game_state = STATE_GAME;
    }
}

// ==================================================
// 스토리 씬 렌더링
// ==================================================
void story_render(void)
{
    // 배경 전체 화면 출력
    SDL_Rect dst = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(app.renderer, story_img, NULL, &dst);

    // 안내 텍스트
    if (hint_tex)
        SDL_RenderCopy(app.renderer, hint_tex, NULL, &hint_rect);
}
