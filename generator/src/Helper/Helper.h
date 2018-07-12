#ifndef CROSSWORD_HELPER_H
#define CROSSWORD_HELPER_H

#include <codecvt>
#include <string>
#include <utility>
#include <locale>

#include "Types.h"

namespace helper {

/**
 * Return opposite direction (UP -> DOWN etc.).
 */
Direction opposite_direction(Direction direction);

/**
 * Translate direction to it's orientation.
 * @param direction
 * @return Direction::VERTICAL (UP, DOWN) and Direction::HORIZONTAL (RIGHT, LEFT)
 */
Direction convert_to_line_direction(Direction direction);

/**
 * Find start position of word relative to cords.
 * @param letter_index - index of letter, that intersect crossword in (y, x)
 * @param y - Y cord
 * @param x - X cord
 * @param direction_ - direction of word, that already placed in (y, x) cord
 * @return two start position (y, x) + direction. Two because of mirror reflection
 */
std::pair<start_position_type, start_position_type>
get_start_position(uint32_t letter_index, int y, int x, Direction direction_);

/**
 * Return summary length of given words.
 */
uint32_t common_length(const std::vector<std::wstring> &words);

/**
 * Return the nearest sqr, upper than given number.
 * @param x - given number
 * @return
 */
uint32_t nearest_sqr(uint32_t x);

/**
 * Go to the next cell by direction from given cords.
 * @param y
 * @param x
 * @param direction
 */
void go_to_vector(uint32_t &y, uint32_t &x, Direction direction);

/**
 * Check that all cells with given shifts from given cords are clear.
 * @param y
 * @param x
 * @param shift_x
 * @param shift_y
 * @param map - two dimension array of cells
 * @return
 */
bool check_cell_for_clear(int y, int x, std::array<int, 3> &shift_x, std::array<int, 3> &shift_y, map_type &map);

/**
 * Check that we can place new words in given position (bound case), checks that cells (marks as '*')
 * are clear (is case of VERTICAL direction).
 *
 * #########
 * #       #
 * # * * * #
 * #   a   #
 * #   b   #
 * #   c   #
 * #########
 *
 * @param start_position
 * @param map
 * @param left_or_up_bound
 * @return
 */
bool check_bound_letter(start_position_type &start_position, map_type &map, bool left_or_up_bound);

/**
 * Check that we can place new words in given position (bound case), checks that cells (marks as '*')
 * are clear (is case of VERTICAL direction) or another word is pass throw us with same letter.
 *
 * #########  #########
 * #       #  #       #
 * #   a   #  #   a   #
 * # * b * #  # a b a #
 * #   c   #  #   c   #
 * #########  #########
 *
 * @param y
 * @param x
 * @param direction_
 * @param letter
 * @param map
 * @return
 */
bool check_inside_letter(int y, int x, Direction direction_, wchar_t letter, map_type &map);

/**
 * Check that we can place hole word in given position
 * @param start_position
 * @param map
 * @param word
 * @return
 */
bool check_start_position(start_position_type &start_position, map_type &map, const std::wstring &word);

/**
 * Add possible position of given word in given cords.
 * @param possible_positions - here will store a result.
 * @param map
 * @param word
 * @param y
 * @param x
 */
void add_possible_positions(std::vector<WordPosition> &possible_positions, map_type &map,
                            const std::wstring &word, int y, int x);

/**
 * Convert wstring to UTF-8 string
 * @param str
 * @return
 */
std::string wstring_to_utf8 (const std::wstring &str);

} // namespace helper
#endif //CROSSWORD_HELPER_H
