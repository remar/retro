#ifndef LEVEL_H
#define LEVEL_H

#include <remar2d.h>
#include "Input.h"
#include "Object.h"
#include "Hero.h"
#include "Fuzz.h"
#include "Coin.h"
#include "Nest.h"
#include "ScoreKeeper.h"
#include "Spawner.h"
#include <map>
#include <string>

class Level
{
 public:
  Level(remar2d *gfx, Input *input);
  Level(remar2d *gfx, Input *input, ScoreKeeper *scoreKeeper);
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
  // bool isOnGround(Object *object, int x, int y);
  void moveFuzz(Fuzz *fuzz);
  bool collides(Object *obj1, Object *obj2);
  void pause();

  Spawner *spawner;

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

  ScoreKeeper *scoreKeeper;

  bool paused;

  /* Names of tilesets */
  char *backgroundBlocks;
  char *blocks;
  char *solids;

  /* Da player. */
  Hero *hero;

  /* Coins */
  int coins;
  Coin *coin[8];

  /* Nests */
  int nests;
  Nest *nest[8];

  /* Fuzzies!! */
  int fuzzes;
  Fuzz *fuzz[8];

  /* Bullets flying through the air! (KERPOWW!!) */
  int bullets;
  Bullet *bullet[8];

  /* Objects in the level (hero, enemies, nests, bonuses, shots). */
  map<string, Object *> objects;
};

#endif
