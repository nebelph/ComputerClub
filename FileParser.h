#pragma once

#include <fstream>
#include <iostream>
#include <string> // stoi()
#include <vector>

#include "Time.h"
#include "ClubManager.h"

class FileParser {
public:
  FileParser(std::string);
  
  bool m_isCorrect;
  
  ClubManager getClub() const;
  std::vector<std::vector<std::string>> getLines() const;
private:
  ClubManager m_club;
  std::vector<std::vector<std::string>> m_lines;
  
  ClubManager readHead(std::ifstream&);
  std::vector<std::vector<std::string>> readLines(std::ifstream&, unsigned);
  
  bool isTime(std::string&);
  bool isName(std::string&);
};
