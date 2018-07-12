#ifndef CROSSWORD_SERVER_H
#define CROSSWORD_SERVER_H

#include <stdint.h>

namespace Server {

class Server {
public:
    explicit Server(uint32_t port) : port(port) {}

    bool init();

    void start();

private:

    void worker(int client_socket);

    uint32_t port;
    int server_socket;
};

} // namespace Server

#endif //CROSSWORD_SERVER_H
