#include "Generator.h"
#include "Debug.h"
#include "Helper/Helper.h"

namespace generator {

bool Generator::bust() {

    if (current_word == _words.size())
        return true;

    std::set<wchar_t> letters;
    for (auto letter : _words[current_word])
        letters.insert(letter);

    std::vector<WordPosition> possible_positions;

    for (uint32_t y = 0; y < map.size(); y++) {
        for (uint32_t x = 0; x < map.size(); x++) {

            if (map[y][x].is_free())
                continue;

            wchar_t current_letter = map[y][x].char_letter();

            if (letters.find(current_letter) == letters.end())
                continue;

            helper::add_possible_positions(possible_positions, map, _words[current_word], y, x);
        }
    }

    for (auto &possible_position : possible_positions) {
        uint32_t y = possible_position.y;
        uint32_t x = possible_position.x;
        Direction direction = possible_position.direction;

        map[y][x].make_first();
        _result[current_word] = WordPosition(y, x, direction);
        for (int j = 0; j < _words[current_word].size(); j++) {
            map[y][x].wstring_letter() = _words[current_word][j];
            map[y][x].add_owner(current_word);
            map[y][x].add_direction(direction);

            helper::go_to_vector(y, x, direction);
        }

        debug::print_map(map);

        uint32_t save = current_word;
        current_word++;
        bool good = bust();
        if (good)
            return true;

        current_word = save;
        Direction back_direction = helper::opposite_direction(direction);
        for (int j = 0; j < _words[current_word].size(); j++) {
            helper::go_to_vector(y, x, back_direction);
            map[y][x].remove_owner(current_word);
            map[y][x].remove_direction(direction);
        }
        map[y][x].clean_first();
    }

    return false;
}

std::tuple<uint32_t, uint32_t, std::vector<WordResult>> Generator::generate_crossword() {

    uint32_t H(0), W(0);
    std::vector<WordResult> result(_words.size());

    std::cout << "Start generate crossword" << std::endl;

    uint32_t start_pos_x = SIDE / 3;
    uint32_t start_pos_y = SIDE / 3;
    Direction start_dir = Direction::RIGHT;

    map[start_pos_y][start_pos_x].make_first();
    _result[0] = WordPosition(start_pos_y, start_pos_x, start_dir);
    for (int i = 0; i < _words[0].size(); i++) {
        map[start_pos_y][start_pos_x].add_owner(0);
        map[start_pos_y][start_pos_x].wstring_letter() = _words[0][i];
        map[start_pos_y][start_pos_x].add_direction(start_dir);

        helper::go_to_vector(start_pos_y, start_pos_x, start_dir);
    }


    current_word = 1;

    if (bust()) {
        std::cout << "Success!" << std::endl;

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

        debug::print_map(map);

        H = maxi_y - mini_y + 1;
        W = maxi_x - mini_x + 1;

        std::cout << "side(" << H << "x" << W << ")" << std::endl;

        for (int i = 0; i < _words.size(); i++) {
            result[i].y = _result[i].y - mini_y;
            result[i].x = _result[i].x - mini_x;
            result[i].direction = _result[i].direction;

            result[i].word = _words[i];
        }

//        for (uint32_t y = mini_y; y <= maxi_y; y++) {
//            for (uint32_t x = mini_x; x <= maxi_x; x++) {
//
//                std::cout << y - mini_y << ' ' << x - mini_x << " => " << map[y][x].is_free();
//
//                if (!map[y][x].is_free() && map[y][x].is_first()) {
//
//                    Direction d;
//                    while (map[y][x].pop_direction(d)) {
//
//                        std::cout << " first ";
//
//                        uint32_t tmp_y(y), tmp_x(x);
//
//                        result.push_back({L"", tmp_y - mini_y, tmp_x - mini_x, d});
//
//                        while (!map[tmp_y][tmp_x].is_free()) {
//                            result.back().word += map[tmp_y][tmp_x].wstring_letter();
//                            helper::go_to_vector(tmp_y, tmp_x, d);
//                        }
//
//                        std::cout << " get(" << helper::wstring_to_utf8(result.back().word) << ") ";
//                    }
//                }
//
//                std::cout << std::endl;
//            }
//        }



    } else {
        std::cout << "I can't :(" << std::endl;
    }

    return {H, W, result};
}
} // namespace generator
