#include "Level.h"
#include <fstream>
#include <iostream>

int DEBUG = 0;

Level::Level(remar2d *gfx, Input *input)
{
}

Level::Level(remar2d *gfx, SoundManager *sfx, Input *input,
	     ScoreKeeper *scoreKeeper)
  : fuzzes(0), nests(0), coins(0), bullets(0), paused(false), win(false),
    debugOutputTimer(60), loadFailed(false)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;

  gfx->setupTileBackground(32, 32);

  /* Load tilesets */
  backgroundBlocks = gfx->loadTileSet("../gfx/background.xml");
  blocks = gfx->loadTileSet("../gfx/block1.xml");
  solids = gfx->loadTileSet("../gfx/solid1.xml");
  dots = gfx->loadTileSet("../gfx/dot.xml");
  spikes = gfx->loadTileSet("../gfx/spikes.xml");

  hudSprite = gfx->createSpriteInstance("hud");
  gfx->setAnimation(hudSprite, "normal");
  gfx->showSprite(hudSprite, true);
  gfx->moveSpriteAbs(hudSprite, 32, 8);

  for(int i = 0;i < 8;i++)
    {
      coin[i] = 0;
      fuzz[i] = 0;
      nest[i] = 0;
      bullet[i] = 0;
    }

  scoreKeeper->resetKills();

  char buf[1024];
  sprintf(buf, "../levels/%d.lev", scoreKeeper->getLevel());

  // TODO: Use exceptions instead of this error handling
  bool success = loadLevel(buf);

  if(!success)
    {
      printf("OMG! Failed to load level %d!\n", scoreKeeper->getLevel());
      loadFailed = true;
    }

  spawner = new Spawner(&nest[0], &fuzz[0],
			scoreKeeper->redFuzzes(),
			scoreKeeper->numberOfEnemy(ScoreKeeper::Fuzz));

  sfx->playMusic(0);
}

Level::~Level()
{
  // Mix_HaltMusic();
  // Mix_FreeMusic(music);
  
  printf("Level destroyed\n");
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
	      field.field[x][y+3] = (Field::BlockType)map[val];
	      break;

	    case 3:
	      heroStartX = x*32 + 8;
	      heroStartY = y*32 + 3*32 + 8;
	      break;

	    case 4:
	      if(nests <= 7)
		{
		  nest[nests] = new Nest(gfx, sfx);
		  nest[nests]->setVisible(true);
		  nest[nests]->moveAbs(x*32+3, y*32+3*32+3);
		  nests++;
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

// 	  if(x == 0)
// 	    std::cout << y << ". " << (y < 10 ? " " : "");
// 	  std::cout << level[x][y+3] << " ";
	  x++;
	  if(x >= 25)
	    {
	      x = 0; y++;
//	      std::cout << std::endl;
	    }
    }

  hero = new Hero(gfx, sfx);
  hero->setVisible(true);
  hero->moveAbs(heroStartX, heroStartY);

  redrawAll();

  return true;
}

