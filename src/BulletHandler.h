#ifndef BULLETHANDLER_H
#define BULLETHANDLER_H

#include "HUD.h"
#include "Hero.h"

class BulletHandler
{
 public:
  BulletHandler(HUD *hud, Hero *hero);
  void update();
  bool fire();
  void reset();
  void addOne(); // Always allow firing of SPECIAL WEAPONS (add one bullet)

 private:
  HUD *hud;
  Hero *hero;

  int ammo;
  int refillCounter;
  int refillCounterTimer;
};

#endif
