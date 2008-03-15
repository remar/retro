#include "ScoreKeeper.h"

ScoreKeeper::ScoreKeeper()
  : skillLevel(1), level(1), score(0), lives(3)
{

}

bool
ScoreKeeper::fastFuzzes()
{
  return skillLevel > 1;
}

int
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
