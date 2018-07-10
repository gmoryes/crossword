#include "Generator.h"

const double WORDS_COVERING = 0.8;

namespace helper {
    Direction opposite_direction(Direction direction) {
        switch (direction) {
            case Direction::UP:
                return Direction::DOWN;
            case Direction::DOWN:
                return Direction::UP;
            case Direction::LEFT:
                return Direction::RIGHT;
            case Direction::RIGHT:
                return Direction::LEFT;
            default:
                throw std::runtime_error("Unknown type of direction, opposite_direction()");
        }

    }

    Direction convert_to_line_direction(Direction direction) {
        assert(direction != Direction::VERTICAL);
        assert(direction != Direction::HORIZONTAL);

        if (direction == Direction::DOWN || direction == Direction::UP)
            return Direction::VERTICAL;
        else
            return Direction::HORIZONTAL;
    }

    std::pair<start_position_type, start_position_type> get_start_position(size_t letter_index, int y, int x, Direction direction_) {
        Direction direction = convert_to_line_direction(direction_);

        if (direction == Direction::VERTICAL) {
            return {
                {y, x - letter_index, Direction::RIGHT},
                {y, x + letter_index, Direction::LEFT}
            };
        } else {
            return {
                {y - letter_index, x, Direction::DOWN},
                {y + letter_index, x, Direction::UP}
            };
        }
    };

    size_t common_length(const std::vector<std::wstring> &words) {
        size_t length = 0;
        for (auto &i : words)
            length += words.size();

        return length;
    }

    size_t nearest_sqr(size_t x) {
        size_t n = 1;
        while (n * n < x)
            n++;

        return n;
    }

    void go_to_vector(size_t &y, size_t &x, Direction direction) {
        switch (direction) {
            case UP:
                y--;
                return;
            case DOWN:
                y++;
                return;
            case LEFT:
                x--;
                return;
            case RIGHT:
                x++;
                return;
            default:
                throw std::runtime_error("Unknown value of Direction");
        }
    }

    bool check_cell_for_clear(int y, int x, std::array<int, 3> &shift_x, std::array<int, 3> &shift_y, map_type &map) {
        for (int i = 0; i < 3; i++)
            if (map[y + shift_y[i]][x + shift_x[i]].word_index != -1)
                return false;

        return true;
    }

    bool check_bound_letter(start_position_type &start_position, map_type &map, bool left_or_up_bound) {
        int y, x;
        Direction direction_;
        std::tie(y, x, direction_) = start_position;

        Direction direction = convert_to_line_direction(direction_);
        std::array<int, 3> must_be_clear_x,  must_be_clear_y;

        if (direction == Direction::HORIZONTAL) {
            if (left_or_up_bound) {
                must_be_clear_x = {-1, -1, -1};
                must_be_clear_y = {0,  -1,  1};
            } else {
                must_be_clear_x = { 1, 1, 1};
                must_be_clear_y = {-1, 0, 1};
            }
        } else {
            if (left_or_up_bound) {
                must_be_clear_x = {-1,  0,  1};
                must_be_clear_y = {-1, -1, -1};
            } else {
                must_be_clear_x = {-1, 0, 1};
                must_be_clear_y = { 1, 1, 1};
            }
        }

        return check_cell_for_clear(y, x, must_be_clear_x, must_be_clear_y, map);
    }

    bool check_inside_letter(int y, int x, Direction direction_, wchar_t letter, map_type &map) {
        Direction direction = convert_to_line_direction(direction_);

        std::array<int, 2> must_check_x{}, must_check_y{};

        if (direction == Direction::HORIZONTAL) {
            must_check_x = {0,  0};
            must_check_y = {-1, 1};
        } else {
            must_check_x = {-1, 1};
            must_check_y = { 0, 0};
        }

        int current_word_index = map[y][x].word_index;
        for (int i = 0; i < 2; i++) {
            int shift_word_index = map[y + must_check_y[i]][x + must_check_x[i]].word_index;
            // Либо эта ячейка свободна
            if (shift_word_index == -1)
                continue;

            // Либо занята тем же словом, что и текущая ячейка и буквы в этих местах совпадают
            if (shift_word_index == current_word_index && map[y][x].letter == letter)
                continue;

            return false;
        }

        if (!map[y][x].is_free() && map[y][x].letter != letter)
            return false;

        return true;
    }

