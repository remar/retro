#include "Coin.h"

Coin::Coin(remar2d *gfx)
  : Object(gfx, "coin"), collected(false)
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

      /* We hadn't collected this before */
      return true;
    }

  return false;
}
