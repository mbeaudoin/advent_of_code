#include <iostream>
#include <vector>
#include <set>
#include "include/myutils.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Missing input" << endl;
        return EXIT_FAILURE;
    }

    auto delta_freq = myutils::read_file<int, std::vector<int> >(argv[1]);

    // Puzzle #1
    auto answer = 42;
    std::cout << "Answer puzzle #1: "<< answer << std::endl;

    // Puzzle #2
    std::cout << "Answer puzzle #2: "<< answer << std::endl;
}
