#include "defs.h"
#include "init.h"
#include "player_draw.h"

void draw_player(void)
{
    SDL_Rect dst = player.pos;
    SDL_RendererFlip flip = player.gravity_inverted ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(app.renderer, player.texture, NULL, &dst, 0.0, NULL, flip);
}
