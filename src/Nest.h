#ifndef NEST_H
#define NEST_H

#include <remar2d.h>
#include "Object.h"
#include "ScoreKeeper.h"
#include "Fuzz.h"

class Nest : public Object
{
 public:
  Nest(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper,
       list<Enemy *> *enemies);
  void update();
  void blink(bool on);
  void spawn(); //Fuzz **fuzz);
  bool isSpawning();
  void lock(bool l);
  void cancelSpawning();

 private:
  ScoreKeeper *scoreKeeper;
  list<Enemy *> *enemies;
  int spawnTimer;
  bool fastFuzzes;
  bool redFuzzes;
};

#endif
