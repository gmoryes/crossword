#include <iostream>
#include <vector>
#include <string>

#include "Generator/Generator.h"

int main() {

    //std::vector<std::wstring> words = {L"qwert", L"qwdfg", L"asdrt"};
    std::vector<std::wstring> words = {L"misha", L"masha", L"mashina", L"windows", L"car", L"neighbour"};

    generate_crossword(words);

    return 0;
}