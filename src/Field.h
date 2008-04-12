#ifndef FIELD_H
#define FIELD_H

struct Field
{
  enum BlockType
    {
      EMPTY = 0,
      SOLID = 1,
      BREAKABLE = 2,
      DAMAGED = 3,
      BROKEN = 4
    };

  BlockType field[25][19];

  int SPIKES_LEVEL;

  Field() : SPIKES_LEVEL(600 - 32 - 16) {}

  inline bool emptyBlock(int x, int y) //BlockType block)
  {
    /* Let checks wrap around */
    if(x == -1)
      x = 24;
    if(x == 25)
      x = 0;

    return field[x][y] == EMPTY || field[x][y] == BROKEN;
  }
};

#endif
