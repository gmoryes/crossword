#ifndef CROSSWORD_GENERATOR_H
#define CROSSWORD_GENERATOR_H

#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <set>
#include <array>
#include <iostream>
#include <algorithm>
#include <cassert>

#include "Types.h"

namespace generator {

class Generator {
public:
    explicit Generator(std::vector<std::wstring> &words):
        _words(std::move(words)),
        current_word(0),
        map(SIDE, std::vector<Cell>(SIDE)) {}

    /**
     * Create new crossword field
     * @param words - vector of words
     * @return Width and Height of map and Words position
     */
    std::tuple<uint32_t, uint32_t, std::vector<WordResult>> generate_crossword();

private:

    static constexpr uint32_t SIDE = 1000;

    /* Try to find suitable place for _words[current_word]. Implement recursive bust. */
    bool bust();

    /* Two dimensial array - crossword */
    map_type map;

    /* Array of words to make crossword from them */
    std::vector<std::wstring> _words;

    /* Current index of word in bust */
    uint32_t current_word;
};

} // namespace generator

#endif //CROSSWORD_GENERATOR_H
