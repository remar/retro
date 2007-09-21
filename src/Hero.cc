#include "Hero.h"

Hero::Hero(remar2d *gfx)
  : Object(gfx, "good"), currentAnimation(STANDING)
{
  setAnimation("stand left");
  setBoundingBox(10, 20, 3, 4);
}

int getDir(float dir)
{
  if(dir < 0.0)
    return -1;
  else if(dir == 0.0)
    return 0;
  else
    return 1;
}

void
Hero::moveRel(float xDir, float yDir, bool onGround)
{
  if(!onGround)
    {
      /* Play jetpack animation */
      if(getDir(xDir) == -1)
	setAnimation("stand left");
      else if(getDir(xDir) == 1)
	setAnimation("stand right");

      currentAnimation = FLYING;
    }
  else if(xDir && onGround && currentAnimation != WALKING)
    {
      /* Move on the ground */
      direction = getDir(xDir);
      if(xDir > 0)
	setAnimation("run right");
      else
	setAnimation("run left");

      currentAnimation = WALKING;
    }
  else if(!xDir && onGround && currentAnimation != STANDING)
    {
      if(direction == -1)
	gfx->setAnimation(sprite_instance, "stand left");
      else if(direction == 1)
	gfx->setAnimation(sprite_instance, "stand right");

      direction = 0;

      currentAnimation = STANDING;
    }

  Object::moveRel(xDir, yDir);
}

void
Hero::stop()
{
  if(direction == 0)
    return;

  if(direction == -1)
    gfx->setAnimation(sprite_instance, "stand left");
  else if(direction == 1)
    gfx->setAnimation(sprite_instance, "stand right");

  direction = 0;
}

void
Hero::jump(bool on)
{
  if(on)
    jumpCounter = 80;
  jumping = on;
}

bool
Hero::jumps(int decrease)
{
  jumpCounter -= decrease;
  if(jumpCounter < 0)
    {
      jumping = false;
    }

  return jumping;
}
