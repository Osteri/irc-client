### Description

Very simple and extensible interactive IRC client/bot reference design written in modern C++. 
Only raw [IRC protocol](https://tools.ietf.org/html/rfc2812) messages are accepted, which means you have to type `privmsg #channel :Your message here` if you want to send a message. Remember to join before sending a message.

![Using IRC client.](https://github.com/Osteri/irc-client/blob/master/irc.gif?raw=true)

### Usage

Print available program options:

    ./irc-client --help

Example usage:

    ./irc-client -s chat.freenode.net -n YourNick -c "#linux" "#ubuntu"

...or long version (same as above):

    ./irc-client --server chat.freenode.net --nick YourNick --channels "#linux" "#ubuntu"

### Requirements

* Boost.Asio (networking)
* Boost.Program_options (parsing cli options)

#### Fedora:

    dnf install boost-program-options boost-system boost-devel

### Install

    mkdir build && cd build
    cmake ..     
    # or cmake .. -DSANITIZER="address"
    # or cmake .. -DSANITIZER="thread"
    make install -j 12
