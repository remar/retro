#include "GameLogic.h"
#include <stdlib.h>

GameLogic::GameLogic(Input *i, remar2d *gfx)
  : input(i), graphics(gfx), mode(GAME), oldTime(SDL_GetTicks())
{
  srand(time(0));

  /* Size of tiles depends on what screen we're on, so this shouldn't
     be here. */
  graphics->setupTileBackground(32, 32);

  /* TODO: Loading of sprites should be lazy instead. */
  char *sprites[] = {"../gfx/good.xml",
		     "../gfx/fuzz.xml",
		     "../gfx/pixel.xml",
		     "../gfx/coin.xml",
		     0};

  /* Read in graphics */
  for(int i = 0;sprites[i];i++)
    {
      graphics->loadSprite(sprites[i]);
    }

  /* Pass on graphics and input objects to the level. */
  level = new Level(graphics, input, "../levels/1.lev");
//   level = new Level(graphics, input, 0);
//   level->randomBlocks();
}

GameLogic::~GameLogic()
{
  delete level;
}

void
GameLogic::update()
{
  int delta = SDL_GetTicks() - oldTime;
  oldTime = SDL_GetTicks();

   switch(mode)
     {
     case MENU:
     case GAME:
       level->update(delta);
       break;
     case QUIT:
       ;
     }
}

bool
GameLogic::quit()
{
  /* Should the application quit? */

  return (input->quit() || input->pressed(SDLK_ESCAPE));
}
