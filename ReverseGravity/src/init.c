#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "defs.h"
#include "init.h"
#include "map.h"

// ==================================================
// 전역 리소스 (사운드 / 텍스처)
// ==================================================

Mix_Music* interface_bgm = NULL;   // NOTE: 타이틀/메뉴용 (현재 게임 루프에선 미사용)
Mix_Music* gameplayBGM = NULL;    // 인게임 BGM
Mix_Music* ending_bgm   = NULL;   // 엔딩 BGM

Mix_Chunk* spin_jump_effect = NULL;
Mix_Chunk* checkpoint_effect = NULL;
Mix_Chunk* death_effect = NULL;

// ==================================================
// 핵심 전역 객체
// ==================================================

App app;
Entity player;
Camera camera = {0, 0};

// ==================================================
// 타일 / 플레이어 텍스처
// ==================================================

SDL_Texture* g_tile_textures[16];

SDL_Texture* player_texture_normal  = NULL;
SDL_Texture* player_texture_reverse = NULL;

// ==================================================
// 레이저 / 투사체 (projectile.c 에서 extern 참조)
// ==================================================

SDL_Texture* g_laser_beam_texture = NULL;
Projectile g_projectiles[MAX_PROJECTILES];

// ==================================================
// 폰트 (타이틀/스토리 씬용)
// ==================================================

// NOTE: 게임 플레이 중에는 사용하지 않음
TTF_Font* font_normal   = NULL;
TTF_Font* font_selected = NULL;

// ==================================================
// 게임 오버 화면
// ==================================================

SDL_Texture* death_texture = NULL;

// ==================================================
// 메모리 초기화
// ==================================================

void InitMemorySet(void)
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
}

// ==================================================
// 내부 전용: 타일 텍스처 로드
// ==================================================

static void load_tile_texture(SDL_Texture** tex_ptr, const char* path)
{
    *tex_ptr = IMG_LoadTexture(app.renderer, path);
    if (*tex_ptr == NULL)
    {
        printf("Error loading texture %s: %s\n", path, IMG_GetError());
        exit(1);
    }
    SDL_SetTextureBlendMode(*tex_ptr, SDL_BLENDMODE_BLEND);
}

// ==================================================
// SDL 초기화
// ==================================================

void InitSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow(
        "Reverse Gravity",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if (!app.window)
    {
        printf("창 생성 실패: %s\n", SDL_GetError());
        exit(1);
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer)
    {
        printf("렌더러 생성 실패: %s\n", SDL_GetError());
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError());
        exit(1);
    }

    death_texture = IMG_LoadTexture(app.renderer, "./gfx/death.png");
    if (!death_texture)
    {
        printf("death.png 로드 실패: %s\n", IMG_GetError());
        exit(1);
    }

    // --------------------------------------------------
    // TTF (현재는 타이틀/스토리 씬 전용)
    // --------------------------------------------------
    if (TTF_Init() < 0)
    {
        printf("SDL_ttf 초기화 실패: %s\n", TTF_GetError());
        exit(1);
    }

    font_normal   = TTF_OpenFont("ttf/LiberationSans-Regular.ttf", 32);
    font_selected = TTF_OpenFont("ttf/LiberationSans-Regular.ttf", 40);

    // --------------------------------------------------
    // 타일 텍스처 로드
    // --------------------------------------------------
    load_tile_texture(&g_tile_textures[TILE_FLOOR], "./gfx/floor_tile.png");
    load_tile_texture(&g_tile_textures[TILE_SPIKE], "./gfx/spike.png");
    load_tile_texture(&g_tile_textures[TILE_GOAL], "./gfx/GoalPoint.png");
    load_tile_texture(&g_tile_textures[TILE_START], "./gfx/StartPoint.png");
    load_tile_texture(&g_tile_textures[TILE_CHECKPOINT], "./gfx/CheckPoint.png");
    load_tile_texture(&g_tile_textures[TILE_GRAVITY_STRING], "./gfx/String.png");
    load_tile_texture(&g_tile_textures[TILE_SPEED], "./gfx/Speed.png");
    load_tile_texture(&g_tile_textures[TILE_SLOW], "./gfx/Slow.png");

    load_tile_texture(&g_tile_textures[TILE_LASER_FLOOR], "./gfx/Laser.png");
    g_tile_textures[TILE_LASER_CEILING] = g_tile_textures[TILE_LASER_FLOOR];
    g_tile_textures[TILE_LASER_LEFT]    = g_tile_textures[TILE_LASER_FLOOR];
    g_tile_textures[TILE_LASER_RIGHT]   = g_tile_textures[TILE_LASER_FLOOR];

    load_tile_texture(&g_laser_beam_texture, "./gfx/LaserBeam.png");

    // --------------------------------------------------
    // 사운드
    // --------------------------------------------------
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer 초기화 실패: %s\n", Mix_GetError());
        exit(1);
    }

    interface_bgm = Mix_LoadMUS("sound/interfaceBGM.wav"); // UNUSED (현재 루프)
    gameplayBGM   = Mix_LoadMUS("sound/gameplayBGM.wav");
    ending_bgm    = Mix_LoadMUS("sound/ending.wav");

    spin_jump_effect  = Mix_LoadWAV("sound/spin_jump.wav");
    checkpoint_effect = Mix_LoadWAV("sound/checkpoint.wav");
    death_effect      = Mix_LoadWAV("sound/deathsound.wav");
}

// ==================================================
// SDL 종료
// ==================================================

void QuitSDL(void)
{
    Mix_FreeChunk(spin_jump_effect);
    Mix_FreeChunk(checkpoint_effect);
    Mix_FreeChunk(death_effect);

    Mix_FreeMusic(interface_bgm);
    Mix_FreeMusic(gameplayBGM);
    Mix_FreeMusic(ending_bgm);

    SDL_DestroyTexture(death_texture);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

// ==================================================
// 플레이어 초기화
// ==================================================

void PlacePlayerAtStart(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (g_map_data[y][x] == TILE_START)
            {
                player.pos.x = x * TILE_SIZE;
                player.pos.y = y * TILE_SIZE;
                return;
            }
        }
    }

    // StartPoint가 없을 경우
    player.pos.x = SCREEN_WIDTH / 4;
    player.pos.y = SCREEN_HEIGHT / 4;
}

void InitPlayer(void)
{
    load_tile_texture(&player_texture_normal, "./gfx/Player.png");
    load_tile_texture(&player_texture_reverse, "./gfx/ReversePlayer.png");

    player.texture = player_texture_normal;

    PlacePlayerAtStart();

    player.pos.w = PLAYER_WIDTH;
    player.pos.h = PLAYER_HEIGHT;
    player.health = 1;

    player.checkpoint_x = player.pos.x;
    player.checkpoint_y = player.pos.y;
    player.checkpoint_room_row = current_room_row;
    player.checkpoint_room_col = current_room_col;

    // UNUSED: 중력 쿨타임 시스템 제거됨
    // player.gravity_cooldown = 0.0;

    player.last_string_row = -1;
    player.last_string_col = -1;

    player.has_checkpoint = 0;
    player.last_checkpoint_id = -1;
}

// UNUSED:
// 타일 초기화는 InitSDL에서 이미 처리됨
// void InitTiles(void) {}
