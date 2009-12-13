#include "SpaceViperBody.h"
#include "SpaceViper.h"

SpaceViperBody::SpaceViperBody(remar2d *gfx, SoundManager *sfx, bool bright,
			       SpaceViper *head)
  : Enemy(gfx, "snake body", sfx, 0), head(head)
{
  if(bright)
    setAnimation("light green");
  else
    setAnimation("dark green");
  setVisible(true);

  setBoundingBox(14, 14, 9, 9);
}

void
SpaceViperBody::update(Field *field, Hero *hero)
{
  Object::update();
  if(collides(hero) && destroyTimer == 0
     && !hero->isBlinking() && !hero->isDead())
    {
      hero->die();

      head->pause();
    }
}

bool
SpaceViperBody::hit()
{
  /* Remove bullet, but no effect on the Spaaaaace Viiiiipeeeeeer! */
  return true;
}

void
SpaceViperBody::splat()
{
  setAnimation("splat");
  destroyTimer = 60;
}
