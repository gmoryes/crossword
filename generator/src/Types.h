#ifndef CROSSWORD_TYPES_H
#define CROSSWORD_TYPES_H

#include <cassert>
#include <string>
#include <vector>
#include <array>
#include <deque>

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    HORIZONTAL,
    VERTICAL
};

class Cell {
public:
    Cell() {
        _word_index = {-1, -1};
    }

    bool is_free() const {
        return _directions.empty();
    }

    void add_direction(Direction d) {
        _directions.push_back(d);
    }

    void remove_direction(Direction d) {
        for (;;) {
            Direction tmp = _directions.front();
            _directions.pop_front();
            if (tmp == d) {
                return;
            } else {
                _directions.push_back(tmp);
            }
        }
    }

    Direction get_direction() {
        return _directions.front();
    }

    bool pop_direction(Direction &direction) {
        if (_directions.empty())
            return false;

        direction = _directions.front();
        _directions.pop_front();
        return true;
    }

    wchar_t letter{};
private:

    std::deque<int> _word_index;
    std::deque<Direction> _directions;
};

/* Type of map where places words */
using map_type = std::vector<std::vector<Cell>>;

/* tuple of (y, x) cords and direction of word */
using start_position_type = std::tuple<uint32_t, uint32_t, Direction>;

struct WordResult {
    std::wstring word;
    uint32_t y, x;
    Direction direction;
};

/* Helpful for save words position during bust */
struct WordPosition {
    WordPosition(): y(0), x(0), direction(UP) {}
    WordPosition(int y, int x, Direction direction) : y(y), x(x), direction(direction) {}

    uint32_t y, x;
    Direction direction;
};


#endif //CROSSWORD_TYPES_H
