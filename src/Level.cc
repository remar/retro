#include "Level.h"
#include <fstream>
#include <iostream>

int DEBUG = 0;

Level::Level(remar2d *gfx, SoundManager *sfx, Input *input,
	     ScoreKeeper *scoreKeeper)
  : GameMode(gfx, sfx, input, scoreKeeper)
{
  //bullets = 0;
  paused = false;
  win = false;
  debugOutputTimer = 60;
  loadFailed = false;
  hero = 0;

  scoreKeeper->setTimer(200);
  timerTimer = 60;
  timerPaused = true;

  doWipe = false;
  wipeTimer = 70;
  wipeCounter = 0;

  gfx->setupTileBackground(32, 32);

//   for(int i = 0;i < 8;i++)
//     {
//       bullet[i] = 0;
//     }

  scoreKeeper->resetKills();

  field = new Field(gfx, sfx, &brokenBlocks, &objects,
		    scoreKeeper->blocksTakeTwoHits(),
		    scoreKeeper->getSkillLevel(),
		    false /* normal blue background */);

  char buf[1024];
  sprintf(buf, "../levels/%d.lev", scoreKeeper->getLevel());

  // TODO: Use exceptions instead of this error handling
  bool success = loadLevel(buf);

  if(!success)
    {
      printf("OMG! Failed to load level %d!\n", scoreKeeper->getLevel());
      loadFailed = true;
    }

  hud = new HUD(gfx, scoreKeeper);

  bulletHandler = new BulletHandler(hud);

  spawner = new Spawner(&objects,
			scoreKeeper->redFuzzes(),
			scoreKeeper->numberOfEnemy(ScoreKeeper::Fuzz));

  for(int i = 0;i < scoreKeeper->numberOfEnemy(ScoreKeeper::Drone);i++)
    enemies.push_back(new Drone(gfx, sfx));

  if(scoreKeeper->numberOfEnemy(ScoreKeeper::SpaceViper))
    {
      SpaceViper *v = new SpaceViper(gfx, sfx, &enemies);
      enemies.push_front(v);
    }

  if(scoreKeeper->numberOfEnemy(ScoreKeeper::BountyHunter))
    {
      enemies.push_back(new BountyHunter(gfx, sfx, &enemies, &objects));
    }

  sfx->playMusic(0);
}

Level::~Level()
{
  if(hud)
    delete hud;

  clearLevel();
}

bool
Level::loadLevel(char *lev)
{
  /*
    
  0: Empty
  1: Breakable
  2: Solid
  3: Captain Good
  4: Fuzz Nest
  5: Golden Coin

  */

  clearLevel();
  scoreKeeper->resetCoins();

  std::ifstream file(lev);
  if(!file)
    {
      /* Failed to open file */
      return false;
    }

  int x = 0, y = 0;
  int val;

  while(!file.eof())
    {
      file >> val;

      if(x >= 0 && x <= 24 && y >= 0 && y <= 14)
	{
	  int map[] = { Field::EMPTY, Field::BREAKABLE, Field::SOLID };
	  switch(val)
	    {
	    case 0: case 1: case 2:
	      field->field[x][y+3] = (Field::BlockType)map[val];
	      break;

	    case 3:
	      heroStartX = x*32 + 8;
	      heroStartY = y*32 + 3*32 + 8;
	      break;

	    case 4:
	      {
		Nest *nest = new Nest(gfx, sfx, &enemies,
				      scoreKeeper->fastFuzzes());
		  nest->setVisible(true);
		  nest->moveAbs(x*32+3, y*32+3*32+3);
		  objects.push_back(nest);
	      }
	      break;

	    case 5:
	      {
		  Coin *coin = new Coin(gfx, sfx);
		  coin->setVisible(true);
		  coin->moveAbs(x*32+8, (y+3)*32 + 4);
		  coins.push_back(coin);

		  scoreKeeper->addCoin();
	      }
	      break;
	    }
	}
      
      x++;
      if(x >= 25)
	{
	  x = 0; y++;
	}
    }

  hero = new Hero(gfx, sfx, &bullets);
  hero->setVisible(true);
  hero->moveAbs(heroStartX, heroStartY);

  field->redrawAll();

  return true;
}

