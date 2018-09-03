#include "connection.h"

#include <iostream>
#include <array>

#include <boost/asio.hpp>

using namespace boost::asio;

Connection::Connection(const std::string& host, const std::string& port)
  : socket(context) {
  ip::tcp::resolver resolver(context);
  ip::tcp::resolver::query query(host, port);

  /* Iterate all tcp/ip endpoints. */
  auto iter = resolver.resolve(query);
  ip::tcp::endpoint ep = *iter;
  std::cout << "Connecting... " << ep << " ";

  boost::system::error_code ec;
  socket.connect(ep, ec);
  if (!ec) {
    std::cout << " - OK!\n";
    context.run();
    return;
  }
  std::cerr << "ERROR: " << ec.message() << '\n';
  exit(1);
}

Connection::~Connection() { socket.close(); }

/* Send data to server. */
Connection& Connection::operator<<(const std::string& buf) {
  socket.send(buffer(buf));
  return *this;
}

/* Read data from server. */
Connection* Connection::operator>>(std::string& buf) {
  static std::array<char, 1024> arr; // can't use std::string for receiving data
  auto bytes_transferred = socket.receive(buffer(arr, arr.size()));
  std::copy(std::begin(arr), std::begin(arr) + bytes_transferred,
            std::back_inserter(buf));
  return this;
}

