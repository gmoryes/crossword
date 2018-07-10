#ifndef CROSSWORD_GENERATOR_H
#define CROSSWORD_GENERATOR_H

#include <vector>
#include <tuple>
#include <string>
#include <set>
#include <array>
#include <iostream>
#include <algorithm>

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    HORIZONTAL,
    VERTICAL
};

struct Cell {
    Cell() {
        word_index = -1;
    }

    bool is_free() const {
        return word_index == -1;
    }

    wchar_t letter;
    int word_index;
    Direction direction;
};
using map_type = std::vector<std::vector<Cell>>;
using start_position_type = std::tuple<size_t, size_t, Direction>;


struct WordResult {
    std::wstring word;
    int y, x;
    Direction direction;
};

struct WordPosition {
    WordPosition(int y, int x, Direction direction): y(y), x(x), direction(direction) {}

    int y, x;
    Direction direction;
};


/**
 * Create new crossword field
 * @param words - vector of words
 * @return Width and Height of map and Words position
 */
/*std::tuple<size_t, size_t, std::vector<WordResult>>*/void generate_crossword(const std::vector<std::wstring> &words);

#endif //CROSSWORD_GENERATOR_H
