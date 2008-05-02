#include "Enemy.h"

Enemy::Enemy(remar2d *gfx, char *sprite, SoundManager *sfx)
  : Object(gfx, sprite, sfx)
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
