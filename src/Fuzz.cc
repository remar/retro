#include "Fuzz.h"

Fuzz::Fuzz(remar2d *gfx, SoundManager *sfx, ScoreKeeper *scoreKeeper)
  : Enemy(gfx, "fuzz", sfx, scoreKeeper), moveDirection(NONE),
    rollDirection(NONE), falling(true), stunned(false), fastFuzz(false),
    FALL_LIMIT(60), dead(false), onSpikes(false)
{
  setAnimation("roll left");
  pauseAnimation(true);
  attachNone();

  setBoundingBox(24, 24, 0, 0);
}

void
Fuzz::update(Field *field, Hero *hero)
{
/*
  Move-A-Fuzz

  0. If we're falling, check for collision with solid blocks and spikes
  1. Check to see that we're still attached to something (no broken block)
  2. See how many pixels we will move
   2a. If less than required to reach a boundary between blocks, just move
   2b. If this will take us over a boundary, check if we'll hit a block
       or roll outside a block with this move
  3. Check for collision with Captain Good
   3a. If we're stunned, push fuzz 1 pixel in some direction
   3b. If we're not stunned, kill Captain Good
*/
  SPIKES_LEVEL = field->SPIKES_LEVEL;

  if(dead)
    {
      /* update death timer (allow death animation to play before
	 cleaning up) */
      if(--deathTimer == 0)
	{
	  destroyMe = true;
	}

      return;
    }

  char *moveDirChar[5] = {"NONE", "LEFT", "RIGHT", "UP", "DOWN"};

  int posX = getX();
  int posY = getY();
  SDL_Rect *box = getBoundingBox();
  int posX1 = posX + box->x;
  int posX2 = posX + box->x + box->w;
  int posY1 = posY + box->y;
  int posY2 = posY + box->y + box->h;
  
  /* Very simple move, just move in roll direction until we hit
     something */
  if(onSpikes)
    {
      int move_x;
      switch(rollDirection)
	{
	case LEFT:
	  move_x = -1;
	  if(field->emptyBlock((posX1+move_x)/32, posY1/32)
	     && field->emptyBlock((posX1+move_x)/32, posY2/32))
	    {
	      moveRel(move_x, 0);
	      moveDirection = LEFT;
	    }
	  else
	    {
	      attach(posX1+move_x, posX2+move_x,
		     posY1, posY2, LEFT);
	      moveDirection = UP;
	      onSpikes = false;
	      moveRel(0, -1);
	    }
	  break;

	case RIGHT:
	  move_x = 1;
	  if(field->emptyBlock((posX2+move_x)/32, posY1/32)
	     && field->emptyBlock((posX2+move_x)/32, posY2/32))
	    {
	      moveRel(move_x, 0);
	      moveDirection = RIGHT;
	    }
	  else
	    {
	      attach(posX1+move_x, posX2+move_x,
		     posY1, posY2, RIGHT);
	      moveDirection = UP;
	      onSpikes = false;
	      moveRel(0, -1);
	    }
	  break;
	default:
	  break;
	}
      return;
    }

  /* Change direction if we hit the spikes */
  if(!falling && !stunned && posY2 >= SPIKES_LEVEL)
    {
      if(rollDirection == RIGHT)
	{
	  rollLeft();
	  moveDirection = UP;
	}
      else
	{
	  rollRight();
	  moveDirection = UP;
	}
    }

  enum { X1, X2, Y1, Y2 };
  int positions[4];
  positions[X1] = posX1;
  positions[X2] = posX2;
  positions[Y1] = posY1;
  positions[Y2] = posY2;

  
  Direction mDir = moveDirection;

  if(falling)
    {
      for(int i = 0;i < 2;i++)
	{

	  /* See if we collide with anything, then start rolling in the
	     rollDirection, else just keep falling */
	  if(!field->emptyBlock(posX1/32, (posY2 + 1)/32)
	     || !field->emptyBlock(posX2/32, (posY2 + 1)/32)
	     || posY2 == SPIKES_LEVEL)
	    {
	      /* Collision! */
	      // printf("COLLISION\n");
	      
	      falling = false;

	      if(stunned)
		{
		  /* If fuzz is stunned and has either fallen far enough
		     or hits spikes, kill fuzz */
		  // printf("posY2: %d, SPIKES_LEVEL: %d\n", posY2, SPIKES_LEVEL);
		  if(fallDistance > FALL_LIMIT
		     || posY2 >= SPIKES_LEVEL)
		    {
		      die();
		      return;
		    }
		}
	      else
		{
		  if(rollDirection == LEFT)
		    {
		      setMoveDir(LEFT);
		      rollLeft();
		    }
		  else
		    {
		      setMoveDir(RIGHT);
		      rollRight();
		    }
		  pauseAnimation(false);
		}
	      
	      if(posY2 >= SPIKES_LEVEL)
		{
		  attachNone();
		  
		  /* Special Roll-On-Spikes mode */
		  onSpikes = true;
		}
	      else
		{
		  /* Now we're attached to this/these block(s) */
		  attach(posX1, posX2, posY1, posY2, DOWN);
		}

	      break;
	    }
	  else
	    {
	      fallDistance++;
	      moveRel(0, 1);

	      posY1++; posY2++;
	      
	      attachNone();
	    }
	}
    }
  else if(!stunned) // not falling and not stunned (rolling on ground...)
    {
      int times = fastFuzz ? 2 : 1;
      int move_x = 0, move_y = 0;

      // enum MoveDir {NONE, LEFT, RIGHT, UP, DOWN};

      int move_x_table[] = {0, -1, 1, 0, 0};
      int move_y_table[] = {0, 0, 0, -1, 1};
      Direction attach_table_left[] = {NONE, DOWN, UP, LEFT, RIGHT};
      Direction attach_table_right[] = {NONE, UP, DOWN, RIGHT, LEFT};

      /* What should move_x, move_y be if we collide while rolling left? */
      int move_x_collision_left[] = {0, 0, 0, 1, -1};
      int move_y_collision_left[] = {0, -1, 1, 0, 0};

      /* What should move_x, move_y be if we collide while rolling right? */
      int move_x_collision_right[] = {0, 0, 0, -1, 1};
      int move_y_collision_right[] = {0, 1, -1, 0, 0};

      /* move_x, move_y if we roll over an edge while rolling left */
      int move_x_empty_left[] = {0, -1,  1, -1,  1};
      int move_y_empty_left[] = {0,  1, -1, -1,  1};

      /* move_x, move_y if we roll over an edge while rolling right */
      int move_x_empty_right[] = {0, -1,  1,  1, -1};
      int move_y_empty_right[] = {0, -1,  1, -1,  1};

      /* Use this looked-up variable to check for collision (left) */
      int collision_dir[] = {0, 0, 1, 2, 3};
      int collision_dir_lookup[5];

      /* Use this looked-up variable to check for an empty block */
      int empty_dir[] = {0, 1, 0, 3, 2};
      int empty_dir_lookup[5];

      int nearest_x[] = {0, 0, 1, 0, 0};
      int nearest_y[] = {0, 2, 2, 2, 3};

      // enum MoveDir {NONE, LEFT, RIGHT, UP, DOWN};

      int empty_check_x_left[] = {0, X2, X1, X1, X2};
      int empty_check_y_left[] = {0, Y2, Y1, Y2, Y1};

      int empty_check_x_right[] = {0, X2, X1, X2, X1};
      int empty_check_y_right[] = {0, Y1, Y2, Y2, Y1};

      int xinc_table_left[] = {0, 0,  0, -1, 1};
      int yinc_table_left[] = {0, 1, -1,  0, 0};

      int xinc_table_right[] = {0,  0, 0, 1, -1};
      int yinc_table_right[] = {0, -1, 1, 0,  0};
      
      /* New direction to travel in when colliding/find empty block
	 (rolling left) */
      Direction new_direction_collision_left[] = {NONE, UP, DOWN, RIGHT, LEFT};
      Direction new_direction_empty_left[] = {NONE, DOWN, UP, LEFT, RIGHT};

      /* New direction to travel in when colliding/find empty block
	 (rolling right) */
      Direction new_direction_collision_right[]
	= {NONE, DOWN, UP, LEFT, RIGHT};
      Direction new_direction_empty_right[] = {NONE, UP, DOWN, RIGHT, LEFT};

      Direction attachDir = NONE;
      
      for(int i = 0;i < times;i++)
	{
	  posX = getX();
	  posY = getY();
	  box = getBoundingBox();
	  posX1 = posX + box->x;
	  posX2 = posX + box->x + box->w;
	  posY1 = posY + box->y;
	  posY2 = posY + box->y + box->h;

	  positions[X1] = posX1;
	  positions[X2] = posX2;
	  positions[Y1] = posY1;
	  positions[Y2] = posY2;

	  mDir = moveDirection;
  
	  for(int i = 1;i < 5;i++)
	    collision_dir_lookup[i] = positions[collision_dir[i]];

	  for(int i = 1;i < 5;i++)
	    empty_dir_lookup[i] = positions[empty_dir[i]];

	  move_x = move_x_table[mDir];
	  move_y = move_y_table[mDir];

	  switch(rollDirection)
	    {
	    case LEFT:
// 	      printf("ROLLING LEFT\n");

	      attachDir = attach_table_left[mDir];
	      
	      // Check if we collide in the direction we're travelling
	      if(collision_dir_lookup[mDir]/32
		 != (collision_dir_lookup[mDir]
		     + (collision_dir[mDir] <= 1
			? move_x
			: move_y))/32)
		{
		  // possible collision with a block
		  if(!field->emptyBlock((positions[nearest_x[mDir]]
					 +move_x)/32,
					(positions[nearest_y[mDir]]
					 +move_y)/32))
		    {
		      move_x = move_x_collision_left[mDir];
		      move_y = move_y_collision_left[mDir];
		      moveDirection = new_direction_collision_left[mDir];
		      attachDir = attach_table_left[moveDirection];
		    }
		}
	      // Check if there's no block to allow us to continue rolling
	      else if(empty_dir_lookup[mDir]/32
		      != (empty_dir_lookup[mDir]
			  + (empty_dir[mDir] <= 1
			     ? move_x
			     : move_y))/32)
		{
		  int xinc = xinc_table_left[mDir];
		  int yinc = yinc_table_left[mDir];

		  int x = positions[empty_check_x_left[mDir]];
		  int y = positions[empty_check_y_left[mDir]];
		      
		  if(field->emptyBlock((x+move_x+xinc)/32,
				       (y+move_y+yinc)/32))
		    {
 		      //printf("Approaching edge, turn!\n");
		      // no block underneath!
		      move_x = move_x_empty_left[mDir];
		      move_y = move_y_empty_left[mDir];
		      
		      moveDirection = new_direction_empty_left[mDir];
		      attachDir = attach_table_left[moveDirection];
		    }
		}
	      
 	      //printf("Real move %d, %d\n", move_x, move_y);
	      moveRel(move_x, move_y);
	      attach(posX1+move_x, posX2+move_x,
		     posY1+move_y, posY2+move_y,
		     attachDir);

	      if(getX() < -25)
		moveAbs(801, getY());
	      else if(getX() > 802)
		moveAbs(-24, getY());
	      
	      break;

	    case RIGHT:
// 	      printf("ROLLING RIGHT\n");

	      attachDir = attach_table_right[mDir];

	      // Check if we collide in the direction we're travelling
	      if(collision_dir_lookup[mDir]/32
		 != (collision_dir_lookup[mDir]
		     + (collision_dir[mDir] <= 1
			? move_x
			: move_y))/32)
		{
		  // possible collision with a block
		  if(!field->emptyBlock((positions[nearest_x[mDir]]
					 +move_x)/32,
					(positions[nearest_y[mDir]]
					 +move_y)/32))
		    {
		      move_x = move_x_collision_right[mDir];
		      move_y = move_y_collision_right[mDir];
		      moveDirection = new_direction_collision_right[mDir];
		      attachDir = attach_table_right[moveDirection];
		    }
		}
	      // Check if there's no block to allow us to continue rolling
	      else if(empty_dir_lookup[mDir]/32
		      != (empty_dir_lookup[mDir]
			  + (empty_dir[mDir] <= 1
			     ? move_x
			     : move_y))/32)
		{
		  int xinc = xinc_table_right[mDir];
		  int yinc = yinc_table_right[mDir];

		  int x = positions[empty_check_x_right[mDir]];
		  int y = positions[empty_check_y_right[mDir]];
		      
		  if(field->emptyBlock((x+move_x+xinc)/32,
				       (y+move_y+yinc)/32))
		    {
		      //printf("Approaching edge, turn!\n");
		      // no block underneath!
		      move_x = move_x_empty_right[mDir];
		      move_y = move_y_empty_right[mDir];
		      
		      moveDirection = new_direction_empty_right[mDir];
		      attachDir = attach_table_right[moveDirection];
		    }
		}

 	      //printf("Real move %d, %d\n", move_x, move_y);
	      moveRel(move_x, move_y);
	      attach(posX1+move_x, posX2+move_x,
		     posY1+move_y, posY2+move_y,
		     attachDir);

	      if(getX() < -25)
		moveAbs(801, getY());
	      else if(getX() > 802)
		moveAbs(-24, getY());

	      break;
	    }
	}

    }
  else // stunned
    {
      /* Count down "stunned" timer */
      stunTimer--;
      if(stunTimer == 60) // TODO: Correct time before starting to blink
	{
	  blink();
	}
      else if(stunTimer == 0)
	{
	  //rollRandom();
	  if(rollDirection == LEFT)
	    setAnimation("roll left");
	  else
	    setAnimation("roll right");

	  stunned = false;
	  sfx->playSound(10, false);
	}
    }


  /* Check for collision with Captain Good */
  if(collides(hero))
    {
      if(stunned)
	{
	  int move_x = 0;

	  /* Push Fuzz left or right */
	  if(hero->getX() + 8 < getX() + 12)
	    move_x = 1;
	  else
	    move_x = -1;

	  if((move_x == 1
	      && field->emptyBlock((posX2+move_x)/32, posY2/32)
	      && field->emptyBlock((posX2+move_x)/32, posY1/32))
	     || (move_x == -1
		 && field->emptyBlock((posX1+move_x)/32, posY1/32)
		 && field->emptyBlock((posX1+move_x)/32, posY2/32)))
	    {
	      moveRel(move_x, 0);
	      if(!falling)
		{
		  attach(posX1+move_x, posX2+move_x, posY1, posY2, DOWN);
		}
	    }
	}
      else
	hero->die();
    }


  if(!onSpikes
     && last1_x != -1 && last1_y != -1 && last2_x != -1 && last2_y != -1
     && field->emptyBlock(last1_x, last1_y)
     && field->emptyBlock(last2_x, last2_y))
    {
      // printf("LOST CONTACT\n");
      /* Last block we we're attached to has broken... begin falling down */
      falling = true;
      if(!stunned)
	pauseAnimation(true);
      fallDistance = 0;
    }

//   printf("Next move direction(%d): %s\n--------------------------------------\n", onSpikes,
// 	 moveDirChar[moveDirection]);
}

