#ifndef CROSSWORD_GENERATOR_H
#define CROSSWORD_GENERATOR_H

#include <vector>
#include <tuple>
#include <string>
#include <set>
#include <array>
#include <iostream>
#include <algorithm>
#include <cassert>

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
        word_index = {-1, -1};
    }

    /**
     * Check that cell if free
     */
    bool is_free() const {
        return word_index[0] == -1 && word_index[1] == -1;
    }

    /**
     * Add new owner for cell
     * @param index - index of new owner
     */
    void add_owner(int index) {
        // Check, that new owner exists
        assert(index != -1);
        // Check, that cell has free space
        assert(word_index[0] == -1 || word_index[1] == -1);

        // Add in free space
        if (word_index[0] == -1)
            word_index[0] = index;
        else
            word_index[1] = index;
    }

    void remove_owner(int index) {
        int to_delete = -1;
        if (word_index[0] == index)
            to_delete = 0;
        else if (word_index[1] == index)
            to_delete = 1;

        assert(to_delete != -1);

        word_index[to_delete] = -1;
    }

    /**
     * @return the first owner of cell
     */
    int owner() const {
        return word_index[0];
    }

    wchar_t letter;
    std::array<int, 2> word_index;
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
