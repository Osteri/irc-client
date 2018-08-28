#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>

class Config
{
public:
  Config(int ac, char **av);
  friend std::ostream& operator<<(std::ostream& os, const Config& conf);

  std::string server;
  uint16_t port;
  std::string nick;
  std::vector<std::string> channels;
};

#endif // CONFIG_H
