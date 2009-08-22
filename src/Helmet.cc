#include "Helmet.h"

Helmet::Helmet(remar2d *gfx, SoundManager *sfx)
  : Object(gfx, "helmet", sfx)
{
  setAnimation("normal");
  setVisible(false);
}
