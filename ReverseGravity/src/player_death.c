#include "defs.h"
#include "init.h"
#include "player_death.h"

void kill_player(void)
{
    player.health = 0;
    player.v_x = 0;
    player.v_y = 0;

    if (death_effect)
        Mix_PlayChannel(-1, death_effect, 0);
    game_state = STATE_GAMEOVER;
}
