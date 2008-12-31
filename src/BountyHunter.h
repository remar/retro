#ifndef BOUNTYHUNTER_H
#define BOUNTYHUNTER_H

#include "Enemy.h"

class BountyHunter : public Enemy
{
 public:
  BountyHunter(remar2d *gfx, SoundManager *sfx,
	       list<Enemy *> *enemies, list<Object *> *objects);
  void update(Field *field, Hero *hero);
  bool hit();

 private:
  void die();
  void randomizeLocation(Field *field, Hero *hero);
  void setFacing(Hero *hero);
  void updateAnimation();
  void fire();

  enum Direction {LEFT, RIGHT};
  enum State {HIDDEN, FADEIN, PREFIRE, POSTFIRE, FADEOUT};

  list<Enemy *> *enemies;
  list<Object *> *objects;

  Direction facing;
  State state;

  int frameCounter;
  
  int hitPoints;

  bool dead;
  int deathTimer;
};

#endif // BOUNTYHUNTER_H
