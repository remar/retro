/*
 *    Source from Retrobattle, a NES-like collect-em-up.
 *    Copyright (C) 2010 Andreas Remar
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    Original authors contact info: andreas.remar@gmail.com
 */

#include "Cats.h"
#include "Menu.h"
#include "FileManager.h"

#define MOVE_ARROWS_TIMEOUT 30

Menu::Menu(SoundManager *sfx, Input *input,
	   ScoreKeeper *scoreKeeper)
  : GameMode(sfx, input, scoreKeeper), level(1), nextTimer(0),
    subMode(NORMAL), moveArrowsTimer(MOVE_ARROWS_TIMEOUT), arrowOffset(2)
{
  Cats::SetupTileLayer(50, 38, 16, 16);

  scoreKeeper->setBonusLevel(1);

  level = scoreKeeper->getLevel();
  skill = scoreKeeper->getSkillLevel();

  drawBackground();

  // remar_games_2012 = gfx->print("text", "remar games 2012");
  // gfx->showSprite(remar_games_2012, true);
  // gfx->moveSpriteAbs(remar_games_2012, 144, 208);

  // enter_to_start = gfx->print("text", "press enter to start");
  // gfx->showSprite(enter_to_start, true);
  // gfx->moveSpriteAbs(enter_to_start, 240, 304-32);

  // space_to_set_keys = gfx->print("text", "press space to set keys");
  // gfx->showSprite(space_to_set_keys, true);
  // gfx->moveSpriteAbs(space_to_set_keys, 240-24, 304);

  // stage = gfx->print("text", "stage");
  // gfx->showSprite(stage, true);
  // gfx->moveSpriteAbs(stage, 336, 336);

  levelCounter = new Counter(2);
  levelCounter->setPosition(432, 336);
  levelCounter->setCounter(level);

  // skillLevel = gfx->print("text", "skill");
  // gfx->showSprite(skillLevel, true);
  // gfx->moveSpriteAbs(skillLevel, 336, 336+32);

  skillCounter = new Counter(1);
  skillCounter->setPosition(432+16, 336+32);
  skillCounter->setCounter(skill);

  char buf[32];
  // sprintf(buf, "score %8.8d", scoreKeeper->getScore());

  // score = gfx->print("text", buf);
  // gfx->showSprite(score, true);
  // gfx->moveSpriteAbs(score, 16*18, 16*27 + 4);

  sprintf(buf, " top  %8.8d", scoreKeeper->getTopScore());

  // topScore = gfx->print("text", buf);
  // gfx->showSprite(topScore, true);
  // gfx->moveSpriteAbs(topScore, 16*18, 16*27+32  - 4);

  const char *aStrings[] = {"left", "right", "fire", "jump"};
  for(int i = 0;i < 4;i++)
    {
      actionStrings[i] = Cats::CreateText("text", aStrings[i]);
      Cats::ShowText(actionStrings[i], false);
      Cats::SetTextPosition(actionStrings[i], 16*3, 16*25 + 32 * i);
    }

  leftArrow = Cats::CreateSpriteInstance("arrow");
  Cats::SetAnimation(leftArrow, "left");
  Cats::SetSpritePosition(leftArrow, 336 - 14*2, 336 + 1);

  rightArrow = Cats::CreateSpriteInstance("arrow");
  Cats::SetAnimation(rightArrow, "right");
  Cats::SetSpritePosition(rightArrow, 336 + 7*16 + 14*2, 336 + 1);

  downArrow = Cats::CreateSpriteInstance("arrow");
  Cats::SetAnimation(downArrow, "down");
  Cats::SetSpritePosition(downArrow, 336 + 7*16 + 14*2, 336 + 32);
}

Menu::~Menu()
{
  // gfx->removeSpriteInstance(remar_games_2012);
  // gfx->removeSpriteInstance(enter_to_start);
  // gfx->removeSpriteInstance(stage);
  // gfx->removeSpriteInstance(space_to_set_keys);
  // gfx->removeSpriteInstance(skillLevel);
  Cats::RemoveSpriteInstance(leftArrow);
  Cats::RemoveSpriteInstance(rightArrow);
  Cats::RemoveSpriteInstance(downArrow);
  delete levelCounter;
  delete skillCounter;
}

