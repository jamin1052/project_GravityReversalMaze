// src/utils.c (내용 전체 교체)
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h> // M_PI, cos, sin 등 사용
#include "defs.h"

// ----------------------------------------
// 플랫폼 게임에 필요한 유틸리티 함수만 유지
// ----------------------------------------

/**
 * @brief 정수형 랜덤 값 생성
 * @param min 최소값
 * @param max 최대값
 * @return min ~ max 사이의 랜덤 정수
 */
int RandInt(int min, int max) {
    if (min > max) {
        return min; // 최소값이 최대값보다 크면 최소값 반환
    }
    return rand() % (max - min + 1) + min;
}

/**
 * @brief 실수형 랜덤 값 생성
 * @param min 최소값
 * @param max 최대값
 * @return min ~ max 사이의 랜덤 실수
 */
double RandDouble(double min, double max) {
    if (min > max) {
        return min; // 최소값이 최대값보다 크면 최소값 반환
    }
    return (double)rand() / (double)RAND_MAX * (max - min) + min;
}

// ----------------------------------------
// Dodge 게임에서 사용했지만, 현재는 필요 없는 함수들은 링커 오류 방지용 더미로 남깁니다.
// ----------------------------------------

/**
 * @brief 두 객체가 충돌했는지 판단 (SDL_HasIntersection을 사용)
 * @return 충돌 시 1, 아니면 0
 */
int CheckCollisionObjects(Entity *object_a, Entity *object_b) {
    // 플레이어/타일 충돌은 이 함수가 아닌 별도 로직으로 구현할 것이므로 0 반환
    return 0; 
}

/**
 * @brief 객체가 벽 밖으로 나갔는지 판단
 * @return 벽 밖에 있다면 1, 아니면 0
 */
int CheckCollisionWall(Entity *object) {
    // 플랫폼 게임 로직에서 벽 충돌은 맵 타일과의 충돌로 처리하므로 0 반환
    return 0;
}

// 총알 관련 함수 삭제 (RandSpawnBullet, CheckCollisionSide 등)

// ----------------------------------------
// 링커 오류 방지를 위한 임시 더미 함수 정의 (utils.h에 선언되어 있으나 로직 제거된 함수)
// ----------------------------------------

// 이 함수가 utils.h에 선언되어 있다면, 링커 오류 방지를 위해 정의만 남겨둡니다.
// (Dodge 게임의 잔재일 가능성이 높습니다)
int CheckCollisionSide (Entity *object) { return 0; }
void RandSpawnBullet (Entity *object) {}
