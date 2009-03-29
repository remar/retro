#include "HunterBullet.h"
#include "Explosion.h"

HunterBullet::HunterBullet(remar2d *gfx, SoundManager *sfx, 
			   list<Object *> *objects)
  : Enemy(gfx, "hshot", sfx, 0), objects(objects)
{
  setBoundingBox(4, 4, 1, 1);
  setAnimation("normal");
  setVisible(true);
  direction = LEFT;
}

void
HunterBullet::moveLeft()
{
  direction = LEFT;
}

void
HunterBullet::moveRight()
{
  direction = RIGHT;
}

void
HunterBullet::update(Field *field, Hero *hero)
{
  if(destroyMe)
    return;

  if(collides(hero))
    {
      hero->die();
      destroyMe = true;
      return;
    }

  if(getX() < -6 || getX() > 800)
    {
      destroyMe = true;
      return;
    }

  int blockX, blockY;
  if(field->objectCollidesWithBackground(this, &blockX, &blockY))
    {
      if(!field->blockHit(blockX, blockY))
	{
	  objects->push_back(new Explosion(gfx, sfx, getX()-9, getY()-9));
	}
      sfx->playSound(3);
      destroyMe = true;
      return;
    }

  if(direction == LEFT)
    moveRel(-3, 0);
  else
    moveRel(3, 0);
}
