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
#include "Explosion.h"
#include "Smoke.h"
#include "Enemy.h"
#include "Collectible.h"
#include "Counter.h"
#include "Drone.h"
#include "HUD.h"

class Level
{
 public:
  Level(remar2d *gfx, SoundManager *sfx, Input *input,
	ScoreKeeper *scoreKeeper);
  ~Level();
  GameMode update(int delta);

 private:
  bool loadLevel(char *lev);
  bool respawn(int x, int y);
  void clearLevel();
  bool collides(Object *obj1, Object *obj2);
  void pause();
  void removeBullet(int i);
  void showAllObjects(bool show);
  void deleteAllObjects();
  void setupHUD();

  Spawner *spawner;

  Field *field;

  /* Graphics engine pointer */
  remar2d *gfx;

  SoundManager *sfx;

  Input *input;

  ScoreKeeper *scoreKeeper;

  bool paused;

  /* Da player. */
  Hero *hero;

  /* Coins */
  int coins;
  Coin *coin[8];

  /* Bullets flying through the air! (KERPOWW!!) */
  int bullets;
  Bullet *bullet[8];

  list<BrokenBlock *> brokenBlocks;
  list<Object *> objects;
  list<Enemy *> enemies;
  list<Collectible *> collectibles;

/*   Mix_Music *music; */
/*   Mix_Chunk *shoot; */
/*   Mix_Chunk *pling; */
/*   Mix_Chunk *explosion; */

  bool win;
  int winTimer;

  /* Start position for the player */
  int heroStartX;
  int heroStartY;

  int debugOutputTimer;

  bool loadFailed;

  int hudSprite;

  /* Timer counts down from 200 to 0, timerTimer keeps track of when
     timer should be decremented... (every 60 frames I guess) */
  int timer;
  int timerTimer;
  bool timerPaused;

  HUD *hud;
};

#endif
