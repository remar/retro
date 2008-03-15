#include "Level.h"
#include <fstream>
#include <iostream>

Level::Level(remar2d *gfx, Input *input)
{
}

Level::Level(remar2d *gfx, Input *input, ScoreKeeper *scoreKeeper)
  : blocksTakeTwoHits(false), fuzzes(0), nests(0), coins(0), bullets(0),
    paused(false)
{
  this->gfx = gfx;
  this->input = input;
  this->scoreKeeper = scoreKeeper;

  /* Load tilesets */
  backgroundBlocks = gfx->loadTileSet("../gfx/background.xml");
  blocks = gfx->loadTileSet("../gfx/block1.xml");
  solids = gfx->loadTileSet("../gfx/solid1.xml");

  for(int i = 0;i < 8;i++)
    {
      coin[i] = 0;
      fuzz[i] = 0;
      nest[i] = 0;
      bullet[i] = 0;
    }

  char buf[1024];
  sprintf(buf, "../levels/%d.lev", scoreKeeper->getLevel());

  // TODO: Use exceptions instead of this error handling
  bool success = loadLevel(buf);

  if(!success)
    {
      printf("OMG! Failed to load level %d!\n", scoreKeeper->getLevel());
    }

  spawner = new Spawner(&nest[0], &fuzz[0],
			scoreKeeper->redFuzzes(),
			scoreKeeper->numberOfEnemy(ScoreKeeper::Fuzz));
}

Level::~Level()
{
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

  clearLevel();

  /* Start position for the player */
  int heroStartX;
  int heroStartY;

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
	  switch(val)
	    {
	    case 0:
	      level[x][y+3] = Level::EMPTY;
	      break;
	      
	    case 1:
	      level[x][y+3] = Level::BREAKABLE;
	      break;

	    case 2:
	      level[x][y+3] = Level::SOLID;
	      break;

	    case 3:
	      heroStartX = x*32 + 8;
	      heroStartY = y*32 + 3*32 + 8;
	      break;

	    case 4:
	      if(nests <= 7)
		{
		  nest[nests] = new Nest(gfx);
		  nest[nests]->setVisible(true);
		  nest[nests]->moveAbs(x*32+3, y*32+3*32+3);
		  // nest[nests]->spawn(&fuzz[fuzzes]);
		  nests++;
		  // fuzzes++;
		}
	      break;

	    case 5:
	      if(coins <= 7)
		{
		  coin[coins] = new Coin(gfx);
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

  hero = new Hero(gfx);
  hero->setVisible(true);
//   hero->moveAbs(400, 8*32+8);
//   hero->moveAbs(x*32+8, y*32+3*32+8);
  hero->moveAbs(heroStartX, heroStartY);

  redrawAll();

  return true;
}

void
Level::clearLevel()
{
  for(int y = 0;y < 19;y++)
    for(int x = 0;x < 25;x++)
      level[x][y] = EMPTY;
  
  for(int i = 0;i < 25;i++)
    {
      level[i][0] = level[i][1] = level[i][2] = level[i][18] = SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      level[0][i] = level[1][i] = level[23][i] = level[24][i] 
	= SOLID;
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
    if(fuzz[i])
      {
	delete fuzz[i];
	fuzz[i] = 0;
      }
}

void
Level::update(int delta)
{
  int move_x = 0;
  int move_y = 1; /* Constantly fall.. :-) */

  if(input->held(SDLK_LEFT)) move_x--;
  if(input->held(SDLK_RIGHT)) move_x++;
  if(input->pressed(SDLK_UP)) hero->jump(true);
  if(input->released(SDLK_UP)) hero->jump(false);
  if(input->pressed(SDLK_z)) hero->shoot(&bullets, &bullet[0]);
  if(input->pressed(SDLK_d)) hero->die();
  if(input->pressed(SDLK_p)) pause();

  if(paused)
    return;

  spawner->update();

//   if(hero->jumps(delta/10))
//     move_y = -1;
  if(hero->jumps(1))
    move_y = -1;

  int heroX = hero->getX();
  int heroY = hero->getY();

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
      // if(move_x != 0 || move_y != 0)
      hero->moveRel(move_x, move_y);//, isOnGround(hero, move_x, move_y));
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
		printf("YOU WIN!!!\n");
	    }
	}
    }

  for(int i = 0;i < 8;i++)
    {
      if(bullet[i])
	bullet[i]->update();

      if(fuzz[i])
	moveFuzz(fuzz[i]);
    }
}

