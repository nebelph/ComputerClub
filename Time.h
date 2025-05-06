#pragma once

#include <iostream>

class Time {
public:
  Time(std::string);
  Time(unsigned, unsigned);
  
  Time operator+(const Time&) const;
  Time operator-(const Time&) const;
  bool operator<(const Time&) const;
  
  friend std::ostream& operator<<(std::ostream&, const Time&);

  unsigned round();
private:
  unsigned m_hours;
  unsigned m_minutes;
};

