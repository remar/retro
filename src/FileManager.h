#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <iostream>

class FileManager
{
 public:
  FileManager();
  void writeTopScore(int score);
  int readTopScore();

 private:
  std::ofstream *openFileForWriting(char *file);
  std::ifstream *openFileForReading(char *file);
  char *getFullFilename(char *file);
};

#endif
