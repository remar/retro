#ifndef BOUNTYHUNTER_H
#define BOUNTYHUNTER_H

#include "Enemy.h"

class BountyHunter : public Enemy
{
 public:
  BountyHunter(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
	       list<Enemy *> *enemies, list<Object *> *objects);
  virtual void update(Field *field, Hero *hero);
  bool hit();
  enum State {HIDDEN, FADEIN, PREFIRE, POSTFIRE, FADEOUT};

 private:
  void die();
  void randomizeLocation(Field *field, Hero *hero);
  void setFacing(Hero *hero);
  void updateAnimation();

  int hitPoints;

  bool dead;
  int deathTimer;

 protected:
  virtual void fire();
  State state;
  int frameCounter;

  list<Object *> *objects;
  list<Enemy *> *enemies;

  enum Direction {LEFT, RIGHT};
  Direction facing;
};

#endif // BOUNTYHUNTER_H
