#include "DamagingExplosion.h"

DamagingExplosion::DamagingExplosion(remar2d *gfx, SoundManager *sfx, int x, int y)
  : Object(gfx, "explosion", sfx), checked(false)
{
  destroyTimer = 60;

  setAnimation("normal");
  setVisible(true);

  setBoundingBox(24, 24, 4, 4);
  moveAbs(x, y);
}

bool
DamagingExplosion::checkCollision()
{
  if(!checked)
    {
      checked = true;
      return true;
    }

  return false;
}
