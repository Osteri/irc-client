#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>

#include <boost/program_options/variables_map.hpp>

class Config
{
public:
  Config() = delete;                    // def ctor
  Config(int ac, char** av);            // parse options from cli
  Config(const std::string& filename);  // parse options from config file

  void Save(const std::string& filename); // save existing state to config file

  friend std::ostream& operator<<(std::ostream& os, const Config& conf); // print
  friend std::ostream& operator<<(std::fstream& os, const Config& conf); // save file

  /* Actual information this class holds. */
  std::string server, port, nick;
  std::vector<std::string> channels;

private:
  auto CreateOptionsDesc(const std::string& name); // reduce boiler plate code
  void AssignValues(const boost::program_options::variables_map& vm);
};

#endif // CONFIG_H
