#include "defs.h"
#include "init.h"
#include "map.h"
#include "projectile.h"
#include "player_death.h"

#define LASER_SPEED 800.0      
// ★ [수정] 발사 간격을 1.0초로 변경 (더 자주 쏨)
#define LASER_INTERVAL 1.0     

#define BEAM_THICKNESS 10      
#define BEAM_LENGTH 30         

static double shoot_timer = 0.0;

// 투사체 생성 함수
void SpawnProjectile(int x, int y, double vx, double vy, double angle, int w, int h) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!g_projectiles[i].active) {
            g_projectiles[i].active = 1;
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

// --------------------------------------------------------
// 메인 업데이트 함수
// --------------------------------------------------------
void UpdateProjectiles(double dt) {
    // 1. 발사 타이머 (기존 유지)
    shoot_timer += dt;
    if (shoot_timer >= LASER_INTERVAL) {
        shoot_timer = 0.0;
        
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tile = g_map_data[y][x];
                
                if (tile == TILE_LASER_FLOOR) { 
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE, 0, -LASER_SPEED, 0.0, BEAM_THICKNESS, BEAM_LENGTH);
                }
                else if (tile == TILE_LASER_CEILING) { 
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE, 0, LASER_SPEED, 180.0, BEAM_THICKNESS, BEAM_LENGTH);
                }
                else if (tile == TILE_LASER_LEFT) { 
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE, LASER_SPEED, 0, 90.0, BEAM_LENGTH, BEAM_THICKNESS);
                }
                else if (tile == TILE_LASER_RIGHT) { 
                    SpawnProjectile(x * TILE_SIZE, y * TILE_SIZE, -LASER_SPEED, 0, 270.0, BEAM_LENGTH, BEAM_THICKNESS);
                }
            }
        }
    }

    // 2. 이동 및 충돌
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (g_projectiles[i].active) {
            Projectile *p = &g_projectiles[i];

            // (A) 이동
            p->x += p->vx * dt;
            p->y += p->vy * dt;

            // (B) 벽 충돌 체크
            int cx = p->x + p->w / 2;
            int cy = p->y + p->h / 2;
            int tx = cx / TILE_SIZE;
            int ty = cy / TILE_SIZE;

            // 맵 밖으로 나가면 소멸
            if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT) {
                p->active = 0;
                continue;
            }

            int tile = g_map_data[ty][tx];

            // ★ [수정] 바닥(1) 뿐만 아니라 가시(2,3,4,5)에도 닿으면 소멸!
            // TILE_SPIKE(2) ~ TILE_SPIKE_RIGHT(5) 범위를 체크
            if (tile == TILE_FLOOR || (tile >= TILE_SPIKE && tile <= TILE_SPIKE_RIGHT)) { 
                p->active = 0; 
                continue;
            }

            // (C) 플레이어 충돌
            SDL_Rect p_rect = player.pos; 
            SDL_Rect b_rect = { (int)p->x, (int)p->y, p->w, p->h };

            if (SDL_HasIntersection(&p_rect, &b_rect)) {
                kill_player();
                p->active = 0;
            }
        }
    }
}
// 렌더링 함수 (이미지 null 체크 포함)
void RenderProjectiles(void) {
    if (!g_laser_beam_texture) return;

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (g_projectiles[i].active) {
            SDL_Rect dest;
            
            // ★ [핵심 수정] 90도나 270도 회전한 경우(벽 레이저)
            // 가로/세로 길이를 서로 바꿔서 그려야 찌그러지지 않고 정상적으로 보입니다.
            if (g_projectiles[i].angle == 90.0 || g_projectiles[i].angle == 270.0) {
                dest.w = g_projectiles[i].h; // 높이를 너비로 (10)
                dest.h = g_projectiles[i].w; // 너비를 높이로 (30)
            } else {
                dest.w = g_projectiles[i].w;
                dest.h = g_projectiles[i].h;
            }

            // ★ [중앙 정렬] 투사체 히트박스의 '중앙'에 이미지를 그립니다.
            // (히트박스 중심점) - (이미지 절반 크기) - (카메라 보정)
            int center_x = g_projectiles[i].x + g_projectiles[i].w / 2;
            int center_y = g_projectiles[i].y + g_projectiles[i].h / 2;

            dest.x = center_x - dest.w / 2 - camera.x;
            dest.y = center_y - dest.h / 2 - camera.y;

            // 그리기 (회전 적용)
            SDL_RenderCopyEx(app.renderer, g_laser_beam_texture, NULL, &dest, g_projectiles[i].angle, NULL, SDL_FLIP_NONE);
        }
    }
}