void
Level::blockHit(int x, int y)
{
  if(level[x][y] == BREAKABLE)
    {
      level[x][y] = BROKEN;
      // gfx->setBackgroundTile(x, y, "broken tile", 0, 0);
      // Add x, y to list of breakable tiles that should be respawned
      // in the future
    }
}

void
Level::twoHitBlocks(bool on)
{
  blocksTakeTwoHits = on;
}

void
Level::redrawAll()
{
  /* Draw background */
  int map1[] = {2, 2, 4, 0};
  int map2[] = {6, 6, 8, 1};
  int map3[] = {0, 5, 7, 3};

  for(int i = 0;i < 25;i++)
    {
      gfx->setTile(i, 0, backgroundBlocks, 0, 0);
      gfx->setTile(i, 1, backgroundBlocks, 0, 0);
      gfx->setTile(i, 2, backgroundBlocks, 4, 0);
      gfx->setTile(i, 18, backgroundBlocks, 2, 0);
    }

  if(level[0][3] == SOLID)
    {
      gfx->setTile(0, 2, backgroundBlocks, 0, 0);
      gfx->setTile(1, 2, backgroundBlocks, 11, 0);
      gfx->setTile(24, 2, backgroundBlocks, 0, 0);
      gfx->setTile(23, 2, backgroundBlocks, 12, 0);
    }

  if(level[0][17] == SOLID)
    {
      gfx->setTile(0, 18, backgroundBlocks, 0, 0);
      gfx->setTile(1, 18, backgroundBlocks, 9, 0);
      gfx->setTile(24, 18, backgroundBlocks, 0, 0);
      gfx->setTile(23, 18, backgroundBlocks, 10, 0);
    }

  for(int i = 3;i < 18;i++)
    {
      if(level[0][i] == SOLID)
	{
	  int i1 = (level[0][i-1] << 1) + level[0][i+1];
	  
	  gfx->setTile(0, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(24, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(1, i, backgroundBlocks, map2[i1], 0);
	  gfx->setTile(23, i, backgroundBlocks, map3[i1], 0);
	}
    }

  /* Draw level */
  int the_map[16] = { 0, 7, 8, 5, 9, 1, 6, 12, 10, 3, 2, 11, 4, 14, 13, 15 };

  for(int y = 3;y < 18;y++)
    for(int x = 2;x < 23;x++)
      {
	int i = (x < 22 ? !!level[x+1][y] : 0)
	      + (y > 3  ? (!!level[x][y-1] << 1) : 0)
	      + (x > 2  ? (!!level[x-1][y] << 2) : 0)
	      + (y < 17 ? (!!level[x][y+1] << 3) : 0);

	if(level[x][y] == SOLID)
	  {
	    gfx->setTile(x, y, solids, the_map[i], 0);
	  }
	else if(level[x][y] == BREAKABLE)
	  {
	    gfx->setTile(x, y, blocks, the_map[i], 0);
	  }
	else if(level[x][y] == DAMAGED)
	  {
	    gfx->setTile(x, y, blocks, the_map[i] + 16, 0);
	  }
      }
}

void
Level::randomBlocks()
{
  printf("Random blocks!\n");

  srand(time(0));

  for(int i = 0;i < 25;i++)
    {
      level[i][0] = level[i][1] = level[i][2] = level[i][18] = SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      level[0][i] = level[1][i] = level[23][i] = level[24][i] 
	= (BlockType)(rand()%2);
    }

  for(int i = 3;i < 18;i++)
    {
      if(level[0][i] == SOLID && level[0][i-1] == EMPTY
	 && level[0][i+1] == EMPTY)
	{
	  level[0][i+1] = level[1][i+1] = SOLID;
	}
    }

  for(int y = 3;y < 17;y++)
    for(int x = 2;x < 23;x++)
      level[x][y] = (BlockType)(rand()%4);

  for(int y = 7;y < 12;y++)
    for(int x = 10;x < 14;x++)
      level[x][y] = EMPTY;

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
	  if(level[blockCollX][posY1/32] != EMPTY
	     || level[blockCollX][posY2/32] != EMPTY)
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

	  if(level[blockCollX][posY1/32] != EMPTY
	     || level[blockCollX][posY2/32] != EMPTY)
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
	  if(level[blockCollX1][(posY1-y_to_move-1)/32] != EMPTY
	     || level[blockCollX2][(posY1-y_to_move-1)/32] != EMPTY)
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
	  if(level[blockCollX1][(posY2+y_to_move+1)/32] != EMPTY
	     || level[blockCollX2][(posY2+y_to_move+1)/32] != EMPTY)
	    {
	      *y = y_to_move;
 	      break;
	    }
	}
    }
}

