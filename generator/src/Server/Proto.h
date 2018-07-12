#ifndef CROSSWORD_PROTO_H
#define CROSSWORD_PROTO_H

#include "Types.h"

#include <string>
#include <vector>

namespace Server {
class Proto {
public:
    explicit Proto(int client_socket): client_socket(client_socket) {}

    void read_data(std::vector<std::wstring> &data);

    void send_data(uint32_t H, uint32_t W, const std::vector<WordResult> &result);
private:
    int client_socket;
};
} // namespace Server

#endif //CROSSWORD_PROTO_H
