#include "ScoreScreen.h"

ScoreScreen::ScoreScreen(remar2d *gfx, SoundManager *sfx, Input *input,
			 ScoreKeeper *scoreKeeper)
  : frameCounter(0)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;

  gfx->setupTileBackground(32, 32);

  for(int y = 0;y < 19;y++)
    for(int x = 0;x < 25;x++)
      {
	gfx->setTile(x, y, "background", 0, 0);
      }

  drawBackground();

  top = gfx->print("text", "top");
  gfx->showSprite(top, true);
  gfx->moveSpriteAbs(top, 9*32, 4*32 + 8);

  score = gfx->print("text", "score");
  gfx->showSprite(score, true);
  gfx->moveSpriteAbs(score, 8*32 + 16, 6*32 + 8);

  stage = gfx->print("text", "stage");
  gfx->showSprite(stage, true);
  gfx->moveSpriteAbs(stage, 10*32+16, 8*32+8);

  level = new Counter(gfx, 2);
  level->setPosition(13*32+16, 8*32+8);
  level->setCounter(scoreKeeper->getLevel());

  topScoreCounter = new Counter(gfx, 8);
  topScoreCounter->setPosition(12*32+16, 4*32 + 8);
  topScoreCounter->setCounter(scoreKeeper->getTopScore());

  scoreCounter = new Counter(gfx, 8);
  scoreCounter->setPosition(12*32+16, 6*32 + 8);
  scoreCounter->setCounter(scoreKeeper->getScore());

  clock = gfx->createSpriteInstance("clock");
  gfx->showSprite(clock, true);
  gfx->setAnimation(clock, "normal");
  gfx->moveSpriteAbs(clock, 10*32+16, 9*32+8);
  
  scorePerTime = gfx->print("text", "20#");
  gfx->showSprite(scorePerTime, true);
  gfx->moveSpriteAbs(scorePerTime, 11*32+16, 9*32+8);

  time = new Counter(gfx, 3);
  time->setPosition(13*32, 9*32+8);
  time->setCounter(scoreKeeper->getTimer());
}

void
ScoreScreen::drawBackground()
{
  int offsetX = 7, offsetY = 3;

  int smallWindows[] =
    {11, 4, 4, 4, 4, 4, 4, 4, 4, 4, 12, 
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
     27, 26, 26, 26, 26, 26, 26, 26, 26, 26, 28,
     1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
     9, 2, 29, 26, 26, 26, 26, 26, 30, 2, 10, };

  for(int y = 0;y < 5;y++)
    for(int x = 0;x < 11;x++)
      {
	if(smallWindows[y*11 + x])
	  {
	    gfx->setTile(x + offsetX, y + offsetY, "background",
			 smallWindows[y*11 + x], 0);
	  }
	else
	  {
	    gfx->setTile(x + offsetX, y + offsetY, 0, 0, 0);
	  }
      }

  offsetX = 9; offsetY = 8;

  for(int y = 0;y < 8;y++)
    for(int x = 0;x < 7;x++)
      {
	int i;
	if(y != 7)
	  {
	    if(x == 0)      i = 1;
	    else if(x == 6) i = 3;
	    else            i = -1;
	  }
	else
	  {
	    if(x == 0)      i = 9;
	    else if(x == 6) i = 10;
	    else            i = 2;
	  }

	if(i == -1)
	  gfx->setTile(x + offsetX, y + offsetY, 0, 0, 0);
	else
	  gfx->setTile(x + offsetX, y + offsetY, "background", i, 0);
      }
}

ScoreScreen::~ScoreScreen()
{

}

GameMode
ScoreScreen::update()
{
  frameCounter++;

  return SCORE;
}
