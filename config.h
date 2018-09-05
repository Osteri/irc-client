#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Config
{
public:
  Config(int ac, char **av);              // parse cli options
  Config(const std::string& filename);    // parse config file
  void Save(const std::string& filename); // save config file from existing state

  friend std::ostream& operator<<(std::ostream& os, const Config& conf);

  std::string server;
  std::string port;
  std::string nick;
  std::vector<std::string> channels;

private:
  auto CreateOptionsDesc(const std::string& name);
  void AssignValues(const po::variables_map& vm,
                       std::shared_ptr<po::options_description> desc);
};

#endif // CONFIG_H
