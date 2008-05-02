#include "Level.h"
#include <fstream>
#include <iostream>

int DEBUG = 0;

Level::Level(remar2d *gfx, SoundManager *sfx, Input *input,
	     ScoreKeeper *scoreKeeper)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;

  coins = 0;
  bullets = 0;
  paused = false;
  win = false;
  debugOutputTimer = 60;
  loadFailed = false;
  hero = 0;

  timer = 200;
  timerTimer = 60;

  gfx->setupTileBackground(32, 32);

  for(int i = 0;i < 8;i++)
    {
      coin[i] = 0;
      bullet[i] = 0;
    }

  scoreKeeper->resetKills();

  field = new Field(gfx, sfx, &brokenBlocks, &objects,
		    scoreKeeper->blocksTakeTwoHits());

  char buf[1024];
  sprintf(buf, "../levels/%d.lev", scoreKeeper->getLevel());

  // TODO: Use exceptions instead of this error handling
  bool success = loadLevel(buf);

  if(!success)
    {
      printf("OMG! Failed to load level %d!\n", scoreKeeper->getLevel());
      loadFailed = true;
    }

  // setupHUD();
  
  hud = new HUD(gfx, scoreKeeper);

  spawner = new Spawner(&objects,
			scoreKeeper->redFuzzes(),
			scoreKeeper->numberOfEnemy(ScoreKeeper::Fuzz));

  for(int i = 0;i < scoreKeeper->numberOfEnemy(ScoreKeeper::Drone);i++)
    enemies.push_back(new Drone(gfx, sfx));

  sfx->playMusic(0);
}

// void
// Level::setupHUD()
// {
//   hudSprite = gfx->createSpriteInstance("hud");
//   gfx->setAnimation(hudSprite, "normal");
//   gfx->showSprite(hudSprite, true);
//   gfx->moveSpriteAbs(hudSprite, 32, 8);
//   gfx->neverRedraw(hudSprite, true);

//   score = new Counter(gfx, 8);
//   score->setPosition(298, 20);
//   score->setCounter(scoreKeeper->getScore());

//   topScore = new Counter(gfx, 8);
//   topScore->setPosition(522, 20);
//   topScore->setCounter(scoreKeeper->getTopScore());

//   time = new Counter(gfx, 3);
//   time->setPosition(713, 20);
//   time->setCounter(timer);

//   lives = new Counter(gfx, 2);
//   lives->setPosition(298, 50);
//   lives->setCounter(scoreKeeper->getLives());

//   level = new Counter(gfx, 2);
//   level->setPosition(458, 50);
//   level->setCounter(scoreKeeper->getLevel());

//   skill = new Counter(gfx, 1);
//   skill->setPosition(618, 50);
//   skill->setCounter(scoreKeeper->getSkillLevel());

// }

Level::~Level()
{
  printf("Level destroyed\n");
  clearLevel();

//   delete lives;
//   delete score;
//   delete level;
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

  printf("Load level: %s\n", lev);

  clearLevel();

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
		  Nest *nest = new Nest(gfx, sfx, &enemies);
		  nest->setVisible(true);
		  nest->moveAbs(x*32+3, y*32+3*32+3);
		  objects.push_back(nest);
	      }
	      break;

	    case 5:
	      if(coins <= 7)
		{
		  coin[coins] = new Coin(gfx, sfx);
		  coin[coins]->setVisible(true);
		  coin[coins]->moveAbs(x*32+8, (y+3)*32+4);
		  coins++;
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

  hero = new Hero(gfx, sfx);
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

  for(int i = 0;i < 8;i++)
    {
      if(bullet[i] && block.collides(bullet[i]))
	return false;
    }

  field->field[x][y] = Field::BREAKABLE;
  field->drawBlockAndSurrounding(x, y);

  objects.push_back(new Smoke(gfx, sfx, x*32, y*32));
  sfx->playSound(12);

  return true;
}

