#include "connection.h"

#include <iostream>
#include <array>

namespace as = boost::asio;

Connection::Connection(const std::string& host, const std::string& port)
  : socket{aio} {
  boost::system::error_code ec;
  as::ip::tcp::resolver resolver(aio);
  as::ip::tcp::resolver::query query(host, port);

  /* Possibly iterate all tcp/ip endpoints. */
  auto iter = resolver.resolve(query, ec);
  if (!ec) {
    as::ip::tcp::endpoint ep = *iter;
    socket.connect(ep, ec);
    std::cout << "Connecting... " << ep;
  }
  else {
    std::cerr << "ERROR: " << ec.message() << '\n';
    exit(1);
  }
  if (!ec) {
    std::cout << " - OK!\n";
    if(aio.stopped()) // don't rerun if other thread has started run()
      aio.run();
    return;
  }
  else {
    std::cerr << " ERROR: " << ec.message() << '\n';
    exit(1);
  }
}

Connection::~Connection() { socket.close(); }

/* Send data to server. */
Connection& Connection::operator<<(const std::string& buf) {
  socket.send(as::buffer(buf));
  return *this;
}

/* Read data from server. */
Connection* Connection::operator>>(std::string& buf) {
  static std::array<char, 1024> arr; // can't use std::string for receiving data
  auto bytes_transferred = socket.receive(as::buffer(arr, arr.size()));
  std::copy(std::begin(arr), std::begin(arr) + bytes_transferred,
            std::back_inserter(buf));
  return this;
}