/*
  Move-A-Fuzz

  1. Check to see that we're still attached to something (no broken block)
  2. See how many pixels we will move
   2a. If less than required to reach a boundary between blocks, just move
   2b. If this will take us over a boundary, check if we'll hit a block
       or roll outside a block with this move

  TODO: moveFuzz should be placed in class Fuzz, supply the level[25][19] as
        argument.
*/

void
Level::moveFuzz(Fuzz *fuzz)
{
  const int speed = 1;

  int move_x = 0;
  int move_y = 0;

  int times = 1;

  Fuzz::MoveDir moveDir = fuzz->getMoveDir();
  Fuzz::MoveDir rollDir = fuzz->getRollDir();

  if(scoreKeeper->fastFuzzes())
    {
      /* Move fuzzes two times this frame (fast fuzzes) */
      times = 2;
    }


  if(moveDir == Fuzz::NONE)
    {
      fuzz->pauseAnimation(true);
      move_x = 0;
      move_y = 1;
      moveObjectRel(fuzz, &move_x, &move_y);
      if(move_y != 1)
	{
	  fuzz->pauseAnimation(false);
	  /* We've hit the floor, start rolling in some direction */
	  if(rollDir == Fuzz::LEFT)
	    fuzz->setMoveDir(Fuzz::LEFT);
	  else
	    {
	      fuzz->setMoveDir(Fuzz::RIGHT);
	      fuzz->rollRight();
	    }
	}
      else
	{
	  fuzz->moveRel(move_x, move_y);

	  /* Return to make sure we don't fall twice this frame */
	  return;
	}
    }

  for(int i = 0;i < times;i++)
    {
      moveDir = fuzz->getMoveDir();
      rollDir = fuzz->getRollDir();

      switch(rollDir)
	{
	case Fuzz::LEFT:
	  switch(moveDir)
	    {
	    case Fuzz::LEFT:
	      move_x = -1*speed;
	      move_y = 1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);
	      if(move_y == 1*speed)
		{
		  /* No floor beneath our "feet", go down */
		  fuzz->setMoveDir(Fuzz::DOWN);
		}
	      else if(move_x == 0)
		{
		  /* Wall to the left, move up */
		  fuzz->setMoveDir(Fuzz::UP);
		}
	      break;
	  
	    case Fuzz::UP:
	      move_x = -1*speed;
	      move_y = -1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);
	  
	      if(move_x == -1*speed)
		{
		  fuzz->setMoveDir(Fuzz::LEFT);
		  move_y = 0;
		}
	      else if(move_y == 0)
		fuzz->setMoveDir(Fuzz::RIGHT);
	      break;
	  
	    case Fuzz::DOWN:
	      move_x = 1*speed;
	      move_y = 1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);

	      if(move_x == 1*speed)
		{
		  fuzz->setMoveDir(Fuzz::RIGHT);
		  move_y = 0;
		}
	      else if(move_y == 0)
		fuzz->setMoveDir(Fuzz::LEFT);
	      break;

	    case Fuzz::RIGHT:
	      move_x = 1*speed;
	      move_y = -1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);

	      if(move_x == 0)
		fuzz->setMoveDir(Fuzz::DOWN);
	      else if(move_y == -1*speed)
		fuzz->setMoveDir(Fuzz::UP);
	      break;
	    }
	  break;

	case Fuzz::RIGHT:
	  switch(moveDir)
	    {
	    case Fuzz::LEFT:
	      move_x = -1*speed;
	      move_y = -1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);
	      if(move_y == -1*speed)
		{
		  /* No floor above our "feet", go up */
		  fuzz->setMoveDir(Fuzz::UP);
		}
	      else if(move_x == 0)
		{
		  /* Wall to the left, move down */
		  fuzz->setMoveDir(Fuzz::DOWN);
		}
	      break;
	  
	    case Fuzz::UP:
	      move_x = 1*speed;
	      move_y = -1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);
	  
	      if(move_x == 1*speed)
		{
		  fuzz->setMoveDir(Fuzz::RIGHT);
		  move_y = 0;
		}
	      else if(move_y == 0)
		fuzz->setMoveDir(Fuzz::LEFT);
	      break;
	  
	    case Fuzz::DOWN:
	      move_x = -1*speed;
	      move_y = 1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);

	      if(move_x == -1*speed)
		{
		  fuzz->setMoveDir(Fuzz::LEFT);
		  move_y = 0;
		}
	      else if(move_y == 0)
		fuzz->setMoveDir(Fuzz::RIGHT);
	      break;

	    case Fuzz::RIGHT:
	      move_x = 1*speed;
	      move_y = 1*speed;
	      moveObjectRel(fuzz, &move_x, &move_y);

	      if(move_x == 0)
		fuzz->setMoveDir(Fuzz::UP);
	      else if(move_y == 1*speed)
		fuzz->setMoveDir(Fuzz::DOWN);
	      break;
	    }
	  break;
	}

      fuzz->moveRel(move_x, move_y);
      if(fuzz->getX() < -25)
	fuzz->moveAbs(801, fuzz->getY());
      else if(fuzz->getX() > 802)
	fuzz->moveAbs(-24, fuzz->getY());
    }
}

