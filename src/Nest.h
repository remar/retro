#ifndef NEST_H
#define NEST_H

#include <remar2d.h>
#include "Object.h"
#include "Fuzz.h"

class Nest : public Object
{
 public:
  Nest(remar2d *gfx, SoundManager *sfx, list<Enemy *> *enemies,
       bool fastFuzzes);
  void update();
  void blink(bool on);
  void spawn(); //Fuzz **fuzz);
  bool isSpawning();

 private:
  list<Enemy *> *enemies;
  int spawnTimer;
  bool fastFuzzes;
};

#endif
