#include "BountyHunter.h"
#include "HunterBullet.h"

BountyHunter::BountyHunter(remar2d *gfx, SoundManager *sfx,
			   ScoreKeeper *scoreKeeper, list<Enemy *> *enemies,
			   list<Object *> *objects)
  : Enemy(gfx, "hunter", sfx, scoreKeeper), dead(false), hitPoints(4),
    enemies(enemies), objects(objects)
{
  setAnimation("left");
  setVisible(false);
  moveAbs(-32, -32);
  setBoundingBox(24, 24, 0, 0);

  frameCounter = 3*60;
  state = HIDDEN;
}

void
BountyHunter::update(Field *field, Hero *hero)
{
  if(dead)
    {
      destroyMe = true;
      return;
    }

  if(hero->isBlinking() && state == HIDDEN)
    return;

  if(collides(hero) && state != HIDDEN)
    hero->die();

  //char *str[] = {"HIDDEN", "FADEIN", "PREFIRE", "POSTFIRE", "FADEOUT"};
  //printf("[%s] counter: %d\n", str[state], frameCounter);

  switch(state)
    {
    case HIDDEN:
      if(frameCounter == 0)
	{
	  setVisible(true);
	  frameCounter = 60;
	  randomizeLocation(field, hero);
	  state = FADEIN;
	  updateAnimation();
	}
      break;

    case FADEIN:
      setFacing(hero);

      if(frameCounter == 0)
	{
	  frameCounter = 60;
	  state = PREFIRE;
	  updateAnimation();
	}
      break;

    case PREFIRE:
      setFacing(hero);

      if(frameCounter == 0)
	{
	  // TODO: Fire bullet
	  fire();
	  frameCounter = 60;
	  state = POSTFIRE;
	  updateAnimation();
	}
      break;

    case POSTFIRE:
      if(frameCounter == 0)
	{
	  frameCounter = 60;
	  state = FADEOUT;
	  updateAnimation();
	}
      break;

    case FADEOUT:
      if(frameCounter == 0)
	{
	  frameCounter = rand() % (3*60);
	  state = HIDDEN;
	  setVisible(false);
	}
      break;
    }

  if(frameCounter)
    --frameCounter;
}

bool
BountyHunter::hit()
{
  if(dead || state == HIDDEN)
    return false;

  if(--hitPoints == 0)
    die();

  return true;
}

void
BountyHunter::die()
{
  scoreKeeper->killed(ScoreKeeper::BountyHunter);

  dead = true;
}

void
BountyHunter::randomizeLocation(Field *field, Hero *hero)
{
  /* Randomize location */
  bool done = false;
  while(!done)
    {
      int x = (rand()%21)*32 + 96 + 4;
      int y = (rand()%14)*32 + 128 + 4;
      
      if(field->emptyBlock(x/32, y/32))
	{
	  moveAbs(x, y);

	  if(!collides(hero))
	    done = true;
	}
    }
}

void
BountyHunter::setFacing(Hero *hero)
{
  Direction oldFacing = facing;

  //printf("hero: %d, hunter: %d\n", hero->getX() + 7, getX());
  if((hero->getX()+7) < getX())
    facing = LEFT;
  else
    facing = RIGHT;

  if(facing != oldFacing)
    updateAnimation();
}

void
BountyHunter::updateAnimation()
{
  const char *facings[] = {"left", "right", "blink left", "blink right"};
  int anim = facing + ((state == FADEIN || state == FADEOUT) ? 2 : 0);
  setAnimation((char *)facings[anim]);
}

void
BountyHunter::fire()
{
  HunterBullet *b = new HunterBullet(gfx, sfx, objects);
  if(facing == LEFT)
    {
      b->moveAbs(getX(), getY()+12);
      b->moveLeft();
    }
  else
    {
      b->moveAbs(getX()+24, getY()+12);
      b->moveRight();      
    }

  enemies->push_back(b);
}
