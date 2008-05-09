#include "GameLogic.h"
#include <stdlib.h>

GameLogic::GameLogic(Input *i, remar2d *gfx, SoundManager *sfx)
  : input(i), graphics(gfx), sound(sfx), oldTime(SDL_GetTicks()),
    lastFrameTime(SDL_GetTicks() / 1000.0), cyclesLeftOver(0), quitGame(false),
    level(0), menu(0), fullScreen(false)
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
		     "../gfx/clock.xml",
		     0};

  printf("Loading sprites");
  for(int i = 0;sprites[i];i++)
    {
      printf(".");
      graphics->loadSprite(sprites[i]);
    }
  printf("\n");

  graphics->loadFont("../gfx/text.xml");

  graphics->loadTileSet("../gfx/background.xml");

  scoreKeeper = new ScoreKeeper();

  //  mode = SCORE;
  //  scoreScreen = new ScoreScreen(graphics, sound, input, scoreKeeper);

  mode = MENU;
  menu = new Menu(graphics, sound, input, scoreKeeper);
}

GameLogic::~GameLogic()
{
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

  if(input->pressed(SDLK_f))
    {
      fullScreen = !fullScreen;
      graphics->setFullScreen(fullScreen);
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

	  if(mode != GAME)
	    {
	      delete level;
	      switch(mode)
		{
		case SCORE:
		  scoreScreen
		    = new ScoreScreen(graphics, sound, input, scoreKeeper);
		  break;
		}
	    }

	  break;

	case SCORE:
// 	  scoreKeeper->calculateScore();
// 	  scoreKeeper->nextLevel();
// 	  delete level;
// 	  level = new Level(graphics, sound, input, scoreKeeper);
// 	  mode = GAME;

	  mode = scoreScreen->update();

	  if(mode != SCORE)
	    {
	      delete scoreScreen;
	      switch(mode)
		{
		case GAME:
		  level = new Level(graphics, sound, input, scoreKeeper);
		  break;
		}
	    }
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
