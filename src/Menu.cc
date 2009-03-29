#include "Menu.h"

Menu::Menu(remar2d *gfx, SoundManager *sfx, Input *input,
	   ScoreKeeper *scoreKeeper)
  : GameMode(gfx, sfx, input, scoreKeeper), level(1), nextTimer(0)
{
  gfx->setupTileBackground(16, 16);

  scoreKeeper->setBonusLevel(1);

  level = scoreKeeper->getLevel();
  skill = scoreKeeper->getSkillLevel();

  drawBackground();

  remar_games_2008 = gfx->print("text", "remar games 2008");
  gfx->showSprite(remar_games_2008, true);
  gfx->moveSpriteAbs(remar_games_2008, 144, 208);

  enter_to_start = gfx->print("text", "press enter to start");
  gfx->showSprite(enter_to_start, true);
  gfx->moveSpriteAbs(enter_to_start, 240, 304);

  stage = gfx->print("text", "stage");
  gfx->showSprite(stage, true);
  gfx->moveSpriteAbs(stage, 336, 336);

  levelCounter = new Counter(gfx, 2);
  levelCounter->setPosition(432, 336);
  levelCounter->setCounter(level);

  skillLevel = gfx->print("text", "skill");
  gfx->showSprite(skillLevel, true);
  gfx->moveSpriteAbs(skillLevel, 336, 336+32);

  skillCounter = new Counter(gfx, 1);
  skillCounter->setPosition(432+16, 336+32);
  skillCounter->setCounter(skill);

  char buf[32];
  sprintf(buf, "score %8.8d", scoreKeeper->getScore());

  score = gfx->print("text", buf);
  gfx->showSprite(score, true);
  gfx->moveSpriteAbs(score, 16*18, 16*27 + 4);

  sprintf(buf, " top  %8.8d", scoreKeeper->getTopScore());

  topScore = gfx->print("text", buf);
  gfx->showSprite(topScore, true);
  gfx->moveSpriteAbs(topScore, 16*18, 16*27+32  - 4);
}

Menu::~Menu()
{
  gfx->removeSpriteInstance(remar_games_2008);
  gfx->removeSpriteInstance(enter_to_start);
  gfx->removeSpriteInstance(stage);
  gfx->removeSpriteInstance(skillLevel);
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
	  scoreKeeper->setScore(0);
	  scoreKeeper->setLives(3);

	  return GAME;
	}
    }

  if(input->pressed(SDLK_RETURN))
    {
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
  else if(input->pressed(SDLK_ESCAPE))
    return QUIT;

  return MENU;
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
	gfx->setTile(x + xOffset, y + yOffset, "maintiles",
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