void
Level::clearLevel()
{
  for(int y = 0;y < 19;y++)
    for(int x = 0;x < 25;x++)
      field->field[x][y] = Field::EMPTY;
  
  for(int i = 0;i < 25;i++)
    {
      field->field[i][0] = field->field[i][1] = field->field[i][2]
	= field->field[i][18] = Field::SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      field->field[0][i] = field->field[1][i] = field->field[23][i]
	= field->field[24][i] = Field::SOLID;
    }

  deleteAllObjects();
}

bool
Level::respawn(int x, int y)
{
  Object block(gfx, "shot", sfx);
  block.setAnimation("normal");
  block.setBoundingBox(32, 32, 0, 0);
  block.moveAbs(x*32, y*32);

  /* Check for collision against Captain Good, enemies, and shots */
  if(block.collides(hero))
    return false;

  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();it++)
    {
      if(block.collides((*it)))
	return false;
    }

  for(int i = 0;i < 8;i++);
  for(list<Bullet *>::iterator it = bullets.begin();it != bullets.end();it++)
    {
      if(block.collides(*it))
	return false;
    }

  field->field[x][y] = Field::BREAKABLE;
  field->drawBlockAndSurrounding(x, y);

  objects.push_back(new Smoke(gfx, sfx, x*32, y*32));
  sfx->playSound(12);

  return true;
}

