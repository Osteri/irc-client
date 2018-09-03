#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>

class Config
{
public:
  Config(int ac, char **av);
  Config(const std::string& filename);
  void Save(const std::string& filename);
  void Load(const std::string& filename);

  friend std::ostream& operator<<(std::ostream& os, const Config& conf);
  friend std::istream& operator>>(std::istream& is, Config& conf);

  std::string server;
  std::string port;
  std::string nick;
  std::vector<std::string> channels;
};

#endif // CONFIG_H
