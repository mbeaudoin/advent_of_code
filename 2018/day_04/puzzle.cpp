#include <iostream>
#include <vector>
#include <set>
#include "include/myutils.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Need a command-line parameter for the input filename
    if (argc != 2) {
        cerr << "Error: missing input" << endl;
        return EXIT_FAILURE;
    }

    string filename(argv[1]);

    // This file needs to exist
    if (! myutils::file_exists(filename))
    {
        cerr << "Error: nonexistent file: " << filename << endl;
        return EXIT_FAILURE;
    }

    // Reading the data
    auto data = myutils::read_file<int, std::vector<int> >(filename);

    // Puzzle #1
    auto answer = 42;
    std::cout << "Answer puzzle #1: "<< answer << std::endl;

    // Puzzle #2
    std::cout << "Answer puzzle #2: "<< answer << std::endl;
}
