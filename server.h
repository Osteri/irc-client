#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <cstdio>
#include <forward_list>
#include <thread>
#include <functional>

#include <boost/asio.hpp>

using namespace boost::asio;

class Connection
{
public:

    Connection(const std::string name) : socket(context) {

        ip::tcp::resolver resolver(context);
        ip::tcp::resolver::query query(name, "http");

        /* Iterate all tcp/ip endpoints. */
        auto iter = resolver.resolve(query);
        ip::tcp::endpoint ep = *iter;
        std::cout << "Connecting... " << ep;

        boost::system::error_code ec;
        socket.connect(ep, ec);
        if (!ec) {
            std::cout << " - OK!\n";
            context.run();
            return;
        }
        std::cerr << " - ERROR!\n";

        /* async connect */
//        socket.async_connect(ep,
//                             [this](const boost::system::error_code& ec) {
//            if (!ec)
//                std::cout << " OK!\n";
//            else
//                std::cout << " ERROR!\n";
//        }
//        );
    }

    ~Connection() { socket.close(); }

private:
    io_service context;
    ip::tcp::socket socket;
};

#endif // C
