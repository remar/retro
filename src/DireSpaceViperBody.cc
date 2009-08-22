#include "DireSpaceViperBody.h"

DireSpaceViperBody::DireSpaceViperBody(remar2d *gfx, SoundManager *sfx,
				       bool bright)
  : SpaceViperBody(gfx, sfx, bright)
{
  // Remove gfx, add new gfx
  gfx->removeSpriteInstance(sprite_instance);
  sprite_instance = gfx->createSpriteInstance("dire snake body");

  if(bright)
    setAnimation("light purple");
  else
    setAnimation("dark purple");

  setVisible(true);
}