Mode
Menu::update()
{
  if(nextTimer > 0)
    {
      --nextTimer;
      if(nextTimer == 0)
	{
	  return GAME;
	}
    }

  Input::ACTION submodeToAction[] = {(Input::ACTION)0, // undefined,
				     Input::LEFT,
				     Input::RIGHT,
				     Input::FIRE,
				     Input::JUMP};

  int lockedKeys[] = {SDLK_ESCAPE, // Used for quitting
		      SDLK_f,      // Fullscreen
		      SDLK_p,      // Pause
		      0};

  moveArrows();

  if(subMode != NORMAL)
    {
      if(--blinkDelay == 0)
	{
	  blinkDelay = 60;
	}

      if(blinkDelay <= 30)
	Cats::ShowText(actionStrings[(int)subMode - 1], false);
      else
	Cats::ShowText(actionStrings[(int)subMode - 1], true);

      if(int i = input->getKeyPressed())
	{
	  if(i == SDLK_SPACE) {
	    return MENU;
	  }

	  for(int j = 0;lockedKeys[j];j++)
	    {
	      if(i == lockedKeys[j])
		{
		  printf("Can't use that key, chose another\n");
		  return MENU;
		}
	    }

	  if(!input->defineActionKey(submodeToAction[subMode], i))
	    {
	      printf("Key already in use, chose another\n");
	      return MENU;
	    }

	  keyConfig[(int)subMode - 1] = i;

	  Cats::ShowText(actionStrings[(int)subMode - 1], true);
	  subMode = (SUBMODE)((int)subMode + 1);

	  if(subMode > REDEFINE_JUMP)
	    {
	      subMode = NORMAL;

	      for(int k = 0;k < 4;k++)
		{
		  Cats::ShowText(actionStrings[k], false);
		}

	      FileManager fileManager;
	      fileManager.writeKeyConfig(keyConfig);
	    }
	}

      return MENU;
    }

  if(input->pressed(SDLK_RETURN))
    {
      scoreKeeper->reset();

      scoreKeeper->setLevel(level);
      scoreKeeper->setSkillLevel(skill);

      nextTimer = 60;
      sfx->playSound(14);
    }
  else if(input->pressed(SDLK_RIGHT))
    {
      increaseLevel();
    }
  else if(input->pressed(SDLK_LEFT))
    {
      decreaseLevel();
    }
  else if(input->pressed(SDLK_DOWN))
    {
      increaseSkillLevel();
    }
  else if(input->pressed(SDLK_SPACE))
    {
      /* Key redefinition mode */
      subMode = REDEFINE_LEFT;

      // Swallow first keypress... it returns 12 (SDLK_CLEAR) after
      // SPACE is pressed
      input->getKeyPressed();

      input->resetActionKeys();

      for(int i = 0;i < 4;i++)
	Cats::ShowText(actionStrings[i], true);

      blinkDelay = 60;
    }
  else if(input->pressed(SDLK_ESCAPE))
    return QUIT;

  return MENU;
}

void
Menu::moveArrows()
{
  if(subMode == NORMAL)
    {
      // Move arrows
      moveArrowsTimer--;
      if(moveArrowsTimer <= 0)
	{
	  Cats::MoveSprite(leftArrow, -arrowOffset, 0);
	  Cats::MoveSprite(rightArrow, arrowOffset, 0);
	  Cats::MoveSprite(downArrow, 0, arrowOffset);

	  moveArrowsTimer = MOVE_ARROWS_TIMEOUT;
	  arrowOffset = -arrowOffset;
	}
    }
}

void
Menu::drawBackground()
{
  // Having a nice layout of the data is A and O... alpha and omega... uh, nice
#define A 10
#define B 11

  int battle[] =
    {
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6, 
      6, B, 1, 0, 2, B, B, B, 3, B, B, B, B, B, B, B, B, 3, B, B, B, B, B, 3, B, B, 3, B, B, 3, B, B, B, B, 6, 
      6, B, 6, 1, 8, 1, 0, 2, A, 5, B, 1, 0, 5, 1, 0, 2, 6, 4, 2, 1, 0, 2, A, 5, B, A, 5, B, 6, 1, 0, 2, B, 6, 
      6, B, 6, 7, 2, 6, 4, 8, 6, B, B, 6, B, B, 6, B, 6, 6, B, 6, 6, B, 6, 6, B, B, 6, B, B, 6, 6, 4, 8, B, 6, 
      6, B, 9, B, 9, 7, 0, 5, 7, 0, 5, 9, B, B, 7, 0, 8, 7, 0, 8, 7, 5, 9, 7, 0, 5, 7, 0, 5, 9, 7, 0, 5, B, 6, 
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6, 
      7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 
    };

  int score[] =
    {
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6,
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6,
      6, B, B, B, B, B, B, B, B, B, B, B, B, B, B, 6,
      7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8,
    };

  drawPattern(battle, 8, 4, 35, 8);

  drawPattern(score, 17, 26, 16, 5);
}

void
Menu::drawPattern(int *arr, int xOffset ,int yOffset, int width, int height)
{
  int i;

  for(int y = 0; y < height;y++)
    for(int x = 0;x < width;x++)
      {
	i = y * width + x;
	Cats::SetTile(x + xOffset, y + yOffset, "maintiles",
		      arr[i]%6, arr[i]/6);
      }
}

void
Menu::increaseLevel()
{
  level++;
  if(level > 32)
    level = 1;

  levelCounter->setCounter(level);
}

void
Menu::decreaseLevel()
{
  level--;
  if(level < 1)
    level = 32;

  levelCounter->setCounter(level);
}

void
Menu::increaseSkillLevel()
{
  skill++;
  if(skill > 8)
    skill = 1;

  skillCounter->setCounter(skill);

  // Starting bonuslevel depends on starting skill level
  scoreKeeper->setBonusLevel(skill);
}
