#include "Helper.h"

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

std::pair<start_position_type, start_position_type> get_start_position(uint32_t letter_index, int y, int x, Direction direction_) {
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

uint32_t common_length(const std::vector<std::wstring> &words) {
    uint32_t length = 0;
    for (auto &i : words)
        length += words.size();

    return length;
}

uint32_t nearest_sqr(uint32_t x) {
    uint32_t n = 1;
    while (n * n < x)
        n++;

    return n;
}

void go_to_vector(uint32_t &y, uint32_t &x, Direction direction) {
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
        if (!map[y + shift_y[i]][x + shift_x[i]].is_free())
            return false;

    return true;
}

bool check_bound_letter(start_position_type &start_position, map_type &map, bool left_or_up_bound) {
    int y, x;
    Direction direction_;
    std::tie(y, x, direction_) = start_position;

    Direction direction = convert_to_line_direction(direction_);
    std::array<int, 3> must_be_clear_x{},  must_be_clear_y{};

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

    int current_word_index = map[y][x].owner();
    for (int i = 0; i < 2; i++) {
        int shift_word_index = map[y + must_check_y[i]][x + must_check_x[i]].owner();
        // Cell is free
        if (shift_word_index == -1)
            continue;

        // Or it has owner the same with current and letter are equals
        if (shift_word_index == current_word_index && map[y][x].char_letter() == letter)
            continue;

        return false;
    }

    if (!map[y][x].is_free() && map[y][x].char_letter() != letter)
        return false;

    return true;
}

bool check_start_position(start_position_type &start_position, map_type &map, const std::wstring &word) {
    uint32_t y, x;
    Direction direction;
    std::tie(y, x, direction) = start_position;

    bool left_or_up_bound = (direction == Direction::RIGHT || direction == Direction::DOWN);

    if (not check_bound_letter(start_position, map, left_or_up_bound))
        return false;

    for (uint32_t i = 0; i < word.size(); i++) {
        if (not check_inside_letter(y, x, direction, word[i], map))
            return false;

        if (i + 1 != word.size())
            go_to_vector(y, x, direction);
    }

    start_position_type last_symbol{y, x, direction};
    return check_bound_letter(last_symbol, map, !left_or_up_bound);
}

void add_possible_positions(std::vector<WordPosition> &possible_positions, map_type &map,
                            const std::wstring &word, int y, int x) {

    std::vector<uint32_t> letter_occur_index;
    wchar_t letter = map[y][x].char_letter();
    for (uint32_t i = 0; i < word.size(); i++)
        if (word[i] == letter)
            letter_occur_index.push_back(i);

    Direction intersect_direction = map[y][x].get_direction();

    Direction direction;
    int tmp_y, tmp_x;

    for (auto index : letter_occur_index) {
        std::pair<start_position_type, start_position_type> start_position =
            get_start_position(index, y, x, intersect_direction);

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

std::string wstring_to_utf8 (const std::wstring &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t> > myconv;
    return myconv.to_bytes(str);
}

} // namespace helper