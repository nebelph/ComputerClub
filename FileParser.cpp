#include "FileParser.h"

FileParser::FileParser(std::string filename) :
  m_isCorrect(true) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Can't open file: " << filename << "\n";
    // должен быть обработчик
  }
  m_club = readHead(file);
  file.close();
}

ClubManager FileParser::getClub() const {
  return m_club;
}

ClubManager FileParser::readHead(std::ifstream& file) {
  std::string errstr;
  try {
    std::string tables;
    std::getline(file, tables);
    std::string time;
    std::getline(file, time);
    std::string open_time_str = "";
    for (int i = 0; i < 5; i++) {
      open_time_str += time[i];
    }
    if (!isTime(open_time_str)) {
      errstr = time;
      throw std::exception();
    }
    Time open_time(open_time_str);
    std::string close_time_str = "";
    for (int i = 6; i < 11; i++) {
      close_time_str += time[i];
    }
    if (!isTime(close_time_str)) {
      errstr = time;
      throw std::exception();
    }
    Time close_time(close_time_str);
    std::string hourly_pay;
    std::getline(file, hourly_pay);
    
    return ClubManager(std::stoi(tables), open_time, close_time, std::stoi(hourly_pay), readLines(file, std::stoi(tables)));
  } catch (std::exception& e) {
    std::cerr << errstr << std::endl;
    m_isCorrect = false;
    return ClubManager();
  }
}

std::vector<std::vector<std::string>> FileParser::readLines(std::ifstream& file, unsigned number_of_tables) {
  std::string errstr;
  std::vector<std::vector<std::string>> retval;
  try {
    std::string line;
    std::string subline = "";
    Time l_time("00:00");
    while (std::getline(file, line)) {
      std::vector<std::string> v;
      for (const auto& ch : line) {
        if (ch == ' ') {
          v.push_back(subline);
          if (v.size() == 1) {
            if (!(l_time < Time(v[0]))) {
              errstr = line;
              throw std::exception();
            }
          }
          if (v.size() == 3) {
            if (!isName(subline)) {
              errstr = line;
              throw std::exception();
            }
          }
          subline = "";
        }
        else {
          subline += ch;
        }
      }
      l_time = Time(v[0]);
      v.push_back(subline);
      //
      if (v.size() == 3) {
        if (!isName(subline)) {
          errstr = line;
          throw std::exception();
        }
      }
      if (v.size() == 4) {
        if ((std::stoi(v[3]) < 1 || (std::stoi(v[3]) > number_of_tables))) {
          errstr = line;
          throw std::exception();
        }
      }
      //
      subline = "";
      retval.push_back(v);
    }
  } catch (std::exception& e) {
    std::cerr << errstr << std::endl;
    m_isCorrect = false;
  }
  return retval;
}

bool FileParser::isTime(std::string& t) {
  if (t.size() != 5) return false;
  if (t.at(2) != ':') return false;
  if ((t[0] - 48) * 10 + (t[1] - 48) < 0 || (t[0] - 48) * 10 + (t[1] - 48) > 24) return false;
  if ((t[0] - 48) * 10 + (t[1] - 48) < 0 || (t[0] - 48) * 10 + (t[1] - 48) > 60) return false;
  return true;
}

bool FileParser::isName(std::string& n) {
  for (const auto& ch : n) {
    if (!(ch == 45 || ch == 95 || (ch >= 48 && ch <= 57) || (ch >= 97 && ch <= 122))) return false;
  }
  return true;
}
