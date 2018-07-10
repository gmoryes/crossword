#include <iostream>
#include <vector>
#include <string>

#include "Generator/Generator.h"

int main() {

    //std::vector<std::wstring> words = {L"qwert", L"qwdfg", L"asdrt"};
    //std::vector<std::wstring> words = {L"misha", L"masha", L"mashina", L"windows", L"car", L"neighbour", L"movement", L"hzhwhaba"};
    std::vector<std::wstring> words = {L"ярмарка", L"реформа", L"рада", L"стрельцы", L"самозванство",
                                       L"слобода", L"уложение", L"раскол", L"кормление", L"приказы", L"мануфактура"};

    generate_crossword(words);

    return 0;
}