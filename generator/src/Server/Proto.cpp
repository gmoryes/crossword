#include "Proto.h"
#include "Helper/Helper.h"

#include <codecvt>
#include <string>
#include <locale>
#include <iostream>
#include <sstream>

#include <unistd.h>

namespace Server {

#define read_data_from_socket(socket, where, size)                                                         \
    {                                                                                                      \
       ssize_t tmp = read(socket, where, size);                                                            \
       if (tmp != (size)) {                                                                                \
           std::stringstream ss;                                                                           \
           ss << "Bad data, want read to " << #where << "from socket: " << #socket << ", size: " << #size; \
           throw std::runtime_error(ss.str());                                                             \
       }                                                                                                   \
    }                                                                                                      \

#define send_data_to_socket(socket, from, size)                                                            \
    {                                                                                                      \
       ssize_t tmp = write(socket, from, size);                                                            \
       std::cerr << "write(" << socket << ", " << #from << ", " << size << ")" << std::endl;               \
       if (tmp != (size)) {                                                                                \
           std::stringstream ss;                                                                           \
           ss << "write() error(" << errno << "), want write from " << #from << " to socket: ";            \
           ss << socket << ", size: " << size;                                                             \
           throw std::runtime_error(ss.str());                                                             \
       }                                                                                                   \
    }                                                                                                      \

#define check_for_positive(data)             \
    if ((data) < 0) {                        \
        std::stringstream ss;                \
        ss << #data << " is negative";       \
        throw std::runtime_error(ss.str());  \
    }                                        \



std::wstring string_to_wstring(std::string &&t_str) {

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(t_str);
}

void Proto::read_data(std::vector<std::wstring> &data) {

    int words_number;
    read_data_from_socket(client_socket, &words_number, 4);
    check_for_positive(words_number);

    std::cerr << "words_number(" << words_number << ")" << std::endl;

    data.resize(words_number);

    for (int i = 0; i < words_number; i++) {
        int word_length;
        read_data_from_socket(client_socket, &word_length, 4);
        std::cerr << "words[" << i << "] length: " << word_length << std::endl;
        check_for_positive(word_length);

        char buf[word_length + 1];

        read_data_from_socket(client_socket, buf, word_length);
        buf[word_length] = '\0';

        data[i] = string_to_wstring(std::string(buf));
    }
}

void Proto::send_data(uint32_t H, uint32_t W, const std::vector<WordResult> &result) {
    send_data_to_socket(client_socket, &H, sizeof(uint32_t));
    send_data_to_socket(client_socket, &W, sizeof(uint32_t));

    for (auto &item : result) {

        std::cerr << "send" << std::endl;

        std::string string = helper::wstring_to_utf8(item.word);

        std::cerr << "x = " << item.x << ", y = " << item.y << std::endl;
        send_data_to_socket(client_socket, &item.y, sizeof(uint32_t));
        send_data_to_socket(client_socket, &item.x, sizeof(uint32_t));

        std::cerr << "direction = " << item.direction << std::endl;
        static_assert(sizeof(item.direction) == 4, "");
        send_data_to_socket(client_socket, &item.direction, 4);

        uint32_t size = string.size();
        std::cerr << "size = " << size << std::endl;
        std::cerr << string << std::endl;
        send_data_to_socket(client_socket, &size, sizeof(uint32_t));
        send_data_to_socket(client_socket, string.data(), size);
    }
}

};
