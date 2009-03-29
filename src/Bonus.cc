#include "Bonus.h"

Bonus::Bonus(remar2d *gfx, bool spawnQuake)
  : Object(gfx, "bonus", 0), isCollected(false)
{
  randomize(spawnQuake);

  char *animations[] = {"small coins", "diamond", "gold bar", "blue pearl",
			"white ball", "lock", "power gun", "laser gun",
			"clock", "bomb", "quake"};
  setAnimation(animations[type]);
  setVisible(true);

  setBoundingBox(20, 20, 0, 0);
}

void
Bonus::randomize(bool spawnQuake)
{
  int selected;

  if(spawnQuake)
    selected = rand() % 23;
  else
    selected = rand() % 22;

  BonusType weighted[] = {SMALL_COINS, SMALL_COINS, SMALL_COINS,
			  DIAMOND, DIAMOND, DIAMOND,
			  GOLD_BAR, GOLD_BAR, GOLD_BAR,
			  BLUE_PEARL, BLUE_PEARL, BLUE_PEARL,
			  WHITE_BALL, WHITE_BALL,
			  LOCK, LOCK,
			  POWER_GUN, POWER_GUN,
			  LASER_GUN, LASER_GUN,
			  CLOCK,
			  BOMB,
			  QUAKE};

  type = weighted[selected];
  type =  LASER_GUN;
}
