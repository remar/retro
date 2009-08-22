#include "DarkBountyHunter.h"
#include "DarkHunterBullet.h"

DarkBountyHunter::DarkBountyHunter(remar2d *gfx, SoundManager *sfx,
				   ScoreKeeper *scoreKeeper,
				   list<Enemy *> *enemies,
				   list<Object *> *objects)
  : BountyHunter(gfx, sfx, scoreKeeper, enemies, objects)
{
  gfx->removeSpriteInstance(sprite_instance);
  sprite_instance = gfx->createSpriteInstance("dark hunter");

  setAnimation("left");
  setVisible(false);
  moveAbs(-32, -32);
}

void
DarkBountyHunter::update(Field *field, Hero *hero)
{
  // frameCounter should be set to 20, not random, for Dark Bounty Hunter,
  // so that Dark Bounty Hunter is hidden exactly 1/3 second
  bool resetFrameCounter = (state == FADEOUT && frameCounter == 0);

  BountyHunter::update(field, hero);

  if(resetFrameCounter)
    frameCounter = 20;
}

void
DarkBountyHunter::fire()
{
  // Create Dark Bounty Hunter Shot !
  DarkHunterBullet *b = new DarkHunterBullet(gfx, sfx, enemies, objects);
  if(facing == LEFT)
    {
      b->moveAbs(getX(), getY()+12);
      b->moveLeft();
    }
  else
    {
      b->moveAbs(getX()+24, getY()+12);
      b->moveRight();
    }

  enemies->push_back(b);
}
