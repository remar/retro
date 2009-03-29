#ifndef BULLETHANDLER_H
#define BULLETHANDLER_H

#include "HUD.h"

class Hero; // defined in Hero.h

#include "Hero.h"


class BulletHandler
{
 public:
  BulletHandler(HUD *hud);
  void setHero(Hero *hero) {this->hero = hero; }
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
