#include "Coin.h"

Coin::Coin(remar2d *gfx)
  : Object(gfx, "coin")
{
  setAnimation("blink");

  /* TODO: Correct bounding box */
  setBoundingBox(10, 10, 0, 0);
}
