#pragma once

// ==================================================
// Projectile / Laser 시스템
// ==================================================

// 투사체 업데이트
void UpdateProjectiles(double dt);

// 투사체 렌더링
void RenderProjectiles(void);

// 방 이동 시 투사체 초기화
void ClearProjectiles(void);