void
Level::clearLevel()
{
  for(int y = 0;y < 19;y++)
    for(int x = 0;x < 25;x++)
      field.field[x][y] = Field::EMPTY;
  
  for(int i = 0;i < 25;i++)
    {
      field.field[i][0] = field.field[i][1] = field.field[i][2]
	= field.field[i][18] = Field::SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      field.field[0][i] = field.field[1][i] = field.field[23][i]
	= field.field[24][i] = Field::SOLID;
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

  for(int i = 0;i < nests;i++)
    {
      if(nest[i])
	{
	  delete nest[i];
	  nest[i] = 0;
	}
    }
  nests = 0;

  for(int i = 0;i < 8;i++)
    {
      if(fuzz[i])
	{
	  delete fuzz[i];
	  fuzz[i] = 0;
	}

      if(bullet[i])
	{
	  delete bullet[i];
	  bullet[i] = 0;
	}
    }
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

  for(int i = 0;i < 8;i++)
    {
      if(fuzz[i] && block.collides(fuzz[i]))
	return false;

      if(bullet[i] && block.collides(bullet[i]))
	return false;
    }

  field.field[x][y] = Field::BREAKABLE;
  drawBlockAndSurrounding(x, y);

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
	      it = brokenBlocks.erase(it);
	    }
	  else
	    {
	      (*it)->resetRespawnTimer(2*60);
	    }
	}
    }

  hero->update();
  if(hero->destroy())
    {
      delete hero;
      hero = new Hero(gfx, sfx);
      hero->setVisible(true);
      hero->moveAbs(heroStartX, heroStartY);      
    }

  int move_x = 0;
  int move_y = 1; /* Constantly fall.. :-) */

  if(input->held(SDLK_LEFT)) move_x--;
  if(input->held(SDLK_RIGHT)) move_x++;
  if(input->pressed(SDLK_UP)) hero->jump(true);
  if(input->released(SDLK_UP)) hero->jump(false);
  if(input->pressed(SDLK_z))
    {
      hero->shoot(&bullets, &bullet[0]);
    }
  if(input->pressed(SDLK_d)) hero->die();
  if(input->pressed(SDLK_p)) pause();

  if(paused)
    return GAME;

  spawner->update();

  if(hero->jumps(1))
    move_y = -1;

  int heroX = hero->getX();
  int heroY = hero->getY();

  if(heroY + 24 == field.SPIKES_LEVEL)
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
      moveObjectRel(hero, &move_x, &move_y);
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
	  if(x < -5 || x > 799)
	    {
	      removeBullet(i);
	      continue;
	    }
	  else if(objectCollidesWithBackground(bullet[i], &x, &y))
	    {
	      removeBullet(i);
	      blockHit(x, y);
	      continue;
	    }

	  for(int j = 0;j < 8;j++)
	    {
	      if(fuzz[j] && !fuzz[j]->isStunned()
		 && fuzz[j]->collides(bullet[i]))
		{
		  fuzz[j]->stun();
		  removeBullet(i);
		  break;
		}
	    }

	  if(bullet[i])
	    bullet[i]->update();
	}

    }
  
  for(int i = 0;i < 8;i++)
    {
      if(fuzz[i])
	{
	  if(fuzz[i]->destroy())
	    {
	      delete fuzz[i];
	      fuzz[i] = 0;
	      spawner->addFuzz();
	      scoreKeeper->killed(ScoreKeeper::Fuzz);
	    }
	  else
	    {
	      fuzz[i]->update(&field, hero);
	    }
	}
    }

  return GAME;
}

void
Level::breakBlock(int x, int y)
{
  field.field[x][y] = Field::BROKEN;

  brokenBlocks.push_back(new BrokenBlock(x, y));

  // Add x, y to list of breakable tiles that should be respawned
  // in the future
}

void
Level::blockHit(int x, int y)
{
  bool redraw = false;

  if(field.field[x][y] == Field::BREAKABLE)
    {
      if(scoreKeeper->blocksTakeTwoHits())
	{
	  field.field[x][y] = Field::DAMAGED;
	}
      else
	{
	  breakBlock(x, y);
	}

      redraw = true;
    }
  else if(field.field[x][y] == Field::DAMAGED)
    {
      // sfx->playSound(3, false);

      breakBlock(x, y);
      redraw = true;
    }

  sfx->playSound(3, false);

  if(redraw)
    {
      drawBlockAndSurrounding(x, y);
    }
}

void
Level::drawBlockAndSurrounding(int x, int y)
{
  drawBlock(x, y);
      
  /* Redraw blocks that surround this block */
  if(x > 2)
    drawBlock(x - 1, y);
  if(x < 22)
    drawBlock(x + 1, y);
  if(y > 3)
    drawBlock(x, y - 1);
  if(y < 16)
    drawBlock(x, y + 1);
}

/* Redraw a single block, taking neighbour blocks into consideration */
void
Level::drawBlock(int x, int y)
{
  int the_map[16] = { 0, 7, 8, 5, 9, 1, 6, 12, 10, 3, 2, 11, 4, 14, 13, 15 };

  int i = (x < 22 ? !field.emptyBlock(x+1, y): 0)
    + (y > 3  ? (!field.emptyBlock(x, y-1) << 1) : 0)
    + (x > 2  ? (!field.emptyBlock(x-1, y) << 2) : 0)
    + (y < 16 ? (!field.emptyBlock(x, y+1) << 3) : 0);

  if(field.field[x][y] == Field::SOLID)
    {
      gfx->setTile(x, y, solids, the_map[i], 0);
    }
  else if(field.field[x][y] == Field::BREAKABLE)
    {
      gfx->setTile(x, y, blocks, the_map[i], 0);
    }
  else if(field.field[x][y] == Field::DAMAGED)
    {
      gfx->setTile(x, y, blocks, the_map[i] + 16, 0);
    }
  else if(field.field[x][y] == Field::BROKEN)
    {
      gfx->setTile(x, y, dots, 0, 0);
    }
}

