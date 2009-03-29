#ifndef SPAWNER_H
#define SPAWNER_H

#include "Nest.h"
#include "Fuzz.h"
#include "Enemy.h"
#include <list>

/* Spawn fuzzies when needed and with correct interval */

class Spawner
{
 public:
  //Spawner(Nest **nest, Fuzz **fuzz, int type, int maxNumber);
  Spawner(list<Object *> *objects, int type, int maxNumber);
  void update();
  void addFuzz();
  void lockNests();
  void cancelSpawningNests();

 private:
  // Fuzz **findEmptyFuzz();

  // Nest **nest;
  // Fuzz **fuzz;
  list<Object *> *objects;
  list<Nest *> nests;
  int type; /* 0 = Blue Fuzz, 1 = Red Fuzz */
  int maxNumber;
  int numberOfNests;

  int countDown; /* Countdown to next spawn */

  int inQueue; /* Number of fuzzes waiting to spawn */

  bool startSpawn; /* Indicates if this is the fast "start spawn" */

  int nestLockTimer;
};

#endif
