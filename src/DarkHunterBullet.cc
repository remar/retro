#include "DarkHunterBullet.h"
#include "DarkExplosion.h"

DarkHunterBullet::DarkHunterBullet(remar2d *gfx, SoundManager *sfx,
				   list<Enemy *> *enemies,
				   list<Object *> *objects)
  : HunterBullet(gfx, sfx, objects), enemies(enemies)
{
  // Change gfx
  gfx->removeSpriteInstance(sprite_instance);
  sprite_instance = gfx->createSpriteInstance("dhshot");
  setAnimation("normal");
  setVisible(true);
}

void
DarkHunterBullet::update(Field *field, Hero *hero)
{
  HunterBullet::update(field, hero);
}

inline bool insideLevel(int x, int y)
{
  return x >= 0 && x <= 24 && y >= 0 && y <= 18;
}

void
DarkHunterBullet::blockHit(Field *field, int blockX, int blockY)
{
  // Spawn 5 Dark Explosions!
  if(insideLevel(blockX, blockY))
     enemies->push_back(new DarkExplosion(gfx, sfx, blockX*32, blockY*32));
  if(insideLevel(blockX-1, blockY))
     enemies->push_back(new DarkExplosion(gfx, sfx, (blockX-1)*32, blockY*32));
  if(insideLevel(blockX+1, blockY))
     enemies->push_back(new DarkExplosion(gfx, sfx, (blockX+1)*32, blockY*32));
  if(insideLevel(blockX, blockY-1))
     enemies->push_back(new DarkExplosion(gfx, sfx, blockX*32, (blockY-1)*32));
  if(insideLevel(blockX, blockY+1))
     enemies->push_back(new DarkExplosion(gfx, sfx, blockX*32, (blockY+1)*32));
}

