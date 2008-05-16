#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>

/*

~/.RemarGames/Retrobattle/topscore
~/.RemarGames/Retrobattle/keyconfig

*/

FileManager::FileManager()
{

}

void
FileManager::writeTopScore(int score)
{
  std::ofstream *file = openFileForWriting("topscore");

  if(!file)
    return;

  *file << score << std::endl;

  delete file;
}

int
FileManager::readTopScore()
{
  int top = 0;

  std::ifstream *file = openFileForReading("topscore");

  if(file)
    {
      *file >> top;
    }

  delete file;

  return top;
}

char *
FileManager::getFullFilename(char *file)
{
  char *filename = new char[1024];

  if(getenv("HOME") != 0)
    {
      sprintf(filename, "%s/.RemarGames/Retrobattle/%s", getenv("HOME"), file);
    }
  else
    {
      sprintf(filename, "%s.dat", file);
    }

  return filename;
}

std::ofstream *
FileManager::openFileForWriting(char *file)
{
  char *filename = getFullFilename(file);

  std::ofstream *f = 0;
  f = new std::ofstream(filename);

  delete [] filename;

  return f;
}

std::ifstream *
FileManager::openFileForReading(char *file)
{
  char *filename = getFullFilename(file);

  std::ifstream *f;
  f = new std::ifstream(filename);

  delete [] filename;

  return f;
}