GameMode
Level::update(int delta)
{
  if(loadFailed)
    return QUIT;

  if(win)
    {
      --winTimer;
      if(!(winTimer % 10))
	{
	  if((winTimer / 10) % 2)
	    {
	      /* Set bright blue background */
	      printf("BRIGHT BLUE BACKGROUND\n");
	    }
	  else
	    {
	      /* Set normal background */
	      printf("NORMAL BLUE BACKGROUND\n");
	    }
	}

      /* When done, go to score tally screen */
      if(winTimer == 0)
	return SCORE;
      else
	return GAME;
    }

  if(input->pressed(SDLK_p)) pause();

  if(paused)
    return GAME;

  if(!hero->isBlinking() && --timerTimer == 0)
    {
      timerTimer = 60;

      if(timer > 0)
	{
	  timer--;

	  if(timer == 0)
	    {
	      hero->die();
	      timer = 100;
	    }

	  hud->setValue(HUD::TIME, timer);
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
  for(objIt = objects.begin();objIt != objects.end();objIt++)
    {
      (*objIt)->update();
      if((*objIt)->destroy())
	{
	  delete (*objIt);
	  objIt = objects.erase(objIt);
	}
    }

  hero->update();
  if(hero->destroy())
    {
      scoreKeeper->heroKilled();
      hud->setValue(HUD::LIVES, scoreKeeper->getLives());

      if(scoreKeeper->getLives() <= 0)
	{
	  /* GAME OVER */
	  printf("\n\nGAME OVER\n\n");

	  /* Obviously we're not supposed to quit when this happens... :-P */
	  return QUIT;
	}

      delete hero;
      hero = new Hero(gfx, sfx);
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
  if(input->pressed(SDLK_z))   hero->shoot(&bullets, &bullet[0]);
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

  /* Check for collision between Captain Good and coins */
  for(int i = 0;i < 8;i++)
    {
      if(collides(hero, coin[i]))
	{
	  if(coin[i]->collect())
	    {
	      coins--;
	      if(coins == 0)
		{
		  printf("YOU WIN!!!\n");
		  win = true;
		  winTimer = 3*60; /* Blink background for 3 seconds */

		  sfx->playMusic(1, false);
		  deleteAllObjects();//showAllObjects(false);

		  return GAME;
		  /*
		    - Remove objects (hero, fuzzes, other stuff)
		    - Blink with background
		    - Go to score tally screen
		  */
		}
	    }
	}
    }

  for(int i = 0;i < 8;i++)
    {
      if(bullet[i])
	{
	  /* check collision between bullet and various stuff... */
	  int x = bullet[i]->getX(), y = bullet[i]->getY();
	  int blockX, blockY;
	  if(x < -5 || x > 799)
	    {
	      removeBullet(i);
	      continue;
	    }
	  else if(field->objectCollidesWithBackground(bullet[i], &blockX, &blockY))
	    {
	      if(!field->blockHit(blockX, blockY))
		{
		  objects.push_back(new Explosion(gfx, sfx, x-9, y-9));
		}
	      removeBullet(i);

	      sfx->playSound(3, false);

	      continue;
	    }

	  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();
	      it++)
	    {
	      if(bullet[i]->collides(*it))
		{
		  if((*it)->hit())
		    {
		      objects.push_back(new Explosion(gfx, sfx, x-9, y-9));
		      sfx->playSound(3, false);
		      removeBullet(i);
		      break;
		    }
		}
	    }

	  if(bullet[i])
	    bullet[i]->update();
	}

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
Level::removeBullet(int i)
{
  if(bullet[i] && bullets)
    {
      delete bullet[i];
      bullet[i] = 0;
      bullets--;
    }
}

void
Level::showAllObjects(bool show)
{
  /* Remove all enemies */
  for(list<Enemy *>::iterator it = enemies.begin();it != enemies.end();it++)
    {
      (*it)->setVisible(show);
    }

  /* Remove other objects */
  for(list<Object *>::iterator it = objects.begin();it != objects.end();it++)
    {
      (*it)->setVisible(show);
    }

  hero->setVisible(show);

  for(int i = 0;i < 8;i++)
    {
      if(coin[i])
	coin[i]->setVisible(show);

      if(bullet[i])
	bullet[i]->setVisible(show);
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

  for(int i = 0;i < coins;i++)
    {
      if(coin[i])
	{
	  delete coin[i];
	  coin[i] = 0;
	}
    }
  coins = 0;

  if(hero)
    {
      delete hero;
      hero = 0;
    }

  for(int i = 0;i < 8;i++)
    {
      if(bullet[i])
	{
	  delete bullet[i];
	  bullet[i] = 0;
	}
    }
}
