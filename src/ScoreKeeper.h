#ifndef SCORE_KEEPER_H
#define SCORE_KEEPER_H

class ScoreKeeper
{
 public:
  ScoreKeeper();

  enum EnemyType
    {
      Fuzz = 0,
      Drone,
      SpaceViper,
      BountyHunter,
      GoldDrone,
      DireSpaceViper,
      DarkBountyHunter
    };

  bool fastFuzzes();
  int redFuzzes();

  int numberOfEnemy(EnemyType type);
/*   int numberOfBlueFuzzes(); */
/*   int numberOfRedFuzzes(); */

  void setSkillLevel(int level);
  int  getSkillLevel();
  int getLevel();

 private:
  int skillLevel;
  int level; /* Level we're on */
  int lives;
  int score;
};

#endif
