#include "config.h"

#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>

using namespace::boost::program_options;

Config::Config(int ac, char **av) : port(6667) { // default values
  /* Declare supported cli options. */
  options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("server,s", value<std::string>(), "server domain or ip")
      ("port,p", value<uint16_t>(), "port")
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
    port = vm["port"].as<uint16_t>();
  }
  if (vm.count("nick")) {
    nick = vm["nick"].as<std::string>();
  }
  if (vm.count("channels")) {
    channels = vm["channels"].as<std::vector<std::string>>();
  }

  std::cout << "Configuration: " << *this << '\n';
}

/* Ability to ostream Config, for example: std::cout << Config << '\n'; */
std::ostream& operator<<(std::ostream& os, const Config& conf) {
  os << conf.server << ' ' << conf.port << ' ';
  for(const auto& c : conf.channels) os << c << ' ';
  return os;
}
