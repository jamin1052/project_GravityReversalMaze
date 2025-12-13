#include "defs.h"
#include "init.h"
#include "map.h"
#include "projectile.h"
#include "player_death.h"

// ==================================================
// 레이저 설정 값
// ==================================================
#define LASER_SPEED     600.0   // 레이저 이동 속도
#define LASER_INTERVAL  1.0     // 발사 간격 (초)

#define BEAM_THICKNESS  10      // 레이저 두께
#define BEAM_LENGTH     30      // 레이저 길이

// 발사 타이머
static double shoot_timer = 0.0;

// ==================================================
// 투사체 생성
// ==================================================
void SpawnProjectile(int x, int y,
                     double vx, double vy,
                     double angle,
                     int w, int h)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!g_projectiles[i].active)
        {
            g_projectiles[i].active = 1;

            // 타일 중앙 기준으로 생성
            g_projectiles[i].x = x + (TILE_SIZE / 2) - (w / 2);
            g_projectiles[i].y = y + (TILE_SIZE / 2) - (h / 2);

            g_projectiles[i].vx = vx;
            g_projectiles[i].vy = vy;
            g_projectiles[i].angle = angle;
            g_projectiles[i].w = w;
            g_projectiles[i].h = h;
            return;
        }
    }
}

// ==================================================
// 투사체 전체 초기화
// NOTE:
// 방 이동 시 이전 방의 레이저 제거용
// ==================================================
void ClearProjectiles(void)
{
    // 모든 투사체 비활성화
    for (int i = 0; i < MAX_PROJECTILES; i++)
        g_projectiles[i].active = 0;

    // 방 진입 즉시 발사되도록 타이머 보정
    shoot_timer = LASER_INTERVAL;
}

// ==================================================
// 투사체 업데이트
// ==================================================
void UpdateProjectiles(double dt)
{
    // ------------------------------
    // 1. 레이저 발사 타이머
    // ------------------------------
    shoot_timer += dt;

    if (shoot_timer >= LASER_INTERVAL)
    {
        shoot_timer = 0.0;

        // 맵 전체를 스캔하여 레이저 타일에서 발사
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                int tile = g_map_data[y][x];

                if (tile == TILE_LASER_FLOOR)
                {
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE,
                                    0, -LASER_SPEED,
                                    0.0,
                                    BEAM_THICKNESS, BEAM_LENGTH);
                }
                else if (tile == TILE_LASER_CEILING)
                {
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE,
                                    0, LASER_SPEED,
                                    180.0,
                                    BEAM_THICKNESS, BEAM_LENGTH);
                }
                else if (tile == TILE_LASER_LEFT)
                {
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE,
                                    LASER_SPEED, 0,
                                    90.0,
                                    BEAM_LENGTH, BEAM_THICKNESS);
                }
                else if (tile == TILE_LASER_RIGHT)
                {
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE,
                                    -LASER_SPEED, 0,
                                    270.0,
                                    BEAM_LENGTH, BEAM_THICKNESS);
                }
            }
        }
    }

    // ------------------------------
    // 2. 이동 및 충돌 처리
    // ------------------------------
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!g_projectiles[i].active)
            continue;

        Projectile *p = &g_projectiles[i];

        // 이동
        p->x += p->vx * dt;
        p->y += p->vy * dt;

        // 중심 기준 타일 좌표
        int cx = p->x + p->w / 2;
        int cy = p->y + p->h / 2;
        int tx = cx / TILE_SIZE;
        int ty = cy / TILE_SIZE;

        // 맵 밖 -> 소멸
        if (tx < 0 || tx >= MAP_WIDTH ||
            ty < 0 || ty >= MAP_HEIGHT)
        {
            p->active = 0;
            continue;
        }

        int tile = g_map_data[ty][tx];

        // 벽 / 가시에 닿으면 소멸
        if (tile == TILE_FLOOR ||
            (tile >= TILE_SPIKE && tile <= TILE_SPIKE_RIGHT))
        {
            p->active = 0;
            continue;
        }

        // 플레이어 충돌
        SDL_Rect p_rect = player.pos;
        SDL_Rect b_rect = { (int)p->x, (int)p->y, p->w, p->h };

        if (SDL_HasIntersection(&p_rect, &b_rect))
        {
            kill_player();
            p->active = 0;
        }
    }
}

// ==================================================
// 투사체 렌더링
// ==================================================
void RenderProjectiles(void)
{
    if (!g_laser_beam_texture)
        return;

    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!g_projectiles[i].active)
            continue;

        SDL_Rect dest;

        // 회전 각도에 따라 크기 보정
        if (g_projectiles[i].angle == 90.0 ||
            g_projectiles[i].angle == 270.0)
        {
            dest.w = g_projectiles[i].h;
            dest.h = g_projectiles[i].w;
        }
        else
        {
            dest.w = g_projectiles[i].w;
            dest.h = g_projectiles[i].h;
        }

        // 히트박스 중심 기준 정렬
        int center_x = g_projectiles[i].x + g_projectiles[i].w / 2;
        int center_y = g_projectiles[i].y + g_projectiles[i].h / 2;

        dest.x = center_x - dest.w / 2 - camera.x;
        dest.y = center_y - dest.h / 2 - camera.y;

        SDL_RenderCopyEx(app.renderer,
                         g_laser_beam_texture,
                         NULL,
                         &dest,
                         g_projectiles[i].angle,
                         NULL,
                         SDL_FLIP_NONE);
    }
}

