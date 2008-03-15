#ifndef SPAWNER_H
#define SPAWNER_H

#include "Nest.h"
#include "Fuzz.h"

/* Spawn fuzzies when needed and with correct interval */

class Spawner
{
 public:
  Spawner(Nest **nest, Fuzz **fuzz, int type, int maxNumber);
  void update();
  void addFuzz();

 private:
  Fuzz **findEmptyFuzz();

  Nest **nest;
  Fuzz **fuzz;
  int type; /* 0 = Blue Fuzz, 1 = Red Fuzz */
  int maxNumber;
  int numberOfNests;

  int countDown; /* Countdown to next spawn */
  int nestCountDown[8]; /* Countdown for the individial nests when
			   spawn is about to happen */

  int inQueue; /* Number of fuzzes waiting to spawn */

  bool startSpawn; /* Indicates if this is the fast "start spawn" */
};

#endif