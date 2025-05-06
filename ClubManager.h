#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#include "Time.h"

class ClubManager {
public:
  ClubManager();
  ClubManager(unsigned, Time, Time, unsigned, std::vector<std::vector<std::string>>);
  
  int exec();
private:
  unsigned m_number_of_tables;
  Time m_open_time;
  Time m_close_time;
  unsigned m_hourly_pay;
  std::vector<std::vector<std::string>> m_events;
  std::vector<std::vector<std::string>> m_tables;
  std::vector<std::vector<Time>> m_tables_time;
  std::vector<std::string> m_clients;
  std::queue<std::string> m_clients_queue;
  
  void handleEvents();
  void handleLastClients();
  void calculateRevenue();
};
