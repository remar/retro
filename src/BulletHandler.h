#ifndef BULLETHANDLER_H
#define BULLETHANDLER_H

#include "HUD.h"
#include "Hero.h"

class BulletHandler
{
 public:
  BulletHandler(HUD *hud);
  void update();
  bool fire();

 private:
  HUD *hud;

  int ammo;
  int refillCounter;
  int refillCounterTimer;
};

#endif
