#include "Enemy.h"

Enemy::Enemy(remar2d *gfx, char *sprite, SoundManager *sfx,
	     ScoreKeeper *scoreKeeper)
  : Object(gfx, sprite, sfx), scoreKeeper(scoreKeeper)
{

}

Enemy::~Enemy()
{

}

void
Enemy::update(Field *field, Hero *hero)
{
  
}

bool
Enemy::hit()
{
  return false;
}
