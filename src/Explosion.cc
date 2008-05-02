#include "Explosion.h"

Explosion::Explosion(remar2d *gfx, SoundManager *sfx, int x, int y)
  : Object(gfx, "explosion", sfx)
{
  deathTimer = 60;
  setAnimation("normal");
  setVisible(true);
  moveAbs(x, y);
}

void
Explosion::update()
{
  if(deathTimer > 0)
    {
      --deathTimer;
      if(deathTimer == 0)
	destroyMe = true;
    }
}
