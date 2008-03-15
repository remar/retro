#include "Nest.h"

Nest::Nest(remar2d *gfx)
  : Object(gfx, "nest")
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
  *fuzz = new Fuzz(gfx);
  (*fuzz)->setVisible(true);
  (*fuzz)->moveAbs(getX(), getY());
  (*fuzz)->rollRandom();  
}
