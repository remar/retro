#include "Hero.h"
#include "PowerBullet.h"
#include "LaserBeam.h"

Hero::Hero(remar2d *gfx, SoundManager *sfx, list<Bullet *> *bullets,
	   HUD *hud)
  : Object(gfx, "good", sfx),  blinking(true), jumping(false), dead(false),
    direction(RIGHT), weapon(STANDARD), jumpCounter(80),
    currentAnimation(STANDING), channel(-1), oldXDir(0), oldYDir(0),
    bullets(bullets), hud(hud), noteShown(false), hasStoppedSound(false)
{
  setAnimation("blink right");
  setBoundingBox(6, 20, 5, 4);

  flame = gfx->createSpriteInstance("flame");
  gfx->setAnimation(flame, "right");
  gfx->showSprite(flame, false);

  note = gfx->createSpriteInstance("note");
  gfx->setAnimation(note, "8");
  gfx->showSprite(note, false);
}

Hero::~Hero()
{
  /* Stop walking sound */
  if(channel != -1)
    sfx->stopSound(channel);

  gfx->showSprite(flame, false);
  gfx->removeSpriteInstance(flame);

  gfx->showSprite(note, false);
  gfx->removeSpriteInstance(note);
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
Hero::moveRel(float xDir, float yDir)
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

//   if(noteShown)
    {
      gfx->moveSpriteAbs(note, getX()+4, getY()-13);
    }

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
Hero::setVisible(bool visible)
{
  Object::setVisible(visible);

  if(flameShown)
    gfx->showSprite(flame, visible);

  if(noteShown)
    gfx->showSprite(note, visible);

  if(!visible && channel != -1)
    {
      sfx->stopSound(channel);
      channel = -1;
      hasStoppedSound = true;
    }
  else if(visible && hasStoppedSound)
    {
      channel = sfx->playSound(11, true);
      hasStoppedSound = false;
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
Hero::shoot()
{
  if(dead)
    return;

  if(blinking)
    {
      blinking = false;
      updateAnimation(0, 0);
    }

  Bullet *b;

  switch(weapon)
    {
    case STANDARD:
      b = new Bullet(gfx, sfx); 
      b->setVisible(true);
      break;

    case POWER:
      b = new PowerBullet(gfx, sfx);
      b->setVisible(true);
      weapon = STANDARD;
      hud->setValue(HUD::WEAPON, 0 /* nothing  */);
      break;

    case LASER:
      b = new LaserBeam(gfx, sfx);
      weapon = STANDARD;
      hud->setValue(HUD::WEAPON, 0 /* nothing  */);
      break;
    }

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

  bullets->push_back(b);

  sfx->playSound(6, false);
}

void
Hero::die()
{
  if(dead || blinking)
    return;

  dead = true;

  gfx->showSprite(flame, false); /* just in case... */
  gfx->showSprite(note, false);

  noteTimer = 0;
  noteShown = false;

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
  if(noteTimer)
    {
      if(--noteTimer == 0)
	{
	  noteShown = false;
	  gfx->showSprite(note, false);
	}
    }

  if(dead)
    {
      deathTimer--;

      if(deathTimer == 0)
	destroyMe = true;
    }
}

void
Hero::showNote(int ammo)
{
  if(dead)
    return;

  char *ammoString[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

  gfx->showSprite(note, true);
  gfx->setAnimation(note, ammoString[ammo]);

  noteShown = true;
  noteTimer = 60;
}

void
Hero::powerShot()
{
  weapon = POWER;
  hud->setValue(HUD::WEAPON, 1 /* POWER */);
}

void
Hero::laserShot()
{
  weapon = LASER;
  hud->setValue(HUD::WEAPON, 2 /* LASER */);
}
