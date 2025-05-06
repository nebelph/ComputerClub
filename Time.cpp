#include "Time.h"

Time::Time(std::string time) :
  m_hours((time[0] - 48) * 10 + (time[1] - 48)),
  m_minutes((time[3] - 48) * 10 + (time[4] - 48)) {}

Time::Time(unsigned hours, unsigned minutes) :
  m_hours(hours),
  m_minutes(minutes) {}

Time Time::operator+(const Time& other) const {
  unsigned hours = m_hours + other.m_hours;
  unsigned minutes = m_minutes + other.m_minutes;
  if (minutes >= 60) {
    hours++;
    minutes -= 60;
  }
  return Time(hours, minutes);
}

Time Time::operator-(const Time& other) const {
  int hours = m_hours - other.m_hours;
  int minutes = m_minutes - other.m_minutes;
  //если клуб работает до утра, то обработать if this < other
  if (minutes < 0) {
    hours--;
    minutes += 60;
  }
  return Time(hours, minutes);
}

bool Time::operator<(const Time& other) const {
  if (m_hours < other.m_hours)
    return true;
  else if (m_hours == other.m_hours) {
    return (m_minutes < other.m_minutes);
  }
  else
    return false;
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
  time.m_hours >= 10 ? (os << time.m_hours << ":") : (os << "0" << time.m_hours << ":");
  time.m_minutes >= 10 ? (os << time.m_minutes) : (os << "0" << time.m_minutes);
  return os;
}

unsigned Time::round() {
  if (m_minutes == 0)
    return m_hours;
  return m_hours + 1;
}
