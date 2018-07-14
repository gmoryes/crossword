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
#include <unordered_map>
#include <deque>
#include <unordered_map>

#include "Types.h"

namespace generator {

class Generator {
public:
    explicit Generator(std::vector<std::wstring> &words):
        _words(std::move(words)),
        map(SIDE, std::vector<Cell>(SIDE)),
        _result(_words.size()) {

        for (int i = 0; i < _words.size(); i++) {
            _bust_words.push_back(i);
        }
    }

    /**
     * Create new crossword field
     * @param words - vector of words
     * @return Width and Height of map and Words position
     */
    std::tuple<uint32_t, uint32_t, std::vector<WordResult>> generate_crossword();

private:

    bool start_bust();
    void add_word_to_map(uint32_t &y, uint32_t &x, Direction direction, int current_word);

    static constexpr uint32_t SIDE = 1000;

    /* Try to find suitable place for _words[current_word]. Implement recursive bust. */
    bool bust();

    /* Two dimensial array - crossword */
    map_type map;

    /* Array of words to make crossword from them */
    std::vector<std::wstring> _words;

    /* Result data */
    std::vector<WordPosition> _result;

    /* Sequence of words to be busted */
    std::deque<int> _bust_words;

    /* Hash table of wchat_t => positions where that char placed */
    std::unordered_map<wchar_t, std::vector<std::pair<uint32_t, uint32_t> > > _letters;
};

} // namespace generator

#endif //CROSSWORD_GENERATOR_H
