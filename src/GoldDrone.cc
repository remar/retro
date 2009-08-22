#include "GoldDrone.h"

GoldDrone::GoldDrone(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper)
  : Drone(gfx, sfx, scoreKeeper)
{
  // Change sprite to Gold Drone sprite
  gfx->removeSpriteInstance(sprite_instance);
  sprite_instance = gfx->createSpriteInstance("gold drone");

  setAnimation("normal");
  setVisible(true);
}

void
GoldDrone::update(Field *field, Hero *hero)
{
  // 50% probability that Gold Drone homes on Captain Good
  aimAtHero = (bool)(rand()%2);

  Drone::update(field, hero);
}
