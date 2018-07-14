#include <iostream>

#include "Debug.h"
#include "Helper/Helper.h"

namespace debug {

void print_map(map_type &map) {

    std::cout << "==============\n";
    size_t big_side = 1000;
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
            if (!map[y][x].is_free()) {
                std::cout << helper::wstring_to_utf8(std::wstring(1, map[y][x].letter));
            } else {
                std::cout << ' ';
            }
        }

        std::cout << std::endl;
    }
}

} // namespace debug