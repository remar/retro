#include "BonusSpawner.h"

BonusSpawner::BonusSpawner(remar2d *gfx, Field *field,
			   list<Bonus *> *bonuses, list<Coin *> *coins,
			   bool spawnQuake)
  : gfx(gfx), field(field), bonuses(bonuses), coins(coins),
    spawnQuake(spawnQuake), bonusRespawnTimer(0)
{
  calculateStartingPoints();
  resetTimer();
}

void
BonusSpawner::update(Hero *hero)
{
  if(hero->isBlinking())
    {
      return;
    }

  if(bonusRespawnTimer > 0 && --bonusRespawnTimer == 0)
    {
      // Spawn new bonus!
      spawnBonus(hero);
      resetTimer();
    }
}

void
BonusSpawner::calculateStartingPoints()
{
  // Go through playing field and find potential bonus locations
  int x_offset = 3;
  int y_offset = 4;

  for(int y = 0;y < 13;y++)
    for(int x = 0;x < 21;x++)
      {
	if(field->emptyBlock(x + x_offset, y + y_offset)
	   && !field->emptyBlock(x + x_offset, y + y_offset + 1))
	  {
	    points.push_back(Point(x + x_offset, y + y_offset));
	  }
      }
}

void 
BonusSpawner::resetTimer()
{
  bonusRespawnTimer = rand()%(33*60)+7*60; // 7 + random(33) seconds
}

void
BonusSpawner::spawnBonus(Hero *hero)
{
  bool bonusSpawned = false;

  Object block(gfx, "shot", 0);
  block.setAnimation("normal");
  block.setBoundingBox(32, 32, 0, 0);

  while(!bonusSpawned)
    {
      int bonusPositionIndex = rand() % points.size();

      Point p = points[bonusPositionIndex];

      block.moveAbs(p.x*32, p.y*32);

      if(field->emptyBlock(p.x, p.y) && !field->emptyBlock(p.x, p.y + 1)
	 && !block.collides(hero)
	 && !collidesWithCoins(&block)
	 && !collidesWithBonuses(&block))
	{
	  // Found position!
	  Bonus *bonus = new Bonus(gfx, spawnQuake);
	  bonus->moveAbs(p.x * 32 + 6, p.y * 32 + 6);
	  bonuses->push_back(bonus);
	  bonusSpawned = true;
	}
    }
}

bool
BonusSpawner::collidesWithCoins(Object *block)
{
  for(list<Coin *>::iterator it = coins->begin();it != coins->end();it++)
    {
      if(block->collides(*it))
	return true;
    }

  return false;
}

bool
BonusSpawner::collidesWithBonuses(Object *block)
{
  for(list<Bonus *>::iterator it = bonuses->begin();it != bonuses->end();it++)
    {
      if(block->collides(*it))
	return true;
    }

  return false;
}
