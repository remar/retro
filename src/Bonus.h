#ifndef BONUS_H
#define BONUS_H

#include "Object.h"
#include "ScoreKeeper.h"

class Bonus : public Object
{
 public:
  Bonus(remar2d *gfx, bool spawnQuake);
  bool getCollected() { return isCollected; }
  void setCollected()
  { 
    isCollected = true; 
    destroyTimer = 60; 
    setVisible(false);
  }

  enum BonusType { SMALL_COINS,
		   DIAMOND,
		   GOLD_BAR,
		   BLUE_PEARL,
		   WHITE_BALL,
		   LOCK,
		   POWER_GUN,
		   LASER_GUN,
		   CLOCK,
		   BOMB,
		   QUAKE };

  BonusType getType() { return type; }


 private:
  void randomize(bool spawnQuake);

  BonusType type;
  bool isCollected;
};

#endif // BONUS_H
