#include "Field.h"
#include "Explosion.h"

Field::Field(remar2d *gfx, SoundManager *sfx,
	     list<BrokenBlock *> *brokenBlocks,
	     list<Object *> *objects,
	     bool blocksTakeTwoHits)
  : SPIKES_LEVEL(600 - 32 - 16)
{
  this->gfx = gfx;
  this->sfx = sfx;
  this->brokenBlocks = brokenBlocks;
  this->objects = objects;
  this->blocksTakeTwoHits = blocksTakeTwoHits;

  /* Load tilesets */
  backgroundBlocks = gfx->loadTileSet("../gfx/background.xml");
  blocks = gfx->loadTileSet("../gfx/block1.xml");
  solids = gfx->loadTileSet("../gfx/solid1.xml");
  dots = gfx->loadTileSet("../gfx/dot.xml");
  spikes = gfx->loadTileSet("../gfx/spikes.xml");
}

/* Return true if this destroyed a block, false otherwise */
bool
Field::blockHit(int x, int y)
{
  bool redraw = false;
  bool brokeABlock = false;

  if(field[x][y] == Field::BREAKABLE)
    {
      if(blocksTakeTwoHits)
	{
	  field[x][y] = Field::DAMAGED;
	}
      else
	{
	  breakBlock(x, y);
	  brokeABlock = true;
	}

      redraw = true;
    }
  else if(field[x][y] == Field::DAMAGED)
    {
      // sfx->playSound(3, false);

      breakBlock(x, y);
      redraw = true;
      brokeABlock = true;
    }

  if(redraw && !brokeABlock)
    {
      drawBlockAndSurrounding(x, y);
    }

  return brokeABlock;
}

void
Field::breakBlock(int x, int y)
{
  field[x][y] = Field::BROKEN;

  // Add x, y to list of breakable tiles that should be respawned
  // in the future
  brokenBlocks->push_back(new BrokenBlock(x, y));

  objects->push_back(new Explosion(gfx, sfx, x*32+4, y*32+4));

  drawBlockAndSurrounding(x, y);
}

void
Field::drawBlockAndSurrounding(int x, int y)
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
Field::drawBlock(int x, int y)
{
  int the_map[16] = { 0, 7, 8, 5, 9, 1, 6, 12, 10, 3, 2, 11, 4, 14, 13, 15 };

  int i = (x < 22 ? !emptyBlock(x+1, y): 0)
    + (y > 3  ? (!emptyBlock(x, y-1) << 1) : 0)
    + (x > 2  ? (!emptyBlock(x-1, y) << 2) : 0)
    + (y < 16 ? (!emptyBlock(x, y+1) << 3) : 0);

  if(field[x][y] == Field::SOLID)
    {
      gfx->setTile(x, y, solids, the_map[i], 0);
    }
  else if(field[x][y] == Field::BREAKABLE)
    {
      gfx->setTile(x, y, blocks, the_map[i], 0);
    }
  else if(field[x][y] == Field::DAMAGED)
    {
      gfx->setTile(x, y, blocks, the_map[i] + 16, 0);
    }
  else if(field[x][y] == Field::BROKEN)
    {
      gfx->setTile(x, y, dots, 0, 0);
    }
}

void
Field::redrawAll()
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

  if(field[0][3] == Field::SOLID)
    {
      gfx->setTile(0, 2, backgroundBlocks, 0, 0);
      gfx->setTile(1, 2, backgroundBlocks, 11, 0);
      gfx->setTile(24, 2, backgroundBlocks, 0, 0);
      gfx->setTile(23, 2, backgroundBlocks, 12, 0);
    }

  if(field[0][17] == Field::SOLID)
    {
      gfx->setTile(0, 18, backgroundBlocks, 0, 0);
      if(field[2][17] == Field::SOLID)
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
      if(field[0][i] == Field::SOLID)
	{
	  int i1 = (field[0][i-1] << 1) + field[0][i+1];
	  
	  gfx->setTile(0, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(24, i, backgroundBlocks, map1[i1], 0);
	  gfx->setTile(1, i, backgroundBlocks, map2[i1], 0);
	  gfx->setTile(23, i, backgroundBlocks, map3[i1], 0);
	}
    }

  for(int i = 2;i < 23;i++)
    {
      if(field[i][17] == Field::EMPTY)
	gfx->setTile(i, 17, spikes, 0, 0);
      else
	{
	  int i1 = (field[i-1][17] << 1) + field[i+1][17];
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
Field::randomBlocks()
{
  printf("Random blocks!\n");

  srand(time(0));

  for(int i = 0;i < 25;i++)
    {
      field[i][0] = field[i][1] = field[i][2]
	= field[i][18] = Field::SOLID;
    }

  for(int i = 3; i < 18;i++)
    {
      field[0][i] = field[1][i] = field[23][i]
	= field[24][i] = (Field::BlockType)(rand()%2);
    }

  for(int i = 3;i < 18;i++)
    {
      if(field[0][i] == Field::SOLID
	 && field[0][i-1] == Field::EMPTY
	 && field[0][i+1] == Field::EMPTY)
	{
	  field[0][i+1] = field[1][i+1] = Field::SOLID;
	}
    }

  for(int y = 3;y < 17;y++)
    for(int x = 2;x < 23;x++)
      field[x][y] = (Field::BlockType)(rand()%4);

  for(int y = 7;y < 12;y++)
    for(int x = 10;x < 14;x++)
      field[x][y] = Field::EMPTY;

  redrawAll();
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
Field::objectCollidesWithBackground(Object *object, int *x, int *y)
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
  if(!emptyBlock(posX1, posY1))
    { *x = posX1; *y = posY1; return true; }
  else if(!emptyBlock(posX2, posY1))
    { *x = posX2; *y = posY1; return true; }
  else if(!emptyBlock(posX1, posY2))
    { *x = posX1; *y = posY2; return true; }
  else if(!emptyBlock(posX2, posY2))
    { *x = posX2; *y = posY2; return true; }

  return false;
}

void
Field::moveObjectRel(Object *object, int *x, int *y)
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
	  if(!emptyBlock(blockCollX, posY1/32)
	     || !emptyBlock(blockCollX, posY2/32))
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

	  if(!emptyBlock(blockCollX, posY1/32)
	     || !emptyBlock(blockCollX, posY2/32))
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
	  if(!emptyBlock(blockCollX1, (posY1-y_to_move-1)/32)
	     || !emptyBlock(blockCollX2,(posY1-y_to_move-1)/32))
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
	  if(!emptyBlock(blockCollX1, (posY2+y_to_move+1)/32)
	     || !emptyBlock(blockCollX2, (posY2+y_to_move+1)/32))
	    {
	      *y = y_to_move;
 	      break;
	    }
	}
    }
}
