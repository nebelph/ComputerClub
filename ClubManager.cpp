#include "ClubManager.h"

ClubManager::ClubManager() :
  m_number_of_tables(0),
  m_open_time(Time(0, 0)),
  m_close_time(Time(0, 0)),
  m_hourly_pay(0) {
  for (unsigned i = 0; i <= m_number_of_tables; i++) {
    m_tables.push_back({std::to_string(i)});
    m_tables_time.push_back({Time("00:00")});
  }  
}

ClubManager::ClubManager(unsigned number_of_tables, Time open_time, Time close_time, unsigned hourly_pay, std::vector<std::vector<std::string>> events) :
  m_number_of_tables(number_of_tables),
  m_open_time(open_time),
  m_close_time(close_time),
  m_hourly_pay(hourly_pay),
  m_events(events) {
  for (unsigned i = 0; i <= m_number_of_tables; i++) {
    m_tables.push_back({std::to_string(i)});
    m_tables_time.push_back({Time("00:00")});
  }
}

int ClubManager::exec() {
  std::cout << m_open_time << std::endl;
  handleEvents();
  handleLastClients();
  std::cout << m_close_time << std::endl;
  calculateRevenue();
  return 0;
}

void ClubManager::handleEvents() {
  for (const auto& event : m_events) {
    int rv = 0;
    for (const auto& i : event) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // 1
    if (event[1] == "1") {
    // 13 YouShallNotPass
      for (const auto& client : m_clients) {
        if (event[2] == client) {
          std::cerr << event[0] << " 13 " << "YouShallNotPass\n";
          rv = 1;
          break;
        }
      }
      if (rv != 0) continue;
      
    // 13 NotOpenYet
      if (Time(event[0]) < m_open_time) {
        std::cerr << event[0] << " 13 " << "NotOpenYet\n";
        rv = 1;
      }
      if (rv != 0) continue;
      
      m_clients.push_back(event[2]);
    }
    
    // 2
    else if (event[1] == "2") {
    // 13 PlaceIsBusy
      if (m_tables[std::stoi(event[3])].size() == 2) {
        std::cerr << event[0] << " 13 " << "PlaceIsBusy\n";
        rv = 1;
      }
      if (rv != 0) continue;
      
    // 13 ClientUnknown
      for (const auto& client : m_clients) {
        if (event[2] == client) {
          rv = 0;
          break;
        }
        rv = 1;
      }
      if (rv != 0) {
        std::cerr << event[0] << " 13 " << "ClientUnknown\n";
        continue;
      }
      
    // сидит за столом, но хочет его сменить
      for (auto& table : m_tables) {
        if (table.size() == 2) {
          if (table[1] == event[2]) {
            table.pop_back();
            // внести запись об окончании
            Time back = m_tables_time[std::stoi(table[0])].back();
            m_tables_time[std::stoi(table[0])].pop_back();
            m_tables_time[std::stoi(table[0])].push_back(Time(event[0]) - back);
          }
        }
      }
      
      m_tables[std::stoi(event[3])].push_back(event[2]);
      // внести запись о начале
      m_tables_time[std::stoi(event[3])].push_back(Time(event[0]));
    }
    
    // 3
    else if (event[1] == "3") {
    // 13 ICanWaitNoLonger!
      for (const auto& table : m_tables) {
        if (table[0] == "0") continue;
        if (table.size() == 1) {
          std::cerr << event[0] << " 13 " << "ICanWaitNoLonger!\n";
          rv = 1;
          break;
        }
      }
      if (rv != 0) continue;
      
    // 11 очередь клиентов больше, чем общее число столов, клиент уходит
      if (m_clients_queue.size() > m_number_of_tables) {
        std::cerr << event[0] << " 11 " << event[2] << std::endl;
      }
      else {
        m_clients_queue.push(event[2]);
      }
    }
    
    // 4
    else if (event[1] == "4") {
    // 13 ClientUnknown
      for (const auto& client : m_clients) {
        if (event[2] == client) {
          rv = 0;
          break;
        }
        rv = 1;
      }
      if (rv != 0) {
        std::cerr << event[0] << " 13 " << "ClientUnknown\n";
        continue;
      }
      
      for (auto& table : m_tables) {
        if (table.size() == 2) {
          if (table[1] == event[2]) {
            table.pop_back();
            // внести запись об окончании
            Time back = m_tables_time[std::stoi(table[0])].back();
            m_tables_time[std::stoi(table[0])].pop_back();
            m_tables_time[std::stoi(table[0])].push_back(Time(event[0]) - back);
    // 12 первый из очереди садится за стол
            if (!m_clients_queue.empty()) {
              table.push_back(m_clients_queue.front());
              // внести запись о начале
              m_tables_time[std::stoi(table[0])].push_back(Time(event[0]));
              m_clients_queue.pop();
              std::cout << event[0] << " 12 " << table[1] << " " << table[0] << std::endl;
            }
          }
        }
      }
    }
  }
}

void ClubManager::handleLastClients() {
  std::vector<std::string> last_clients;
  for (auto& table : m_tables) {
    if (table.size() == 2) {
      last_clients.push_back(table[1]);
      // внести запись об окончании
      Time back = m_tables_time[std::stoi(table[0])].back();
      m_tables_time[std::stoi(table[0])].pop_back();
      m_tables_time[std::stoi(table[0])].push_back(m_close_time - back);
    }
  }
  std::sort(last_clients.begin(), last_clients.end());
  for (const auto& client : last_clients) {
    std::cout << m_close_time << " 11 " << client << std::endl;
  }
}

void ClubManager::calculateRevenue() {
  for (int i = 1; i <= m_number_of_tables; i++) {
    Time sum_time("00:00");
    unsigned sum_pay = 0;
    for (int j = 0; j < m_tables_time[i].size(); j++) {
      sum_time = sum_time + m_tables_time[i][j];
      sum_pay = sum_pay + m_tables_time[i][j].round() * m_hourly_pay;
    }
    std::cout << i << " " << sum_pay << " " << sum_time << std::endl;
  }
}
