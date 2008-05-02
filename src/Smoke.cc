#include "Smoke.h"

Smoke::Smoke(remar2d *gfx, SoundManager *sfx, int x, int y)
  : Object(gfx, "smoke", sfx)
{
  setAnimation("normal");
  setVisible(true);
  moveAbs(x, y);

  deathTimer = 80;
}


void
Smoke::update()
{
  if(deathTimer > 0)
    {
      --deathTimer;
      if(deathTimer == 0)
	destroyMe = true;
    }
}