Mode
Level::update()
{
  if(loadFailed)
    return QUIT;

  if(doWipe)
    {
      --wipeTimer;

      if(wipeTimer == 0)
	{
	  return returnMode;
	}

       if(wipeTimer % 2 && wipeCounter < 19)
 	{
	  for(int x = 0;x < 25;x++)
	    {
	      gfx->setTile(x, wipeCounter, 0, 0, 0);
	    }

	  wipeCounter++;
 	}

      return GAME;
    }

  if(input->pressed(SDLK_ESCAPE))
    {
      performWipe(MENU);
      return GAME;
    }

  if(win)
    {
      --winTimer;
      if(!(winTimer % 10))
	{
	  if((winTimer / 10) % 2)
	    {
	      /* Set bright blue background */
	      field->redrawAll(true);
	    }
	  else
	    {
	      /* Set normal background */
	      field->redrawAll(false);
	    }
	}

      /* When done, go to score tally screen */
      if(winTimer == 0)
	{
	  performWipe(SCORE);
	}

      return GAME;
    }

  if(input->pressed(SDLK_p)) pause();

  if(paused)
    return GAME;


  if(hero->isDead())
    {
      timerPaused = true;
    }

  if(timerPaused && !hero->isBlinking() && !hero->isDead())
    {
      timerPaused = false;
    }

  if(!timerPaused && --timerTimer == 0)
    {
      timerTimer = 60;

      if(scoreKeeper->getTimer() > 0)
	{
	  scoreKeeper->decTimer();
	
	  hud->setValue(HUD::TIME, scoreKeeper->getTimer());

	  if(scoreKeeper->getTimer() == 0)
	    {
	      hero->die();
	      scoreKeeper->setTimer(100);
	      timerPaused = true;
	    }
	}
    }

  if(DEBUG && --debugOutputTimer == 0)
    {
      printf("----------------------------------------\n");
      printf("Broken blocks\n");

      debugOutputTimer = 60;

      list<BrokenBlock *>::iterator it = brokenBlocks.begin();
      for(;it != brokenBlocks.end();it++)
	{
	  BrokenBlock *b = *it;
	  printf("%d, %d, respawn: %d\n",
		 b->getX(), b->getY(), b->respawn());
	  
	}
    }

  list<BrokenBlock *>::iterator it;
  for(it = brokenBlocks.begin();it != brokenBlocks.end();it++)
    {
      (*it)->update();
      if((*it)->respawn())
	{
	  int x = (*it)->getX();
	  int y = (*it)->getY();

	  if(respawn(x, y))
	    {
	      delete (*it);
	      it = brokenBlocks.erase(it);
	    }
	  else
	    {
	      (*it)->resetRespawnTimer(2*60);
	    }
	}
    }

  list<Object *>::iterator objIt;
  for(objIt = objects.begin();objIt != objects.end();)
    {
      (*objIt)->update();
      if((*objIt)->destroy())
	{
	  delete (*objIt);
	  objIt = objects.erase(objIt);
	  continue;
	}
      objIt++;
    }

  bulletHandler->update();

  hero->update();
  if(hero->destroy())
    {
      scoreKeeper->heroKilled();
      hud->setValue(HUD::LIVES, scoreKeeper->getLives());
      hud->setValue(HUD::TIME, scoreKeeper->getTimer());

      timerPaused = true;

      if(scoreKeeper->getLives() <= 0)
	{
	  /* GAME OVER */
	  printf("\n\nGAME OVER\n\n");

	  performWipe(SCORE);
	  return GAME;
	  // return SCORE;
	}

      delete hero;
      hero = new Hero(gfx, sfx, &bullets);
      hero->setVisible(true);
      hero->moveAbs(heroStartX, heroStartY);      
    }

  // TODO: Move movement code for Captain Good to Hero class

  int move_x = 0;
  int move_y = 1; /* Constantly fall.. :-) */

  if(input->held(SDLK_LEFT))   move_x--;
  if(input->held(SDLK_RIGHT))  move_x++;
  if(input->pressed(SDLK_UP))  hero->jump(true);
  if(input->released(SDLK_UP)) hero->jump(false);
//   if(input->pressed(SDLK_z) && bulletHandler->fire())
//     hero->shoot(&bullets, &bullet[0]);
  if(input->pressed(SDLK_z) && bulletHandler->fire())
    hero->shoot(); //&bullets, &bullet[0]);
  if(input->pressed(SDLK_d))   hero->die();

  spawner->update();

  if(hero->jumps(1))
    move_y = -1;

  int heroX = hero->getX();
  int heroY = hero->getY();

  if(heroY + 24 == field->SPIKES_LEVEL)
    hero->die();

  if(heroX < -25)
    {
      hero->moveAbs(801, heroY);
    }
  else if(heroX > 802)
    {
      hero->moveAbs(-24, heroY);
    }
  else
    {
      field->moveObjectRel(hero, &move_x, &move_y);
      hero->moveRel(move_x, move_y);
    }

  for(list<Coin *>::iterator it = coins.begin();it != coins.end();)
    {
      (*it)->update();
      if((*it)->destroy())
	{
	  delete (*it);
	  it = coins.erase(it);

	  continue;
	}

      if((*it)->collides(hero) && (*it)->collect())
	{
	  scoreKeeper->collectCoin();

	  if(scoreKeeper->coinsLeft() == 0)
	    {
	      win = true;
	      winTimer = 3*60; /* Blink background for 3 seconds */
	      
	      sfx->playMusic(1, false);
	      deleteAllObjects();
	      
	      return GAME;
	    }
	}

      it++;
    }

  //for(int i = 0;i < 8;i++);
  for(list<Bullet *>::iterator b = bullets.begin();b != bullets.end();)
    {
//       if(bullet[i])
// 	{
      /* check collision between bullet and various stuff... */
      int x = (*b)->getX(), y = (*b)->getY();
      int blockX, blockY;
      if(x < -5 || x > 799)
	{
	  //removeBullet(i);
	  delete (*b);
	  b = bullets.erase(b);
	  continue;
	}
      else if(field->objectCollidesWithBackground((*b), &blockX, &blockY))
	{
	  if(!field->blockHit(blockX, blockY))
	    {
	      objects.push_back(new Explosion(gfx, sfx, x-9, y-9));
	    }

	  sfx->playSound(3, false);

	  //removeBullet(i);
	  delete (*b);
	  b = bullets.erase(b);

	  continue;
	}

      bool bulletRemoved = false;
      bool removeBullet = false;

      for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();
	  it++)
	{
	  if((*b)->collides(*it))
	    {
	      if((*it)->hit())
		{
		  if(!removeBullet)
		    {
		      objects.push_back(new Explosion(gfx, sfx, x-9, y-9));
		      sfx->playSound(3, false);
		    }

		  removeBullet = true;

		  //removeBullet(i);
		  break;
		}
	    }
	}

      if(removeBullet)
	{
	  delete (*b);
	  b = bullets.erase(b);
	  bulletRemoved = true;
	}

      if(!bulletRemoved)
	(*b)->update();

      b++;
    }

  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();)
    {
      (*it)->update(field, hero);
      if((*it)->destroy())
	{
	  /* If this is a Fuzz, make sure that Spawner respawns a Fuzz
	     later on */
	  if(dynamic_cast<Fuzz *>(*it))
	    {
 	      scoreKeeper->killed(ScoreKeeper::Fuzz);
	      spawner->addFuzz();
	    }
	  else if(dynamic_cast<Drone *>(*it))
	    {
 	      scoreKeeper->killed(ScoreKeeper::Drone);
	    }
	  else if(dynamic_cast<SpaceViper *>(*it))
	    {
	      scoreKeeper->killed(ScoreKeeper::SpaceViper);
	    }
	  else if(dynamic_cast<BountyHunter *>(*it))
	    {
	      scoreKeeper->killed(ScoreKeeper::BountyHunter);
	      // TODO: Add explosions
	    }

	  delete (*it);
	  it = enemies.erase(it);
	}
      else
	{
	  it++;
	}
    }

  return GAME;
}

