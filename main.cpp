#include <iostream>
#include "server.h"
#include <algorithm>

int main()
{
    std::cout << "Hello World!" << std::endl;
    Connection con1("google.fi");
    Connection con2("wikipedia.org");
    std::cout << "Bye World!" << std::endl;

    std::vector<std::string> v;
    v.push_back("Hello!");
    v.push_back("Hello1!");
    v.push_back("Hello2!");
    v.push_back("Hello22222!");
    v.push_back("Hello222!");
    v.push_back("Hello222!");
    v.push_back("Moi22!");
    std::sort(v.begin(), v.end());

    return 0;
}
