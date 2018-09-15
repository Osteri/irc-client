#include "config.h"

#include <iostream>
#include <fstream>

#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

/* Define supported options. Reducing boiler plate code in constructors. */
auto Config::CreateOptionsDesc(const std::string& name) {
  auto desc = std::make_shared<po::options_description>(name);
  desc->add_options()
      ("help,h", "produce help message")
      ("server,s", po::value<std::string>()->required(), "server domain or ip")
      ("port,p", po::value<std::string>()->default_value("6667"), "port")
      ("nick,n", po::value<std::string>()->required(), "your nickname")
      ("channels,c", po::value<std::vector<std::string>>()->multitoken(), "channels to join")
      ;
  return desc;
}

/* Construct Config from cli arguments. */
Config::Config(int ac, char **av) {
  if(ac == 1) // no parameters
    return;
  try {
    po::variables_map vm;
    auto desc = CreateOptionsDesc("Allowed options");
    po::store(po::parse_command_line(ac, av, *desc), vm);
    /* Check "help" before po::notify(). Otherwise "required_options" exception
     * is thrown. Help doesn't require "required_options" so we don't want to
     * check them. */
    if(vm.count("help")) {
      std::cout << *desc << '\n';
      exit(1);
    }
    po::notify(vm);
    AssignValues(vm);
  }
  catch(const boost::program_options::error& e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    exit(1);
  }
}

/* Construct Config from config file. */
Config::Config(const std::string& filename) {
  try {
    std::ifstream in(filename, std::ios_base::in);
    in.exceptions(std::ifstream::badbit); // throw if file doesn't exist
    po::variables_map vm;
    auto desc = CreateOptionsDesc("Allowed options");
    po::store(po::parse_config_file(in, *desc), vm);
    /* Check "help" before po::notify(). Otherwise "required_options" exception
     * is thrown. Help doesn't require "required_options" so we don't want to
     * check them. */
    if(vm.count("help")) {
      std::cout << *desc << '\n';
      exit(1);
    }
    po::notify(vm);
    AssignValues(vm);
  }
  catch(const std::ifstream::failure) {
    std::cerr << "ERROR: reading file \"" << filename << "\" failed. "
              << '\n';
    exit(1);
  }
  catch(const boost::program_options::error& e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    exit(1);
  }
}

/* Save Config to file. */
void Config::Save(const std::string& filename) {
  std::fstream out(filename, std::ios_base::out);
//  out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  if(out)
    out << *this;
  else
    std::cerr << "ERROR: writing file \"" << filename << "\" failed." << '\n';
}

/* Ability to fstream Config; for example: std::fstream << Config << '\n';
 * Normal use case is for saving config to a file. */
std::ostream& operator<<(std::fstream& os, const Config& conf) {
  os << "server=" << conf.server << '\n' << "port=" << conf.port << '\n'
     << "nick=" << conf.nick << '\n';
  /* 1.) Replace 1st '#' char as '/'. Otherwise boost::program_options
   * will think it's a comment. We must agree that '#' is some other
   * character. In this case we chose '/'. */
  for(auto c : conf.channels) {
    if(c.at(0) == '#') c.replace(c.cbegin(), ++c.begin(), {'/'});
    os << "channels=" << c.c_str() << '\n';
  }
  return os;
}

/* Ability to ostream Config; for example: std::cout << Config << '\n';
 * Normal use case is for printing config. */
std::ostream& operator<<(std::ostream& os, const Config& conf) {
  os << "server=" << conf.server << '\n' << "port=" << conf.port << '\n'
     << "nick=" << conf.nick << '\n';
  os << "channels=";
  for(auto c : conf.channels) os << c << ' ';
  os << '\n';
  return os;
}

/* Assign member variables from boost variables_map. */
void Config::AssignValues(const po::variables_map& vm) {
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
    /* Replace every channels 1st '/' char as '#'. Reverse operation of 1.) */
    for(auto& c : channels) {
      if(c.at(0) == '/') c.replace(c.cbegin(), ++c.begin(), {'#'});
    }
  }
}
