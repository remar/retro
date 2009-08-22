#include "RedFuzz.h"

RedFuzz::RedFuzz(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper)
  : Fuzz(gfx, sfx, scoreKeeper)
{
  // Replace Blue fuzz sprite with Red fuzz sprite
  gfx->removeSpriteInstance(sprite_instance);
  sprite_instance = gfx->createSpriteInstance("red fuzz");

  setAnimation("roll left");
  pauseAnimation(true);

  // Set when this fuzz will change direction
  directionTimeout = 3*60 + rand()%(3*60);
}

void
RedFuzz::update(Field *field, Hero *hero)
{
  // Randomly change direction every ... frames
  if(directionTimeout)
    --directionTimeout;

  if(directionTimeout == 0 && rollDirection != NONE)
    {
      rollRandom();

      directionTimeout = 3*60 + rand()%(3*60);
    }

  // Just call parent update(field, hero)
  Fuzz::update(field, hero);
}