bool
Level::collides(Object *obj1, Object *obj2)
{
  return obj1->collides(obj2);
}

void
Level::pause()
{
  if(paused)
    {
      paused = false;
      showAllObjects(true);
      sfx->playSound(13);
    }
  else
    {
      paused = true;
      showAllObjects(false);
      sfx->playSound(13);
    }

  gfx->pauseAnimations(paused);
}

void
Level::showAllObjects(bool show)
{
  /* Show/hide all enemies */
  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();it++)
    {
      (*it)->setVisible(show);
    }

  /* Show/hide other objects */
  for(list<Object *>::iterator it = objects.begin();it != objects.end();it++)
    {
      (*it)->setVisible(show);
    }

  for(list<Coin *>::iterator it = coins.begin();it != coins.end();it++)
    {
      (*it)->setVisible(show);
    }

  hero->setVisible(show);

  for(list<Bullet *>::iterator it = bullets.begin();it != bullets.end();it++)
    {
      (*it)->setVisible(show);
    }
}

void
Level::deleteAllObjects()
{
  /* Remove all enemies */
  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();)//it++)
    {
      delete (*it);
      it = enemies.erase(it);
    }

  /* Remove other objects */
  for(list<Object *>::iterator it = objects.begin();it != objects.end();)//it++)
    {
      delete (*it);
      it = objects.erase(it);
    }

  for(list<Coin *>::iterator it = coins.begin();it != coins.end();)
    {
      delete (*it);
      it = coins.erase(it);
    }

  if(hero)
    {
      delete hero;
      hero = 0;
    }

  for(list<Bullet *>::iterator it = bullets.begin();it != bullets.end();it++)
    {
      delete (*it);
      it = bullets.erase(it);
    }
}

void
Level::performWipe(Mode modeToReturn)
{
  sfx->stopMusic();

  deleteAllObjects();
  
  delete hud;
  hud = 0;

  returnMode = modeToReturn;
  doWipe = true;
}
