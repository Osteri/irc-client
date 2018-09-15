#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace as = boost::asio;

class Connection
{
public:
  Connection(const std::string& host, const std::string& port);
  Connection(const Connection&) = delete;
  ~Connection();
  Connection& operator<<(const std::string& buf);
  Connection* operator>>(std::string& buf);
private:
  as::io_service aio; // async io
  as::ip::tcp::socket socket;
};

#endif // CONNECTION_H
