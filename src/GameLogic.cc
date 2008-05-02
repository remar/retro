#include "GameLogic.h"
#include <stdlib.h>

GameLogic::GameLogic(Input *i, remar2d *gfx, SoundManager *sfx)
  : input(i), graphics(gfx), sound(sfx), oldTime(SDL_GetTicks()),
    lastFrameTime(SDL_GetTicks() / 1000.0), cyclesLeftOver(0), quitGame(false),
    level(0), menu(0)
{
  srand(time(0));

  /* Read in graphics */
  char *sprites[] = {"../gfx/good.xml",
		     "../gfx/fuzz.xml",
		     "../gfx/pixel.xml",
		     "../gfx/coin.xml",
		     "../gfx/flame.xml",
		     "../gfx/nest.xml",
		     "../gfx/shot.xml",
		     "../gfx/hud.xml",
		     "../gfx/explosion.xml",
		     "../gfx/smoke.xml",
		     "../gfx/numbers.xml",
		     "../gfx/drone.xml",
		     0};
  for(int i = 0;sprites[i];i++)
    {
      graphics->loadSprite(sprites[i]);
    }

  scoreKeeper = new ScoreKeeper();

  // scoreKeeper->setSkillLevel(5);

  mode = MENU;

  menu = new Menu(graphics, sound, input, scoreKeeper);
//   /* Pass on graphics, input, and score keeper objects to the level. */
//   level = new Level(graphics, sound, input, scoreKeeper);
}

GameLogic::~GameLogic()
{
  if(level)
    delete level;
}

void
GameLogic::update()
{
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
	  mode = menu->update();
	  if(mode != MENU)
	    {
	      delete menu;
	      switch(mode)
		{
		case GAME:
		  level = new Level(graphics, sound, input, scoreKeeper);
		  break;
		}
	    }
	  break;

	case GAME:
	  mode = level->update(0);
	  break;

	case SCORE:
	  scoreKeeper->calculateScore();
	  scoreKeeper->nextLevel();
	  delete level;
	  level = new Level(graphics, sound, input, scoreKeeper);
	  mode = GAME;
	  break;

	case QUIT:
	  quitGame = true;
	  break;
	}
    }

  cyclesLeftOver = updateIterations;
  lastFrameTime = currentTime;
}

bool
GameLogic::quit()
{
  /* Should the application quit? */
  // TODO: The application should only quit if the player chooses QUIT
  // in the menu

  return (input->quit() || input->pressed(SDLK_ESCAPE) || quitGame);
}
