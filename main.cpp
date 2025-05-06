#include <iostream>

#include "ClubManager.h"
#include "Time.h"
#include "FileParser.h"

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cerr << "Text file was not found!\n";
    return 1;
  }
  FileParser file(argv[1]);
  if (!file.m_isCorrect)
    return 1;
  return file.getClub().exec();
}