void
Fuzz::rollLeft()
{
  if(rollDirection != LEFT)
    {
      setAnimation("roll left");
      rollDirection = LEFT;
      moveDirection = LEFT;
      stunned = false;
    }
}

void
Fuzz::rollRight()
{
  if(rollDirection != RIGHT)
    {
      setAnimation("roll right");
      rollDirection = RIGHT;
      moveDirection = RIGHT;
      stunned = false;
    }
}

void
Fuzz::rollRandom()
{
  if(rand()%2)
    rollRight();
  else
    rollLeft();
}

void
Fuzz::stun()
{
  // printf("STUN!\n");

  if(rollDirection != NONE)
    {
      // TODO: Correct time to be stunned 
      stunTimer = 5*60; /* 5 seconds */

      sfx->playSound(9, false);
      setAnimation("stunned");
      pauseAnimation(false);
      stunned = true;
      // rollDirection = NONE;
      blinkTime = SDL_GetTicks() + 2000;
      if(lastAttachDirection != DOWN)
	{
	  falling = true;
	  fallDistance = 0;
	}
    }
}

void
Fuzz::blink()
{
  setAnimation("blink");
}

void
Fuzz::die()
{
  if(dead)
    return;

  scoreKeeper->killed(ScoreKeeper::Fuzz);

  dead = true;
  sfx->playSound(8, false);
  setAnimation("splat");
  deathTimer = 60;
}

