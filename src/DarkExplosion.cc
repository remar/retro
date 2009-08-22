#include "DarkExplosion.h"

DarkExplosion::DarkExplosion(remar2d *gfx, SoundManager *sfx, int x, int y)
  : Enemy(gfx, "dark explosion", sfx, 0), firstUpdate(true)
{
  moveAbs(x, y);
  setAnimation("normal");
  setVisible(true);

  setBoundingBox(32, 32, 0, 0);

  destroyTimer = 20;
}

void
DarkExplosion::update(Field *field, Hero *hero)
{
  //printf("Update update update!\n");

  if(firstUpdate)
    {
      // Potentially destroy/damage block located under this explosion
      field->blockHit(getX()/32, getY()/32);

      firstUpdate = false;
    }

  if(collides(hero))
    {
      hero->die();
    }

  Enemy::update(field, hero);
}
