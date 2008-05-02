#include "ScoreKeeper.h"
#include <stdio.h>

ScoreKeeper::ScoreKeeper()
  : skillLevel(1), level(1), score(0), top(0), lives(3)
{

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
  char *enemyChar[] = { "Fuzz",
			"Drone",
			"Space Viper",
			"Bounty Hunter",
			"Gold Drone",
			"Dire Space Viper",
			"Dark Bounty Hunter"};

  printf("Killed a %s\n", enemyChar[type]);

  kills[type]++;
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
ScoreKeeper::addScore(int s)
{
  score += s;
}

void
ScoreKeeper::setScore(int s)
{
  score = s;
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
