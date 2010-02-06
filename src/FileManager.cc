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

void
FileManager::writeKeyConfig(int *config)
{
  std::ofstream *file = openFileForWriting("keyconfig");

  if(!file)
    return;

  *file << config[0] << std::endl
	<< config[1] << std::endl
	<< config[2] << std::endl
	<< config[3] << std::endl;

  delete file;
}

void
FileManager::readKeyConfig(int *config)
{
  std::ifstream *file = openFileForReading("keyconfig");

  if(file)
    {
      *file >> config[0];
      *file >> config[1];
      *file >> config[2];
      *file >> config[3];
    }

  delete file;
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
