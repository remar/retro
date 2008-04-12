#include "Coin.h"

Coin::Coin(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "coin", sfx), collected(false)
{
  setAnimation("blink");

  /* TODO: Correct bounding box */
  setBoundingBox(10, 10, 0, 0);
}

bool
Coin::collect()
{
  if(!collected)
    {
      setAnimation("pling");
      collected = true;
      sfx->playSound(1, false);

      /* We hadn't collected this before */
      return true;
    }

  return false;
}
