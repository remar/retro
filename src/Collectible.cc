#include "Collectible.h"

Collectible::Collectible(remar2d *gfx, char *sprite, SoundManager *sfx)
  : Object(gfx, sprite, sfx)
{

}

/* Standard update() implementation */
void
Collectible::update()
{
  if(destroyTimer > 0)
    {
      --destroyTimer;
      if(destroyTimer == 0)
	{
	  destroyMe = true;
	}
    }
}

/* We've collided with Captain Good, do something smarter than this */
void
Collectible::hit()
{
  destroyTimer = 60;
}
