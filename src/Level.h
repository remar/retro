#ifndef LEVEL_H
#define LEVEL_H

#include <remar2d.h>
#include "Input.h"
#include "Object.h"
#include "Hero.h"
#include "Fuzz.h"
#include "Coin.h"
#include <map>
#include <string>

class Level
{
 public:
  Level(remar2d *gfx, Input *input);
  Level(remar2d *gfx, Input *input, char *lev);
  ~Level();
  bool loadLevel(char *lev);
  void update(int delta);
  void blockHit(int x, int y);
  void twoHitBlocks(bool on);
  void redrawAll();
  void randomBlocks();

 private:
  void clearLevel();
  void moveObjectRel(Object *object, int *x, int *y);
  bool isOnGround(Object *object, int x, int y);
  void moveFuzz(Fuzz *fuzz);

  enum BlockType
    {
      EMPTY = 0,
      SOLID = 1,
      BREAKABLE = 2,
      DAMAGED = 3,
      BROKEN = 4
    };

  BlockType level[25][19];

  bool blocksTakeTwoHits;

  /* Graphics engine pointer */
  remar2d *gfx;

  Input *input;

  /* Names of tilesets */
  char *backgroundBlocks;
  char *blocks;
  char *solids;

  /* Da player. */
  Hero *hero;

  /* Two (2) fuzzes. */
  Fuzz *fuzz, *fuzz2;

  /* Keep track of number of coins */
  int coins;
  Coin *coin[8];

  /* Four pixel sprites to display bounding box */
  int pixel[4];

  /* Objects in the level (hero, enemies, nests, bonuses, shots). */
  map<string, Object *> objects;
};

#endif
