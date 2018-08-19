#include <thread>
#include <atomic>
#include <iostream>

#include "connection.h"
#include "config.h"

int main(int ac, char** av)
{
  Config conf(ac, av);
  Connection con(conf.server, conf.port);
  std::atomic<bool> run(true);  // monitor 'send thread'
                                // and exit based on it

  /* This thread takes input from command line and sends it. */
  std::thread cli_send([&con, &run]() {
    std::string buf;
    while(buf != "quit") {
      try {
        std::getline(std::cin, buf);
        con << buf << "\r\n";
      } catch (const boost::system::system_error& ex) {
        std::cerr << ex.what() << '\n';
        exit(1);
      }
    }
    run = false;
  });
  /* This thread receives replies from server and prints them. */
  std::thread receive([&con, &run]() {
    std::string buf;
    buf.reserve(1024);
    while(run) {
      try {
        con >> buf;
      } catch (const boost::system::system_error& ex) {
        std::cerr << ex.what() << '\n';
        exit(1);
      }
      std::cout << buf;
      buf.clear(); // possibly remove
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  });

  /* IRC protocol messages. Set nick, register connection, etc... */
  try {
    con << "nick " << conf.nick << "\r\n";
    con << "user " << conf.nick << " 8 * :Real Name" << "\r\n";
    for(const auto& c : conf.channels)
      con << "join " << c << "\r\n";
  } catch (const boost::system::system_error& ex) {
    std::cerr << ex.what() << '\n';
  }

  cli_send.join();
  receive.join();
  return 0;
}
