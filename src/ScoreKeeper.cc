#include "ScoreKeeper.h"
#include <stdio.h>
#include "FileManager.h"

ScoreKeeper::ScoreKeeper()
  : skillLevel(1), level(1), bonusLevel(1), levelsCompleted(0), score(0),
    top(0), lives(3)
{
  resetKills();

  FileManager fileManager;

  top = fileManager.readTopScore();

  resetCoins();
}

ScoreKeeper::~ScoreKeeper()
{
  FileManager fileManager;

  fileManager.writeTopScore(top);
}

bool
ScoreKeeper::fastFuzzes()
{
  return skillLevel > 1;
}

bool
ScoreKeeper::redFuzzes()
{
  return skillLevel >= 5;
}

int
ScoreKeeper::numberOfEnemy(EnemyType type)
{
  int amounts[] = 

  /*  0  1  2  3  4  5  6  7  8 */

    { 0, 3, 4, 5, 6, 6, 6, 6, 6, /* Fuzz (Blue or Red) */
      0, 2, 3, 4, 4, 4, 0, 0, 0, /* Drone */
      0, 0, 4, 5, 6, 6, 6, 0, 0, /* Space Viper (length) */
      0, 0, 0, 1, 1, 1, 1, 1, 0, /* Bounty Hunter */
      /*0, 0, 0, 0, 0, 6, 6, 6, 6,*/ /* Red Fuzz */
      0, 0, 0, 0, 0, 0, 4, 4, 4, /* Gold Drone */
      0, 0, 0, 0, 0, 0, 0, 6, 6, /* Dire Space Viper (length) */
      0, 0, 0, 0, 0, 0, 0, 0, 1  /* Dark Bounty Hunter */
    };

  return amounts[type * 9 + skillLevel];
}

void
ScoreKeeper::setSkillLevel(int level)
{
  skillLevel = level;
}

int
ScoreKeeper::getSkillLevel()
{
  return skillLevel;
}

void
ScoreKeeper::incSkillLevel()
{
  if(skillLevel < 8)
    skillLevel++;
}

int
ScoreKeeper::getLevel()
{
  return level;
}

void
ScoreKeeper::setLevel(int lev)
{
  level = lev;
}

void
ScoreKeeper::nextLevel()
{
  level++;
  if(level > 32)
    level = 1;
}

bool
ScoreKeeper::nextLevelIsBonus()
{
  if(levelsCompleted == 4)
    {
      levelsCompleted = 0;
      return true;
    }
  return false;
}
			  
bool
ScoreKeeper::blocksTakeTwoHits()
{
  return skillLevel >= 4;
}

void
ScoreKeeper::resetKills()
{
  for(int i = 0;i < 7;i++)
    kills[i] = 0;
}

void
ScoreKeeper::killed(EnemyType type)
{
  kills[type]++;
}

int
ScoreKeeper::howManyKilled(EnemyType type)
{
  return kills[type];
}

void
ScoreKeeper::heroKilled()
{
  if(lives)
    lives--;
}

int
ScoreKeeper::getLives()
{
  return lives;
}

void
ScoreKeeper::setLives(int l)
{
  lives = l;
}

void
ScoreKeeper::addScore(int s)
{
  setScore(score + s);
}

void
ScoreKeeper::setScore(int s)
{
  score = s;

  if(score > top)
    top = score;
}

int
ScoreKeeper::getScore()
{
  return score;
}

void
ScoreKeeper::setTopScore(int s)
{
  top = s;
}

int
ScoreKeeper::getTopScore()
{
  return top;
}

void
ScoreKeeper::calculateScore()
{
  int scores[] = {120, 200, 1200, 1000, 200, 1200, 1000};

  for(int i = 0;i < 7;i++)
    {
      score += kills[i] * scores[i];
    }

  score += 100*skillLevel;
}
