#include "Hero.h"

Hero::Hero(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "good", sfx),  blinking(true), jumping(false), dead(false),
    direction(RIGHT), jumpCounter(80), currentAnimation(STANDING),
    channel(-1)
{
  setAnimation("blink right");
  setBoundingBox(6, 20, 5, 4);

  flame = gfx->createSpriteInstance("flame");
  gfx->showSprite(flame, false);
  gfx->setAnimation(flame, "right");
}

Hero::~Hero()
{
  /* Stop walking sound */
  if(channel != -1)
    sfx->stopSound(channel);

  gfx->showSprite(flame, false);
  gfx->removeSpriteInstance(flame);
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
Hero::setDirection(float dir)
{
  if(dir < 0.0)
    direction = LEFT;
  else if(dir > 0.0)
    direction = RIGHT;
}

void
Hero::moveRel(float xDir, float yDir)//, bool onGround)
{
  if(dead)
    return;

  bool inAir = true;

  /* When on the ground, yDir == 0 and jumping == false*/
  if(yDir == 0 && jumping == false)
    {
      jumpCounter = 80;
      inAir = false;
    }
  else if(yDir >= 0.0)
    {
      /* We've either hit the roof or let go of the jump button, don't
	 allow more jumping */
      jumpCounter = -1;
    }

  if(blinking)
    {
      if(xDir != 0.0 || yDir != 0.0)
	blinking = false;
      else
	return;
    }

  setDirection(xDir);

  if((xDir != oldXDir || yDir != oldYDir) && blinking == false)
    updateAnimation(xDir, yDir);

  oldXDir = xDir;
  oldYDir = yDir;

  Object::moveRel(xDir, yDir);

  //updateFlame();

  if(inAir)
    {
      gfx->showSprite(flame, true);
      if(direction == LEFT)
	{
	  if(flameShown == false || flameDirection == RIGHT)
	    {
	      gfx->setAnimation(flame, "left");
	      flameDirection = LEFT;
	    }
	  gfx->moveSpriteAbs(flame, getX()+13, getY()+13);
	}
      else
	{
	  if(flameShown == false || flameDirection == LEFT)
	    {
	     gfx->setAnimation(flame, "right");
	     flameDirection = RIGHT;
	    }
	  gfx->moveSpriteAbs(flame, getX()-3, getY()+13);
	}
      flameShown = true;
    }
  else
    {
      gfx->showSprite(flame, false);
      flameShown = false;
    }

}

void
Hero::stop()
{
  if(direction == LEFT)
    setAnimation("stand left");
  else
    setAnimation("stand right");
}

void
Hero::jump(bool on)
{
  if(on == false)
    jumpCounter = -1;
  jumping = on;

  if(on && jumpCounter == 80)
    sfx->playSound(4, false);
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

void
Hero::updateAnimation(float xDir, float yDir)
{
  /* getY()%32 == 8 assures us that we're on the ground */
  if(xDir == 0 && yDir == 0 && (getY()%32)==8)
    {
      if(direction == LEFT)
	setAnimation("stand left");
      else
	setAnimation("stand right");

      if(channel != -1)
	{
	  sfx->stopSound(channel);
	  channel = -1;
	}

      currentAnimation = STANDING;
    }
  else if(yDir == 0 && (getY()%32)==8)
    {
      if(direction == LEFT)
	setAnimation("run left");
      else
	setAnimation("run right");

      if(channel == -1)
	{
	  channel = sfx->playSound(11, true);
	}

      currentAnimation = WALKING;
    }
  else
    {
      if(direction == LEFT)
	setAnimation("stand left");
      else
	setAnimation("stand right");

      if(channel != -1)
	{
	  sfx->stopSound(channel);
	  channel = -1;
	}

      currentAnimation = FLYING;
    }
}

void
Hero::shoot(int *bullets, Bullet **bullet)
{
  if(dead)
    return;

  if(blinking)
    {
      blinking = false;
      updateAnimation(0, 0);
    }

  if(*bullets < 8)
    {
      Bullet *b = new Bullet(gfx, sfx);
      if(direction == LEFT)
	{
	  b->moveAbs(getX()-4, getY()+8);
	  b->moveLeft();
	}
      else if(direction == RIGHT)
	{
	  b->moveAbs(getX()+16, getY()+8);
	  b->moveRight();
	}
      b->setVisible(true);

      for(int i = 0;i < 8;i++)
	if(bullet[i] == 0)
	  {
	    bullet[i] = b;
	    break;
	  }

      (*bullets)++;

      sfx->playSound(6, false);
    }
}

void
Hero::die()
{
  if(dead || blinking)
    return;

  dead = true;

  gfx->showSprite(flame, false); /* just in case... */

  /* Stop walking sound */
  if(channel != -1)
    sfx->stopSound(channel);

  sfx->playSound(2, false);

  if(direction == LEFT)
    setAnimation("death left");
  else
    setAnimation("death right");

  deathTimer = 150;
}

void
Hero::update()
{
  if(dead)
    {
      deathTimer--;

      if(deathTimer == 0)
	destroyMe = true;
    }
}
