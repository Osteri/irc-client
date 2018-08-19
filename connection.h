#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

#include <boost/asio.hpp>

using namespace boost::asio;

class Connection
{
public:
  Connection(const std::string name, const uint16_t port);
  ~Connection();
  Connection& operator<<(const std::string& buf);
  Connection* operator>>(std::string& buf);
private:
  io_service context;
  ip::tcp::socket socket;
};

#endif // CONNECTION_H
