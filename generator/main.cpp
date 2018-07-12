#include "Generator/Generator.h"
#include "Server/Server.h"

#include <iostream>
#include <vector>
#include <string>

int main() {

    Server::Server server(2809);
    if (!server.init()) {
        std::cerr << "Can not run server" << std::endl;
        return 1;
    }

    try {
        server.start();
    } catch(std::runtime_error &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}