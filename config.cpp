#include "config.h"

#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>

namespace po = boost::program_options;

/* Define supported options. */
auto Config::CreateOptionsDesc(const std::string& name) {
  auto desc = std::make_shared<po::options_description>(name);
  desc->add_options()
      ("help,h", "produce help message")
      ("server,s", po::value<std::string>(), "server domain or ip")
      ("port,p", po::value<std::string>()->default_value("6667"), "port")
      ("nick,n", po::value<std::string>(), "your nickname")
      ("channels,c", po::value<std::vector<std::string>>()->multitoken(), "channels to join")
      ;
  return desc;
}

/* Construct Config from cli arguments. */
Config::Config(int ac, char **av) {
  po::variables_map vm;
  auto desc = CreateOptionsDesc("Allowed options");
  po::store(po::parse_command_line(ac, av, *desc), vm);
  po::notify(vm);
  AssignValues(vm, desc);
}

/* Construct Config from config file. */
Config::Config(const std::string& filename) {
  po::variables_map vm;
  auto desc = CreateOptionsDesc("Allowed options");
  std::fstream in(filename, std::ios_base::in);
  if(in) {
    po::store(po::parse_config_file(in, *desc), vm);
    po::notify(vm);
  }
  else
    std::cerr << "ERROR: loading file \"" << filename << "\" failed." << '\n';
  AssignValues(vm, desc);
}

/* Save Config to file. */
void Config::Save(const std::string& filename) {
  std::fstream out(filename, std::ios_base::out);
  if(out)
    out << *this;
  else
    std::cerr << "ERROR: saving file \"" << filename << "\" failed." << '\n';
}

/* Ability to ostream Config, for example: std::cout << Config << '\n'; */
std::ostream& operator<<(std::ostream& os, const Config& conf) {
  os << "server=" << conf.server << '\n' << "port=" << conf.port << '\n'
     << "nick=" << conf.nick << '\n';
  os << "channels=";
  for(const auto& c : conf.channels)
    os << c << ' ';
  os << '\n';
  return os;
}

/* Assign member variables here. */
void Config::AssignValues(const po::variables_map& vm,
                      std::shared_ptr<po::options_description> desc) {
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
}
