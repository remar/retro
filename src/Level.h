#ifndef LEVEL_H
#define LEVEL_H

#include <remar2d.h>
#include "SoundManager.h"
#include "Input.h"
#include "Object.h"
#include "Hero.h"
#include "Fuzz.h"
#include "Coin.h"
#include "Nest.h"
#include "ScoreKeeper.h"
#include "Spawner.h"
#include "Field.h"
#include <map>
#include <string>
#include <list>
#include "SDL_mixer.h"
#include "GameMode.h"
#include "BrokenBlock.h"

class Level
{
 public:
  Level(remar2d *gfx, Input *input);
  Level(remar2d *gfx, SoundManager *sfx, Input *input,
	ScoreKeeper *scoreKeeper);
  ~Level();
  bool loadLevel(char *lev);
  GameMode update(int delta);
  void breakBlock(int x, int y);
  void blockHit(int x, int y);
  void drawBlock(int x, int y);
  void drawBlockAndSurrounding(int x, int y);
  void redrawAll();
  void randomBlocks();
  bool respawn(int x, int y);

 private:
  void clearLevel();
  void moveObjectRel(Object *object, int *x, int *y);

  /* Returns true if the Object collides with the background. x and y
     is set to the background tile that the object collides with (can
     only collide with one tile...) */
  bool objectCollidesWithBackground(Object *object, int *x, int *y);

  // bool isOnGround(Object *object, int x, int y);
  void moveFuzz(Fuzz *fuzz);
  bool collides(Object *obj1, Object *obj2);
  void pause();
  void removeBullet(int i);

  Spawner *spawner;

/*   enum BlockType */
/*     { */
/*       EMPTY = 0, */
/*       SOLID = 1, */
/*       BREAKABLE = 2, */
/*       DAMAGED = 3, */
/*       BROKEN = 4 */
/*     }; */

/*   BlockType level[25][19]; */

  Field field;

  bool emptyBlock(Field::BlockType block);

  /* Graphics engine pointer */
  remar2d *gfx;

  SoundManager *sfx;

  Input *input;

  ScoreKeeper *scoreKeeper;

  bool paused;

  /* Names of tilesets */
  char *backgroundBlocks;
  char *blocks;
  char *solids;
  char *dots;
  char *spikes;

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
  list<BrokenBlock *> brokenBlocks;

  Mix_Music *music;
  Mix_Chunk *shoot;
  Mix_Chunk *pling;
  Mix_Chunk *explosion;

  bool win;
  int winTimer;

  /* Start position for the player */
  int heroStartX;
  int heroStartY;

  int debugOutputTimer;

  bool loadFailed;

  int hudSprite;
};

#endif
