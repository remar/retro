#include "Menu.h"

Menu::Menu(remar2d *gfx, SoundManager *sfx, Input *input,
	   ScoreKeeper *scoreKeeper)
  : nextTimer(0), level(1)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;

  gfx->setupTileBackground(16, 16);
  mainTiles = gfx->loadTileSet("../gfx/maintiles.xml");

  drawBackground();

  levelCounter = new Counter(gfx, 2);
  levelCounter->setPosition(400, 300);
  levelCounter->setCounter(level);
}

Menu::~Menu()
{
  printf("Delete menu\n");
}

GameMode
Menu::update()
{
  if(nextTimer > 0)
    {
      --nextTimer;
      if(nextTimer == 0)
	{
	  return GAME;
	}
    }

  if(input->pressed(SDLK_RETURN))
    {
      scoreKeeper->setLevel(level);

      nextTimer = 60;
      sfx->playSound(14);
    }
  else if(input->pressed(SDLK_RIGHT))
    {
      increaseLevel();
    }
  else if(input->pressed(SDLK_LEFT))
    {
      decreaseLevel();
    }
  else if(input->pressed(SDLK_ESCAPE))
    return QUIT;

  return MENU;
}

void
Menu::drawBackground()
{
  // Having a nice layout of the data is A and O... alpha and omega... uh, nice
#define A 10
#define B 11

  int battle[] =
    {
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6, 
      6, B, 1, 0, 2, B, B, B, 3, B, B, B, B, B, B, B, B, 3, B, B, B, B, B, 3, B, B, 3, B, B, 3, B, B, B, B, 6, 
      6, B, 6, 1, 8, 1, 0, 2, A, 5, B, 1, 0, 5, 1, 0, 2, 6, 4, 2, 1, 0, 2, A, 5, B, A, 5, B, 6, 1, 0, 2, B, 6, 
      6, B, 6, 7, 2, 6, 4, 8, 6, B, B, 6, B, B, 6, B, 6, 6, B, 6, 6, B, 6, 6, B, B, 6, B, B, 6, 6, 4, 8, B, 6, 
      6, B, 9, B, 9, 7, 0, 5, 7, 0, 5, 9, B, B, 7, 0, 8, 7, 0, 8, 7, 5, 9, 7, 0, 5, 7, 0, 5, 9, 7, 0, 5, B, 6, 
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6, 
      7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 
    };

  int xOffset = 8;
  int yOffset = 4;

  int i;
  for(int y = 0; y < 8;y++)
    for(int x = 0;x < 35;x++)
      {
	i = y * 35 + x;
	gfx->setTile(x + xOffset, y + yOffset, mainTiles, battle[i]%6, battle[i]/6);
      }
}

void
Menu::increaseLevel()
{
  level++;
  if(level > 32)
    level = 1;

  levelCounter->setCounter(level);
}

void
Menu::decreaseLevel()
{
  level--;
  if(level < 1)
    level = 32;

  levelCounter->setCounter(level);
}