    bool check_start_position(start_position_type &start_position, map_type &map, const std::wstring &word) {
        size_t y, x;
        Direction direction;
        std::tie(y, x, direction) = start_position;

        bool left_or_up_bound = (direction == Direction::RIGHT || direction == Direction::DOWN);

        if (not check_bound_letter(start_position, map, left_or_up_bound))
            return false;

        for (size_t i = 0; i < word.size(); i++) {
            if (not check_inside_letter(y, x, direction, word[i], map))
                return false;

            go_to_vector(y, x, direction);
        }

        start_position_type last_symbol{y, x, direction};
        return check_bound_letter(last_symbol, map, !left_or_up_bound);
    }

    void add_possible_positions(std::vector<WordPosition> &possible_positions,
                                map_type &map, const std::wstring &word, int y, int x) {

        std::vector<size_t> letter_occur_index;
        wchar_t letter = map[y][x].letter;
        for (size_t i = 0; i < word.size(); i++)
            if (word[i] == letter)
                letter_occur_index.push_back(i);

        Direction intersect_direction = map[y][x].direction;

        Direction direction;
        int tmp_y, tmp_x;

        if (word == L"neighbour") {
            int asd = 4;
        }

        for (auto index : letter_occur_index) {
            std::pair<start_position_type, start_position_type> start_position =
                get_start_position(index, y, x, intersect_direction);

            int a1 = std::get<0>(start_position.first);
            int b1 = std::get<1>(start_position.first);

            if (check_start_position(start_position.first, map, word)) {
                std::tie(tmp_y, tmp_x, direction) = start_position.first;
                possible_positions.emplace_back(tmp_y, tmp_x, direction);
            }

            if (check_start_position(start_position.second, map, word)) {
                std::tie(tmp_y, tmp_x, direction) = start_position.second;
                possible_positions.emplace_back(tmp_y, tmp_x, direction);
            }
        }
    }
} // namespace helper

bool bust(map_type &map, const std::vector<std::wstring> &words, int current_word) {

    if (current_word == words.size())
        return true;

    std::set<wchar_t> letters;
    for (auto letter : words[current_word])
        letters.insert(letter);

    std::vector<WordPosition> possible_positions;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map.size(); x++) {

            if (map[y][x].is_free())
                continue;

            wchar_t current_letter = map[y][x].letter;

            if (letters.find(current_letter) == letters.end())
                continue;

            helper::add_possible_positions(possible_positions, map, words[current_word], y, x);
        }
    }

    for (int i = 0; i < possible_positions.size(); i++) {
        size_t y = possible_positions[i].y;
        size_t x = possible_positions[i].x;
        Direction direction = possible_positions[i].direction;
        for (int j = 0; j < words[current_word].size(); j++) {
            map[y][x].letter = words[current_word][j];
            map[y][x].word_index = current_word;
            map[y][x].direction = direction;

            helper::go_to_vector(y, x, direction);
        }

        bool good = bust(map, words, current_word + 1);
        if (good)
            return true;

        direction = helper::opposite_direction(direction);
        for (int j = 0; j < words[current_word].size(); j++) {
            map[y][x].word_index = -1;

            helper::go_to_vector(y, x, direction);
        }
    }

    return false;
}

void /*std::tuple<size_t, size_t, std::vector<WordResult>> */generate_crossword(const std::vector<std::wstring> &words) {
    //size_t big_side = helper::nearest_sqr(helper::common_length(words) / (1 - WORDS_COVERING));

    size_t big_side = 1000;

    map_type map(big_side, std::vector<Cell>(big_side));

    size_t start_pos_x = big_side / 3;
    size_t start_pos_y = big_side / 3;
    Direction start_dir = Direction::RIGHT;

    for (int i = 0; i < words[0].size(); i++) {
        map[start_pos_y][start_pos_x].word_index = 0;
        map[start_pos_y][start_pos_x].letter = words[0][i];
        map[start_pos_y][start_pos_x].direction = start_dir;

        helper::go_to_vector(start_pos_y, start_pos_x, start_dir);
    }

    if (bust(map, words, 1)) {
        std::cout << "Success!" << std::endl;

        size_t mini_x(big_side + 1), mini_y(big_side + 1), maxi_x(0), maxi_y(0);

        for (size_t y = 0; y < big_side; y++) {
            for (size_t x = 0; x < big_side; x++) {
                if (map[y][x].is_free())
                    continue;

                mini_x = std::min(mini_x, x);
                mini_y = std::min(mini_y, y);

                maxi_x = std::max(maxi_x, x);
                maxi_y = std::max(maxi_y, y);
            }
        }

        for (size_t y = mini_y; y <= maxi_y; y++) {
            for (size_t x = mini_x; x <= maxi_x; x++) {
                if (!map[y][x].is_free())
                    std::wcout << map[y][x].letter;
                else
                    std::wcout << ' ';
            }

            std::wcout << std::endl;
        }
    } else {
        std::wcout << "I can't :(\n";
    }
}
