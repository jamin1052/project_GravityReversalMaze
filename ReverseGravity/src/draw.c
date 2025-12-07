#include "defs.h"
#include <SDL2/SDL.h>

// DrawGame에서 사용되는 함수의 프로토타입을 미리 선언합니다.
void RenderEntity(Entity *object); 

extern TTF_Font* font_normal;

// ----------------------------------------
// 맵 렌더링 함수 (수정됨)
// ----------------------------------------
void DrawMap(void) {
    // 기본적으로 64x64 크기로 설정
    SDL_Rect destRect = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_Texture* texture_to_draw = NULL;

    // 맵 배열의 세로 (y)와 가로 (x)를 순회
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tile_type = g_map_data[y][x];

            // 렌더링 옵션 변수
            double angle = 0.0;
            SDL_RendererFlip flip = SDL_FLIP_NONE;

            // 1. 텍스처 선택 및 회전 설정
            if (tile_type == TILE_SPIKE_REVERSE) { // 3: 천장 가시 (180도)
                texture_to_draw = g_tile_textures[TILE_SPIKE]; 
                angle = 180.0;
            } 
            else if (tile_type == TILE_SPIKE_LEFT) { // 4: 왼쪽 벽 가시 (90도)
                texture_to_draw = g_tile_textures[TILE_SPIKE];
                angle = 90.0;
            }
            else if (tile_type == TILE_SPIKE_RIGHT) { // 5: 오른쪽 벽 가시 (270도)
                texture_to_draw = g_tile_textures[TILE_SPIKE];
                angle = 270.0;
            }
            else {
                // 바닥, 체크포인트, 실 등은 원래 이미지 사용
                texture_to_draw = g_tile_textures[tile_type]; 
                angle = 0.0;
            }
            
            // 빈 공간(0)이 아니며 텍스처가 존재할 경우에만 그림
            if (tile_type != TILE_EMPTY && texture_to_draw != NULL) {
                
                // ★ [수정] 체크포인트(6)와 실(7)은 원본 크기 사용
                if (tile_type == TILE_CHECKPOINT || tile_type == TILE_GRAVITY_STRING) {
                    int w, h;
                    SDL_QueryTexture(texture_to_draw, NULL, NULL, &w, &h);
                    destRect.w = w;
                    destRect.h = h;
                } else {
                    destRect.w = TILE_SIZE;
                    destRect.h = TILE_SIZE;
                }

                // 2. 그려질 화면상의 위치 계산 (카메라 보정)
                destRect.x = x * TILE_SIZE - camera.x;
                destRect.y = y * TILE_SIZE - camera.y;

                // ★ [수정] 크기가 작아진 경우 위치 보정 (정렬)
                if (tile_type == TILE_CHECKPOINT) {
                    // 깃발: 가로 중앙 정렬 + 바닥(아래) 정렬
                    destRect.x += (TILE_SIZE - destRect.w) / 2;
                    destRect.y += (TILE_SIZE - destRect.h); 
                }
                else if (tile_type == TILE_GRAVITY_STRING) {
                    // 실: 가로 중앙 정렬 + 천장(위) 정렬 (y는 그대로)
                    destRect.x += (TILE_SIZE - destRect.w) / 2;
                }

                // 화면 밖 컬링 (Culling)
                if (destRect.x + destRect.w < 0) continue;
                if (destRect.x > SCREEN_WIDTH) continue;
                if (destRect.y + destRect.h < 0) continue;
                if (destRect.y > SCREEN_HEIGHT) continue;

                // 3. 텍스처 그리기
                SDL_RenderCopyEx(app.renderer, texture_to_draw, NULL, &destRect, angle, NULL, flip);
            }
        }
    }
}

// ----------------------------------------
// DrawGame 함수
// ----------------------------------------
void DrawGame(void) {
    DrawMap();
    RenderEntity(&player); 
}

// ----------------------------------------
// DrawGameOver 함수
// ----------------------------------------
void DrawGameOver(void) 
{
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);

    const char* msg = "Press R to Restart";
    SDL_Color color = {255, 255, 255, 255};  

    SDL_Surface* surface = TTF_RenderText_Solid(font_normal, msg, color);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);

    SDL_Rect dest;
    dest.w = surface->w;
    dest.h = surface->h;
    dest.x = (SCREEN_WIDTH - dest.w) / 2;   
    dest.y = (SCREEN_HEIGHT - dest.h) / 2;  

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// ----------------------------------------
// RenderEntity 함수
// ----------------------------------------
void RenderEntity(Entity *object)
{
    SDL_Rect r = object->pos;
    r.x -= camera.x;
    r.y -= camera.y;

    SDL_RenderCopy(app.renderer, object->texture, NULL, &r);
}

void ClearWindow(void) {
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);
}

void ShowWindow(void) {
    SDL_RenderPresent(app.renderer);
}