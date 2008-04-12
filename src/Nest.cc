#include "Nest.h"

Nest::Nest(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "nest", sfx)
{
  setAnimation("normal");

  blink(false);
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
Nest::spawn(Fuzz **fuzz)
{
  *fuzz = new Fuzz(gfx, sfx);
  (*fuzz)->setVisible(true);
  (*fuzz)->moveAbs(getX(), getY());
  (*fuzz)->rollRandom();  

  sfx->playSound(7, false);
}
