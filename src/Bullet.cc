#include "Bullet.h"

Bullet::Bullet(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "shot", sfx), direction(LEFT)
{
  setAnimation("normal");
  setBoundingBox(6, 6, 0, 0);
}

void
Bullet::moveLeft()
{
  direction = LEFT;
}

void
Bullet::moveRight()
{
  direction = RIGHT;
}

void
Bullet::update()
{
  if(direction == LEFT)
    moveRel(-3, 0);
  else
    moveRel(3, 0);
}

Bullet::Direction
Bullet::getDirection()
{
  return direction;
}
