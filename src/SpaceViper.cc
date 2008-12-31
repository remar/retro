#include "SpaceViper.h"

SpaceViper::SpaceViper(remar2d *gfx, SoundManager *sfx, list<Enemy *> *enemies)
  : Enemy(gfx, "snake head", sfx), enemies(enemies), hitPoints(6), dead(false),
    length(80), moved(0)
{
  /* This is just a meta object... */
  setAnimation("up");
  setVisible(false);

  setBoundingBox(19, 19, 6, 6);

  for(int i = 0;i < length;i++)
    {
      SpaceViperBody *b = new SpaceViperBody(gfx, sfx, i%10 == 0);
      body.push_back(b);
      enemies->push_back(b);
    }

  head = gfx->createSpriteInstance("snake head");
  gfx->setAnimation(head, "up");
  gfx->showSprite(head, true);

  updateAnimation();

  /* Not initialized yet */
  moveAbs(-32, -32);
  gfx->moveSpriteAbs(head, -32, -32);
}

SpaceViper::~SpaceViper()
{
  gfx->removeSpriteInstance(head);
}

void
SpaceViper::update(Field *field, Hero *hero)
{
  if(dead)
    {
      if(--destroyTimer == 0)
	destroyMe = true;

      return;
    }

  if(getX() == -32)
    {
      /* Randomize location */
      bool done = false;
      while(!done)
	{
	  int x = (rand()%21)*32 + 96;
	  int y = (rand()%14)*32 + 128;

	  if(field->emptyBlock(x/32, y/32))
	    {
	      moveAbs(x, y);
	      gfx->moveSpriteAbs(head, x, y);
	      list<SpaceViperBody *>::iterator it = body.begin();
	      for(;it != body.end();it++)
		{
		  (*it)->moveAbs(x, y);
		}
	      done = true;
	    }
	}

      /* Force new direction */
      moved = 32;

      return;
    }

  if(collides(hero))
    hero->die();

  if(moveThisFrame == false)
    {
      moveThisFrame = true;
      return;
    }

  if(moved == 32)
    {
      /* First, check all directions for possible ways, if none exist,
	 stop and coil up tail. */

      int surroundingBlocks = 0;
      int surroundingTail = 0;

      int blockX, blockY;

      for(int i = 1;i < 5;i++)
	{
	  switch((Direction)i)
	    {
	    case LEFT:
	      blockX = getX() / 32 - 1;
	      blockY = getY() / 32;
	      break;
	      
	    case RIGHT:
	      blockX = getX() / 32 + 1;
	      blockY = getY() / 32;
	      break;

	    case UP:
	      blockX = getX() / 32;
	      blockY = getY() / 32 - 1;
	      break;

	    case DOWN:
	      blockX = getX() / 32;
	      blockY = getY() / 32 + 1;
	      break;
	    }

	  if(!field->emptyBlock(blockX, blockY))
	    surroundingBlocks++;
	  if(tailAtPosition(blockX, blockY))
	    surroundingTail++;	 
	}

      if(surroundingBlocks == 4)
	{
	  /* We must DIG our way out! */
	}
      else if(surroundingBlocks + surroundingTail >= 4)
	{
	  /* Wait for tail to coil up */
	  moveDirection = NONE;
	}
      else
	{
	  bool done = false;

	  while(!done)
	    {
	      moveDirection = (Direction)(random()%4 + 1);

	      switch(moveDirection)
		{
		case LEFT:
		  blockX = getX() / 32 - 1;
		  blockY = getY() / 32;
		  break;
		  
		case RIGHT:
		  blockX = getX() / 32 + 1;
		  blockY = getY() / 32;
		  break;
		  
		case UP:
		  blockX = getX() / 32;
		  blockY = getY() / 32 - 1;
		  break;
		  
		case DOWN:
		  blockX = getX() / 32;
		  blockY = getY() / 32 + 1;
		  break;
		}
	      
	      if(field->emptyBlock(blockX, blockY)
		 && !tailAtPosition(blockX, blockY))
		done = true;
	    }

	  updateAnimation();
	  moved = 0;
	}
    }

  int move_x_table[] = {0, -2, 2, 0, 0};
  int move_y_table[] = {0, 0, 0, -2, 2};

  int move_x = move_x_table[moveDirection];
  int move_y = move_y_table[moveDirection];  

  moveRel(move_x, move_y);
  gfx->moveSpriteRel(head, move_x, move_y);

  int old_x, old_y;
  int new_x, new_y;

  new_x = getX();
  new_y = getY();
      
  list<SpaceViperBody *>::iterator it = body.begin();
  for(;it != body.end();it++)
    {
      old_x = (*it)->getX();
      old_y = (*it)->getY();
      
      (*it)->moveAbs(new_x, new_y);
      
      new_x = old_x;
      new_y = old_y;
    }

  if(moveDirection != NONE)
    moved+=2;

  if(getX() <= -32)
    {
      moveAbs(798, getY());
      gfx->moveSpriteAbs(head, 798, getY());
      moved = 2;
    }
  if(getX() >= 800)
    {
      moveAbs(-30, getY());
      gfx->moveSpriteAbs(head, -30, getY());
      moved = 2;
    }

  moveThisFrame = false;
}

bool
SpaceViper::hit()
{
  if(--hitPoints == 0)
    {
      dead = true;
      destroyTimer = 60;

      list<SpaceViperBody *>::iterator it = body.begin();
      for(;it != body.end();it++)
	(*it)->splat();
    }

  return true;
}

void
SpaceViper::updateAnimation()
{
  const char *dirStrings[] = {"up", "left", "right", "up", "down"};

  gfx->setAnimation(head, (char *)dirStrings[moveDirection]);
}

bool
SpaceViper::tailAtPosition(int blockX, int blockY)
{
  /* Wrap checks */
  if(blockX == -1)
    blockX = 24;
  if(blockX == 25)
    blockX = 0;

  list<SpaceViperBody *>::iterator it = body.begin();
  for(;it != body.end();it++)
    {
      if((*it)->getX() / 32 == blockX && (*it)->getY() / 32 == blockY)
	return true;
    }

  return false;
}
