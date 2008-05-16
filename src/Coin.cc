#include "Coin.h"

Coin::Coin(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "coin", sfx), collected(false)
{
  setAnimation("blink");

  setBoundingBox(16, 24, 0, 0);
}

bool
Coin::collect()
{
  if(!collected)
    {
      /* Delete object after 120 frames */
      destroyTimer = 120;

      setAnimation("pling");
      collected = true;
      sfx->playSound(1, false);

      /* We hadn't collected this before */
      return true;
    }

  return false;
}