bool
Fuzz::isStunned()
{
  return stunned;
}

bool
Fuzz::hit()
{
  if(!stunned)
    {
      stun();
      return true;
    }

  return false;
}

void
Fuzz::attach(int posX1, int posX2, int posY1, int posY2, Direction direction)
{
  lastAttachDirection = direction;

  switch(direction)
    {
    case DOWN:
      last1_x = posX1/32; last1_y = (posY2 + 1)/32;
      last2_x = posX2/32; last2_y = (posY2 + 1)/32;
      break;

    case UP:
      last1_x = posX1/32; last1_y = (posY1 - 1)/32;
      last2_x = posX2/32; last2_y = (posY1 - 1)/32;
      break;

    case LEFT:
      last1_x = (posX1 - 1)/32; last1_y = posY1/32;
      last2_x = (posX1 - 1)/32; last2_y = posY2/32;
      break;

    case RIGHT:
      last1_x = (posX2 + 1)/32; last1_y = posY1/32;
      last2_x = (posX2 + 1)/32; last2_y = posY2/32;
      break;
    }

//   gfx->setTile(last1_x, last1_y,
// 	       "dots", 1, 0);
//   gfx->setTile(last2_x, last2_y,
// 	       "dots", 2, 0);
}

void
Fuzz::attachNone()
{
  last1_x = last1_y = last2_x = last2_y = -1;

  lastAttachDirection = NONE;
}

void
Fuzz::isFast()
{
  fastFuzz = true;
}
