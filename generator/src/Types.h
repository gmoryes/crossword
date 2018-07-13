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
    Cell(): _first(false) {
        _word_index = {-1, -1};
    }

    /**
     * Check that cell is free
     */
    bool is_free() const {
        return _word_index[0] == -1 && _word_index[1] == -1;
    }

    /**
     * Add new owner for cell
     * @param index - index of new owner
     */
    void add_owner(int index) {

        // Check, that new owner exists
        assert(index != -1);

        // Check, that cell has free space
        assert(_word_index[0] == -1 || _word_index[1] == -1);

        // Add in free space
        if (_word_index[0] == -1)
            _word_index[0] = index;
        else
            _word_index[1] = index;
    }

    /**
     * Removes one owner of cell
     * @param index - index of removing owner
     */
    void remove_owner(int index) {

        int to_delete = -1;
        if (_word_index[0] == index)
            to_delete = 0;
        else if (_word_index[1] == index)
            to_delete = 1;

        assert(to_delete != -1);

        _word_index[to_delete] = -1;
    }

    /**
     * @return the first owner of cell
     */
    int owner() const {
        return _word_index[0];
    }

    /**
     * @return letter if wchar_t format
     */
    wchar_t char_letter() const {
        return _letter[0];
    }

    /**
     * @return letter in std::wstring format
     */
    std::wstring &wstring_letter() {
        return _letter;
    }

    void make_first() {
        _first = true;
    }

    void clean_first() {
        _first = false;
    }

    bool is_first() const {
        return _first;
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

private:
    std::wstring _letter;
    std::array<int, 2> _word_index;
    bool _first;
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
