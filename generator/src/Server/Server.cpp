#include "Server.h"
#include "Proto.h"
#include "Generator/Generator.h"

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <tuple>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>


namespace Server {

bool Server::init() {

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == -1) {
        std::cerr << "Failed to open socket" << std::endl;
        return false;
    }

    int opts = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) == -1) {
        close(server_socket);
        std::cerr << "Socket setsockopt() failed" << std::endl;
        return false;
    }

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        close(server_socket);
        std::cerr << "Socket bind() failed" << std::endl;
        return false;
    }

    if (listen(server_socket, 5) == -1) {
        close(server_socket);
        std::cerr << "Socket listen() failed" << std::endl;
        return false;
    }

    return true;
}

void Server::worker(int client_socket) {

    std::cerr << "Start worker" << std::endl;

    Proto protocol(client_socket);
    std::vector<std::wstring> data;
    try {
        protocol.read_data(data);
    } catch (std::runtime_error &error) {
        std::cerr << "Bad format of data: " << error.what() << std::endl;
        return;
    }

    generator::Generator gen(data);
    uint32_t H, W;
    std::vector<WordResult> result;
    std::tie(H, W, result) = gen.generate_crossword();

    try {
        protocol.send_data(H, W, result);
    } catch (std::runtime_error &error) {
        std::cerr << "Can not send data to socket, error: " << error.what() << std::endl;
        return;
    }

    std::cerr << "Data send to client, close socket!" << std::endl;
    close(client_socket);
}

void Server::start() {

    std::cerr << "Server started" << std::endl;

    int client_socket;
    socklen_t sinSize = sizeof(struct sockaddr_in);
    sockaddr_in client_addr;

    for (;;) {

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sinSize)) == -1) {
            close(server_socket);
            std::cout << "accept() failed, errno = " << errno << std::endl;
            throw std::runtime_error("Socket accept() failed");
        }

        std::cerr << "Get new client: " << client_socket << std::endl;

        std::thread thread(&Server::worker, this, client_socket);
        thread.detach();
    }
}
} // namespace Server