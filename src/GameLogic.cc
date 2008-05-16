#include "GameLogic.h"
#include <stdlib.h>

GameLogic::GameLogic(Input *i, remar2d *gfx, SoundManager *sfx)
  : input(i), graphics(gfx), sound(sfx), oldTime(SDL_GetTicks()),
    lastFrameTime(SDL_GetTicks() / 1000.0), cyclesLeftOver(0), quitGame(false),
    level(0), menu(0), fullScreen(false)
{
  srand(time(0));

  graphics->setupTileBackground(32, 32);
  graphics->loadFont("../gfx/text.xml");

  int loading = graphics->print("text", "loading");
  graphics->showSprite(loading, true);
  graphics->moveSpriteAbs(loading, 0, 0);
  graphics->redraw();

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
		     "../gfx/ammo.xml",
		     "../gfx/reload.xml",
		     0};

  printf("Loading sprites");
  for(int i = 0;sprites[i];i++)
    {
      printf(".");
      graphics->loadSprite(sprites[i]);
    }
  printf("\n");


  graphics->loadTileSet("../gfx/background.xml");

  scoreKeeper = new ScoreKeeper();

  graphics->showSprite(loading, false);
  graphics->removeSpriteInstance(loading);

//   scoreKeeper->setSkillLevel(3);
//   scoreKeeper->setTimer(178);
//   scoreKeeper->killed(ScoreKeeper::Fuzz);
//   scoreKeeper->killed(ScoreKeeper::Fuzz);
//   scoreKeeper->killed(ScoreKeeper::Drone);

  mode = MENU;
  gameMode = new Menu(graphics, sound, input, scoreKeeper);
//   mode = SCORE;
//   gameMode = new ScoreScreen(graphics, sound, input, scoreKeeper);
}

GameLogic::~GameLogic()
{
  delete scoreKeeper;
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
      Mode oldMode = mode;

      if(gameMode)
	{
	  mode = gameMode->update();
	}
      
      if(oldMode != mode)
	{
	  delete gameMode;
	  switch(mode)
	    {
	    case MENU:	
	      gameMode = new Menu(graphics, sound, input, scoreKeeper);
	      break;

	    case GAME:
	      gameMode = new Level(graphics, sound, input, scoreKeeper);
	      break;

	    case SCORE:
	      gameMode = new ScoreScreen(graphics, sound, input, scoreKeeper);
	      break;

	    case QUIT:
	      quitGame = true;
	      break;
	    }
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

  return quitGame;
  //return (input->quit() || input->pressed(SDLK_ESCAPE) || quitGame);
}
