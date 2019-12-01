#include <iostream>
#include <cassert>
#include <vector>
#include "include/myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    return 42;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    return 42;
}

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

    // Verify puzzle1 examples
    const auto example1 = 42;
    assert(solve_puzzle1(example1) == 42 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // Verify puzzle2 examples
    const auto example2 = 42;
    assert(solve_puzzle2(example2) == 42 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
