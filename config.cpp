#include "config.h"

#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>

using namespace::boost::program_options;

/* Construct Config from cli arguments. */
Config::Config(int ac, char **av) : port("6667") { // default values
  /* Declare supported cli options. */
  options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("server,s", value<std::string>(), "server domain or ip")
      ("port,p", value<std::string>(), "port")
      ("nick,n", value<std::string>(), "your nickname")
      ("channels,c", value<std::vector<std::string>>()->multitoken(), "channels to join")
      ;

  variables_map vm;
  store(parse_command_line(ac, av, desc), vm);
  notify(vm);

  if (vm.count("help")) {
    std::cout << desc << '\n';
    exit(1);
  }
  if (vm.count("server")) {
    server = vm["server"].as<std::string>();
  }
  if (vm.count("port")) {
    port = vm["port"].as<std::string>();
  }
  if (vm.count("nick")) {
    nick = vm["nick"].as<std::string>();
  }
  if (vm.count("channels")) {
    channels = vm["channels"].as<std::vector<std::string>>();
  }

  std::cout << "Configuration: " << *this << '\n';
}

/* Construct Config from file. */
Config::Config(const std::string& filename) {
  this->Load(filename);
}

/* Save Config to a file. */
void Config::Save(const std::string& filename) {
  std::fstream fs(filename, std::ios_base::out);
  if(fs)
    fs << *this;
  else
    std::cerr << "ERROR: saving file \"" << filename << "\" failed." << '\n';
  fs.close();
}

/* Load Config from file. */
void Config::Load(const std::string& filename) {
  std::fstream fs(filename, std::ios_base::in);
  if(fs)
    fs >> *this;
  else
    std::cerr << "ERROR: loading file \"" << filename << "\" failed." << '\n';
  fs.close();
}

/* Ability to istream Config, for example: Config >> var1 >> var2; */
std::istream& operator>>(std::istream& is, Config& conf) {
  std::string c;
  is >> conf.server >> conf.port >> conf.nick;
  while(!is.eof()) {
    is >> c;
    conf.channels.push_back(c);
  }
  return is;
}

/* Ability to ostream Config, for example: std::cout << Config << '\n'; */
std::ostream& operator<<(std::ostream& os, const Config& conf) {
  os << conf.server << ' ' << conf.port << ' ' << conf.nick << ' ';
  for(const auto& c : conf.channels)
    os << c << ' ';
  return os;
}
