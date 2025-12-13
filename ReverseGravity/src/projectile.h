#pragma once

void InitProjectiles(void);
void UpdateProjectiles(double dt);
void RenderProjectiles(void);

// ★ [추가] 맵 이동 시 투사체 초기화 및 타이머 리셋 함수
void ClearProjectiles(void);