void
Level::redrawAll()
{
  /* Draw background */
  int map1[] = {2, 2, 4, 0};
  int map2[] = {6, 6, 8, 1};
  int map3[] = {0, 5, 7, 3};
  int map4[] = {0, 5, 6, 2};
  int map5[] = {0, 10, 9, 0};

  for(int i = 0;i < 25;i++)
    {
      gfx->setTile(i, 0, backgroundBlocks, 0, 0);
      gfx->setTile(i, 1, backgroundBlocks, 0, 0);
      gfx->setTile(i, 2, backgroundBlocks, 4, 0);
      gfx->setTile(i, 18, backgroundBlocks, 2, 0);
    }

  if(field.field[0][3] == Field::SOLID)
    {
      gfx->setTile(0, 2, backgroundBlocks, 0, 0);
      gfx->setTile(1, 2, backgroundBlocks, 11, 0);
      gfx->setTile(24, 2, backgroundBlocks, 0, 0);
      gfx->setTile(23, 2, backgroundBlocks, 12, 0);
    }

  if(field.field[0][17] == Field::SOLID)
    {
      gfx->setTile(0, 18, backgroundBlocks, 0, 0);
      if(field.field[2][17] == Field::SOLID)
	{
	  gfx->setTile(1, 18, backgroundBlocks, 0, 0);
	}
      else
	{
	  gfx->setTile(1, 18, backgroundBlocks, 9, 0);
	}
      gfx->setTile(24, 18, backgroundBlocks, 0, 0);
      gfx->setTile(23, 18, backgroundBlocks, 10, 0);
    }

  for(int i = 3;i < 17;i++)
    {
      if(field.field[0][i] == Field::SOLID)
	{
	  int i1 = (field.field[0][i-1] << 1) + field.field[0][i+1];
	  
	  gfx->setTile(0, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(24, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(1, i, backgroundBlocks, map2[i1], 0);
	  gfx->setTile(23, i, backgroundBlocks, map3[i1], 0);
	}
    }

  for(int i = 2;i < 23;i++)
    {
      if(field.field[i][17] == Field::EMPTY)
	gfx->setTile(i, 17, spikes, 0, 0);
      else
	{
	  int i1 = (field.field[i-1][17] << 1) + field.field[i+1][17];
	  gfx->setTile(i, 17, backgroundBlocks, map4[i1], 0);
	  gfx->setTile(i, 18, backgroundBlocks, map5[i1], 0);
	}
    }

  /* Draw level */
  for(int y = 3;y < 17;y++)
    for(int x = 2;x < 23;x++)
      {
	drawBlock(x, y);
      }
}

void
Level::randomBlocks()
{
  printf("Random blocks!\n");

  srand(time(0));

  for(int i = 0;i < 25;i++)
    {
      field.field[i][0] = field.field[i][1] = field.field[i][2]
	= field.field[i][18] = Field::SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      field.field[0][i] = field.field[1][i] = field.field[23][i]
	= field.field[24][i] = (Field::BlockType)(rand()%2);
    }

  for(int i = 3;i < 18;i++)
    {
      if(field.field[0][i] == Field::SOLID
	 && field.field[0][i-1] == Field::EMPTY
	 && field.field[0][i+1] == Field::EMPTY)
	{
	  field.field[0][i+1] = field.field[1][i+1] = Field::SOLID;
	}
    }

  for(int y = 3;y < 17;y++)
    for(int x = 2;x < 23;x++)
      field.field[x][y] = (Field::BlockType)(rand()%4);

  for(int y = 7;y < 12;y++)
    for(int x = 10;x < 14;x++)
      field.field[x][y] = Field::EMPTY;

  redrawAll();
}

void
Level::moveObjectRel(Object *object, int *x, int *y)
{
  /*
    1. Try to move an object (pixel by pixel)
    2. Check for intervening terrain
    3. Place object at correct position
  */

  int posX = object->getX();
  int posY = object->getY();
  SDL_Rect *box = object->getBoundingBox();

  int posX1 = posX + box->x;
  int posX2 = posX + box->x + box->w;
  int posY1 = posY + box->y;
  int posY2 = posY + box->y + box->h;

  int x_to_move = 0;
  if(*x < 0)
    {
      /* Try to move left */
      for(;x_to_move != *x;x_to_move--)
	{
	  int blockCollX;
	  if(posX1 - x_to_move - 1 < 0 || posX1 - x_to_move - 1 > 799)
	    blockCollX = 0;
	  else
	    blockCollX = (posX1-x_to_move-1)/32;
	  if(!field.emptyBlock(blockCollX, posY1/32)
	     || !field.emptyBlock(blockCollX, posY2/32))
	    {
	      *x = x_to_move;
	      break;
	    }
	}
    }
  else if(*x > 0)
    {
      /* Try to move right */
      for(;x_to_move != *x;x_to_move++)
	{
	  int blockCollX;
	  if(posX2 + x_to_move + 1 > 799)
	    blockCollX = 24;
	  else
	    blockCollX = (posX2+x_to_move+1)/32;

	  if(!field.emptyBlock(blockCollX, posY1/32)
	     || !field.emptyBlock(blockCollX, posY2/32))
	    {
	      *x = x_to_move;
	      break;
	    }
	}
    }

  /* Move x first, then possibly move y */
  posX1 += *x;
  posX2 += *x;

  int y_to_move = 0;

  int blockCollX1, blockCollX2;
  if(posX2 > 799)
    {
      blockCollX1 = blockCollX2 = 24;
    }
  else if(posX1 < 0)
    {
      blockCollX1 = blockCollX2 = 0;
    }
  else
    {
      blockCollX1 = posX1/32;
      blockCollX2 = posX2/32;
    }

  if(*y < 0)
    {
      /* Try to move up */
      for(;y_to_move != *y;y_to_move--)
	{
	  if(!field.emptyBlock(blockCollX1, (posY1-y_to_move-1)/32)
	     || !field.emptyBlock(blockCollX2,(posY1-y_to_move-1)/32))
	    {
	      *y = y_to_move;
	      break;
	    }
	}
    }
  else if(*y > 0)
    {
      /* Try to move down */
      for(;y_to_move != *y;y_to_move++)
	{
	  if(!field.emptyBlock(blockCollX1, (posY2+y_to_move+1)/32)
	     || !field.emptyBlock(blockCollX2, (posY2+y_to_move+1)/32))
	    {
	      *y = y_to_move;
 	      break;
	    }
	}
    }
}

int clip(int value, int limit)
{
  if(value < 0)
    return 0;
  else if(value > limit)
    return limit;
  else
    return value;
}

bool
Level::objectCollidesWithBackground(Object *object, int *x, int *y)
{
  int posX = object->getX();
  int posY = object->getY();
  SDL_Rect *box = object->getBoundingBox();

  int posX1 = clip((posX + box->x) / 32, 799);
  int posX2 = clip((posX + box->x + box->w) / 32, 799);
  int posY1 = clip((posY + box->y) / 32, 599);
  int posY2 = clip((posY + box->y + box->h) / 32, 599);

  /* Test if one of the four points (upper left, upper right, lower
     left, lower right) collides with the background) */
  if(!field.emptyBlock(posX1, posY1))
    { *x = posX1; *y = posY1; return true; }
  else if(!field.emptyBlock(posX2, posY1))
    { *x = posX2; *y = posY1; return true; }
  else if(!field.emptyBlock(posX1, posY2))
    { *x = posX1; *y = posY2; return true; }
  else if(!field.emptyBlock(posX2, posY2))
    { *x = posX2; *y = posY2; return true; }

  return false;
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
      paused = false;
  else
    paused = true;

  gfx->pauseAnimations(paused);
}

void
Level::removeBullet(int i)
{
  if(bullet[i] && bullets)
    {
      bullet[i]->setVisible(false);
      delete bullet[i];
      bullet[i] = 0;
      bullets--;
    }
}
