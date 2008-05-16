#include "GameMode.h"

GameMode::GameMode(remar2d *gfx, SoundManager *sfx, Input *input,
		   ScoreKeeper *scoreKeeper)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;
}

GameMode::~GameMode()
{

}

Mode
GameMode::update()
{

}
