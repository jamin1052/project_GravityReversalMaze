#ifndef GRAVITY_H
#define GRAVITY_H


// 중력 반전 포함 낙하 처리
// 가속도 기반 중력이 아닌, 고정 속도 기반낙하를 수행한다.

void gravity_inverted(double dt);


// 수직 타일 충돌 해결
// 실제 구현은 player_collision 모듈에 위임한다.

void resolve_vertical_collision(void);

#endif