inline int abs(int x)
{
  return x < 0 ? -x : x;
}

bool
Level::collides(Object *obj1, Object *obj2)
{
  SDL_Rect *bb1 = obj1->getBoundingBox();
  SDL_Rect *bb2 = obj2->getBoundingBox();

  int bb1_x1 = obj1->getX() + bb1->x;
  int bb1_y1 = obj1->getY() + bb1->y;
  int bb1_x2 = bb1_x1 + bb1->w;
  int bb1_y2 = bb1_y1 + bb1->h;

  int bb2_x1 = obj2->getX() + bb2->x;
  int bb2_y1 = obj2->getY() + bb2->y;
  int bb2_x2 = bb2_x1 + bb2->w;
  int bb2_y2 = bb2_y1 + bb2->h;

  /* Assume all objects are smaller than 32x32 (small optimization) */
  if(abs(bb1_x1 - bb2_x1) > 32 || abs(bb1_y1 - bb2_y1) > 32)
    return false;

  /* x1, y1 */
  if(bb2_x1 >= bb1_x1 && bb2_x1 <= bb1_x2
     && bb2_y1 >= bb1_y1 && bb2_y1 <= bb1_y2)
    return true;
  /* x2, y1 */
  if(bb2_x2 >= bb1_x1 && bb2_x2 <= bb1_x2
     && bb2_y1 >= bb1_y1 && bb2_y1 <= bb1_y2)
    return true;
  /* x1, y2 */
  if(bb2_x1 >= bb1_x1 && bb2_x1 <= bb1_x2
     && bb2_y2 >= bb1_y1 && bb2_y2 <= bb1_y2)
    return true;
  /* x2, y2 */
  if(bb2_x2 >= bb1_x1 && bb2_x2 <= bb1_x2
     && bb2_y2 >= bb1_y1 && bb2_y2 <= bb1_y2)
    return true;

  /* No collision */
  return false;
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
