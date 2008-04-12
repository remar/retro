#ifndef BROKEN_BLOCK_H
#define BROKEN_BLOCK_H

class BrokenBlock
{
 public:
  BrokenBlock(int x, int y);
  void update();
  bool respawn();
  int getX();
  int getY();
  void resetRespawnTimer(int value);

 private:
  int x, y;
  int respawnTimer;
};

#endif
