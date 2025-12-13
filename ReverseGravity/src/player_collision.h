#ifndef PLAYER_COLLISION_H
#define PLAYER_COLLISION_H

// 타일 접근
int get_tile_at(int tx, int ty);

// 충돌 판정
int is_wall_tile(int t);
int check_wall_collision(void);

// 충돌 해소
void resolve_horizontal_collision(void);
void resolve_vertical_tile_collision(void);

// 위험 / 목표
void check_spike_collision(void);
void check_goal_reach(void);

// 상호작용 타일
void check_interactive_tiles(void);

// Speed / Slow 타일
int is_on_speed_tile(void);
int is_on_slow_tile(void);

#endif
