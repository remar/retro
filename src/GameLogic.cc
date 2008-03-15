#include "GameLogic.h"
#include <stdlib.h>

GameLogic::GameLogic(Input *i, remar2d *gfx)
  : input(i), graphics(gfx), mode(GAME), oldTime(SDL_GetTicks()),
    lastFrameTime(SDL_GetTicks() / 1000.0), cyclesLeftOver(0)
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
		     "../gfx/flame.xml",
		     "../gfx/nest.xml",
		     "../gfx/shot.xml",
		     0};

  /* Read in graphics */
  for(int i = 0;sprites[i];i++)
    {
      graphics->loadSprite(sprites[i]);
    }

  scoreKeeper = new ScoreKeeper();

  /* Pass on graphics, input, and score keeper objects to the level. */
  level = new Level(graphics, input, scoreKeeper);
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
//   int delta = SDL_GetTicks() - oldTime;
//   oldTime = SDL_GetTicks();

  float currentTime = SDL_GetTicks() / 1000.0;
  float updateIterations = (currentTime - lastFrameTime) + cyclesLeftOver;

  if(updateIterations > maxCyclesPerFrame * updateInterval)
    {
      updateIterations = maxCyclesPerFrame * updateInterval;
    }

  for(;updateIterations > updateInterval; updateIterations -= updateInterval)
    {
      switch(mode)
	{
	case MENU:
	case GAME:
	  level->update(0);
	  break;
	case QUIT:
	  ;
	}
    }

  //printf("Loops: %d\n", loops);

  cyclesLeftOver = updateIterations;
  lastFrameTime = currentTime;
}

bool
GameLogic::quit()
{
  /* Should the application quit? */

  return (input->quit() || input->pressed(SDLK_ESCAPE));
}
