#include "Nest.h"

Nest::Nest(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
	   list<Enemy *> *enemies, bool fastFuzzes)
  : Object(gfx, "nest", sfx), scoreKeeper(scoreKeeper), spawnTimer(0)
{
  this->enemies = enemies;
  this->fastFuzzes = fastFuzzes;

  setAnimation("normal");

  blink(false);
}

void
Nest::update()
{
  if(spawnTimer > 0)
    {
      --spawnTimer;
      if(spawnTimer == 0)
	{
	  Fuzz *fuzz = new Fuzz(gfx, sfx, scoreKeeper);
	  fuzz->setVisible(true);
	  fuzz->moveAbs(getX(), getY());
	  fuzz->rollRandom();  

	  if(fastFuzzes)
	    fuzz->isFast();

	  enemies->push_back(fuzz);

	  sfx->playSound(7, false);  	  
	  blink(false);
	}
    }
}

void
Nest::blink(bool on)
{
  if(on)
    setAnimation("blink");
  else
    setAnimation("normal");
}

void
Nest::spawn()
{
  spawnTimer = 60;
  blink(true);
}

bool
Nest::isSpawning()
{
  return spawnTimer > 0;
}

void
Nest::lock(bool l)
{
  if(l)
    setAnimation("locked");
  else
    setAnimation("normal");
}

void
Nest::cancelSpawning()
{
  spawnTimer = 0;
  setAnimation("normal");
}
