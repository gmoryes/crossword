#include "Generator.h"
#include "Helper/Helper.h"
#include "Debug.h"

#include <algorithm>

namespace generator {

void Generator::add_word_to_map(uint32_t &y, uint32_t &x, Direction direction, int current_word) {

    _result[current_word] = WordPosition(y, x, direction);
    for (auto letter : _words[current_word]) {
        map[y][x].letter = letter;
        map[y][x].add_direction(direction);

        auto it = _letters.find(letter);
        if (it == _letters.end()) {
            std::tie(it, std::ignore) = _letters.emplace(
                letter,
                std::vector<std::pair<uint32_t, uint32_t>>()
            );
        }

        it->second.emplace_back(y, x);

        helper::go_to_vector(y, x, direction);
    }
}

bool Generator::bust() {

    if (_bust_words.empty())
        return true;

    /* Вектор возможных позициий (координты + направление слова) */
    std::vector<WordPosition> possible_positions;

    /* Какое слово мы сейчас пытаемся поставить */
    int current_word = _bust_words.front();
    _bust_words.pop_front();

    if (_bust_words.size() + 1 == _words.size()) {
        /* Случай первого слова */
        possible_positions.emplace_back(SIDE / 3, SIDE / 3, Direction::RIGHT);
    } else {

        std::set<wchar_t> letters;
        for (auto letter : _words[current_word])
            letters.insert(letter);

        for (auto &letter : letters) {
            auto it = _letters.find(letter);
            if (it == _letters.end())
                continue;

            for (auto&& [y, x] : it->second)
                helper::add_possible_positions(possible_positions, map, _words[current_word], y, x);
        }
    }

    for (auto&& [y, x, direction] : possible_positions) {

        add_word_to_map(y, x, direction, current_word);

        /* Перебираем все слова по очереди, пока встретим слово с которого начали */
        int save = _bust_words.front();
        do {

            bool good = bust();
            if (good)
                return true;

            debug::print_map(map);

            _bust_words.push_back(_bust_words.front());
            _bust_words.pop_front();
        } while (save != _bust_words.front());

        Direction back_direction = helper::opposite_direction(direction);
        for (wchar_t letter : _words[current_word]) {
            helper::go_to_vector(y, x, back_direction);
            map[y][x].remove_direction(direction);

            _letters[letter].pop_back();
        }
    }

    _bust_words.push_front(current_word);
    return false;
}

bool Generator::start_bust() {
    size_t n = 0;
    while (n != _words.size()) {
        if (bust())
            return true;

        _bust_words.push_back(_bust_words.front());
        _bust_words.pop_front();

        n++;
    }

    return false;
}

std::tuple<uint32_t, uint32_t, std::vector<WordResult>> Generator::generate_crossword() {

    std::stable_sort(_words.begin(), _words.end(), [] (auto&& a, auto&& b) {
        return a.size() > b.size();
    });

    uint32_t H(0), W(0);
    std::vector<WordResult> result(_words.size());

    std::cout << "Start generate crossword" << std::endl;

    if (start_bust()) {
        std::cout << "Success!" << std::endl;

        debug::print_map(map);

        uint32_t mini_x(SIDE + 1), mini_y(SIDE + 1), maxi_x(0), maxi_y(0);

        for (uint32_t y = 0; y < SIDE; y++) {
            for (uint32_t x = 0; x < SIDE; x++) {
                if (map[y][x].is_free())
                    continue;

                mini_x = std::min(mini_x, x);
                mini_y = std::min(mini_y, y);

                maxi_x = std::max(maxi_x, x);
                maxi_y = std::max(maxi_y, y);
            }
        }

        H = maxi_y - mini_y + 1;
        W = maxi_x - mini_x + 1;

        std::cout << "side(" << H << "x" << W << ")" << std::endl;

        for (int i = 0; i < _words.size(); i++) {
            result[i].y = _result[i].y - mini_y;
            result[i].x = _result[i].x - mini_x;
            result[i].direction = _result[i].direction;

            result[i].word = _words[i];
        }

    } else {
        std::cout << "I can't :(" << std::endl;
    }

    return {H, W, result};
}

} // namespace generator
