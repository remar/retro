#ifndef FIELD_H
#define FIELD_H

#include <remar2d.h>
#include <list>
#include "SoundManager.h"
#include "BrokenBlock.h"
#include "Object.h"


class Field
{
 public:
  enum BlockType
    {
      EMPTY = 0,
      SOLID = 1,
      BREAKABLE = 2,
      DAMAGED = 3,
      BROKEN = 4
    };

  BlockType field[25][19];

  int SPIKES_LEVEL;

  Field(remar2d *gfx, SoundManager *sfx,
	list<BrokenBlock *> *brokenBlocks,
	list<Object *> *objects,
	bool blocksTakeTwoHits,
	int skill,
	bool redBackground);

  bool blockHit(int x, int y);
  void breakBlock(int x, int y);
  void repairBlock(int x, int y);
  void drawBlock(int x, int y);
  void drawBlockAndSurrounding(int x, int y);
  void redrawAll(bool bright = false);
  void randomBlocks();
  void moveObjectRel(Object *object, int *x, int *y);

  /* Returns true if the Object collides with the background. x and y
     is set to the background tile that the object collides with (can
     only collide with one tile...) */
  bool objectCollidesWithBackground(Object *object, int *x, int *y);

  inline bool emptyBlock(int x, int y)
  {
    /* Let checks wrap around */
    if(x == -1)
      x = 24;
    if(x == 25)
      x = 0;

    return field[x][y] == EMPTY || field[x][y] == BROKEN;
  }

 private:

  remar2d *gfx;
  SoundManager *sfx;
  list<BrokenBlock *> *brokenBlocks;
  list<Object *> *objects;
  bool blocksTakeTwoHits;
  int skill;

  /* Names of tilesets */
  char *backgroundBlocks;
  char *blocks;
  char *solids;
  char *dots;
  char *spikes;
};

#endif
