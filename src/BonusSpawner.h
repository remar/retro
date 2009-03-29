#ifndef BONUS_SPAWNER_H
#define BONUS_SPAWNER_H

#include <vector>
#include "remar2d.h"
#include "Field.h"
#include "Bonus.h"
#include "Hero.h"
#include "Coin.h"

class BonusSpawner
{
 public:
  BonusSpawner(remar2d *gfx, Field *field, list<Bonus *> *bonuses,
	       list<Coin *> *coins, bool spawnQuake);
  void update(Hero *hero);

 private:
  void calculateStartingPoints();
  void resetTimer();
  void spawnBonus(Hero *hero);
  bool collidesWithCoins(Object *block);
  bool collidesWithBonuses(Object *block);

  struct Point
  { 
    Point(int x, int y) : x(x), y(y) {};
    int x; 
    int y; 
  };

  remar2d *gfx;
  Field *field;
  list<Bonus *> *bonuses;
  list<Coin *> *coins;
  bool spawnQuake;

  int bonusRespawnTimer;

  vector<Point> points;
};

#endif // BONUS_SPAWNER_H
