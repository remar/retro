#include "DireSpaceViper.h"
#include "DireSpaceViperBody.h"

DireSpaceViper::DireSpaceViper(remar2d *gfx, SoundManager *sfx,
			       ScoreKeeper *scoreKeeper,
			       list<Enemy *> *enemies, int length)
  : SpaceViper(gfx, sfx, scoreKeeper, enemies, length)
{
  // Remove old sprite and body parts
  gfx->removeSpriteInstance(sprite_instance);

  // Go through list of body parts and remove from the "enemies" list

  list<SpaceViperBody *>::iterator it = body.begin();
  for(;it != body.end();it++)
    {
      enemies->remove(*it);
    }
  body.clear();

  // Create new sprite and body parts
  for(int i = 0;i < length;i++)
    {
      DireSpaceViperBody *b = new DireSpaceViperBody(gfx, sfx, i%10 == 0, (SpaceViper *)this);
      body.push_back(b);
      enemies->push_back(b);
    }

  sprite_instance = gfx->createSpriteInstance("dire snake head");
  setVisible(true);
  updateAnimation();

  /* Not initialized yet */
  moveAbs(-32, -32);
}

DireSpaceViper::~DireSpaceViper()
{

}

void DireSpaceViper::update(Field *field, Hero *hero)
{
  // 50% chance that Dire Space Viper will home on Captain Good when
  // it's time to choose direction
  aimAtHero = (bool)(rand()%2);

  SpaceViper::update(field, hero);
}
