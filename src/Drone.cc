#include "Drone.h"

Drone::Drone(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper)
  : Enemy(gfx, "drone", sfx, scoreKeeper), moved(0), moveThisUpdate(true),
    hitPoints(2), dead(false)
{
  setAnimation("normal");
  setVisible(true);
  moveAbs(-32, -32);

  setBoundingBox(24, 24, 0, 0);
}

void
Drone::update(Field *field, Hero *hero)
{
  if(dead)
    {
      if(--deathTimer == 0)
	{
	  destroyMe = true;
	}

      return;      
    }

  if(collides(hero))
    {
      hero->die();
    }

  if(!moveThisUpdate)
    {
      moveThisUpdate = true;
      return;
    }

  if(getX() == -32)
    {
      /* Randomize location */
      bool done = false;
      while(!done)
	{
	  int x = (rand()%21)*32 + 96 + 4;
	  int y = (rand()%14)*32 + 128 + 4;

	  if(field->emptyBlock(x/32, y/32))
	    {
	      moveAbs(x, y);
	      done = true;
	    }
	}

      /* Force recalibration of move direction, sir! */
      moved = 32;
    }

  int move_x_table[] = {0, -2, 2, 0, 0};
  int move_y_table[] = {0, 0, 0, -2, 2};

  if(moved == 4 && willDig)
    {
      int blockX, blockY;
      blockX = getX() / 32 + move_x_table[moveDirection]/2;
      blockY = getY() / 32 + move_y_table[moveDirection]/2;

      field->breakBlock(blockX, blockY);
      sfx->playSound(3);
    }
  else if(moved == 32)
    {
      // printf("CENTER %d, %d\n", getX(), getY());

      Direction newDirection;
      bool done = false;
      willDig = false;

      while(!done)
	{
	  newDirection = (Direction)(rand()%4 + 1);
	  bool dig = ((rand()%100) > 91) && !(hero->isBlinking());
	  int blockX, blockY;

	  switch(newDirection)
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

	  if(field->emptyBlock(blockX, blockY))
	    done = true;
	  else if((field->field[blockX][blockY] == Field::BREAKABLE
		   || field->field[blockX][blockY] == Field::DAMAGED)
		  && dig)
	    {
	      done = true;
	      willDig = true;
	    }
	}

      moveDirection = newDirection;
      moved = 0;
    }

  int move_x = move_x_table[moveDirection];
  int move_y = move_y_table[moveDirection];

  moveRel(move_x, move_y);
  moved+=2;

  if(getX() <= -25)
    {
      moveAbs(800, getY());
      moved = 4;
    }
  if(getX() >= 801)
    {
      moveAbs(-24, getY());
      moved = 4;
    }

  moveThisUpdate = false;
}

bool
Drone::hit()
{
  if(dead)
    return false;

  if(hitPoints > 0)
    {
      hitPoints--;
      if(hitPoints == 0)
	{
	  die();
	}
    }
  return true;
}

void
Drone::die()
{
  scoreKeeper->killed(ScoreKeeper::Drone);

  setAnimation("crash");
  deathTimer = 60;

  dead = true;
}
