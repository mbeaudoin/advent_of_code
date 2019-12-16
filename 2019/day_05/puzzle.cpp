//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

#include "Intcode.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, int input, bool debug = false)
{
    Intcode computer(data, input, debug);

    return computer.run();
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, int input, bool debug = false)
{
    Intcode computer(data, input, debug);

    return computer.run();
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
    auto data = myutils::read_file_csv<int, std::vector<int> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data, 1, true) == 7839346 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 1) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    vector<int> v;
    v = {3,9,8,9,10,9,4,9,99,-1,8}; assert(solve_puzzle2(v, 8, true) == 1 && "Error verifying puzzle #2");
    v = {3,9,8,9,10,9,4,9,99,-1,8}; assert(solve_puzzle2(v, 7, true) == 0 && "Error verifying puzzle #2");

    v = {3,9,7,9,10,9,4,9,99,-1,8}; assert(solve_puzzle2(v, 7, true) == 1 && "Error verifying puzzle #2");
    v = {3,9,7,9,10,9,4,9,99,-1,8}; assert(solve_puzzle2(v, 9, true) == 0 && "Error verifying puzzle #2");

    v = {3,3,1108,-1,8,3,4,3,99}; assert(solve_puzzle2(v, 7, true) == 0 && "Error verifying puzzle #2");
    v = {3,3,1108,-1,8,3,4,3,99}; assert(solve_puzzle2(v, 8, true) == 1 && "Error verifying puzzle #2");

    v = {3,3,1107,-1,8,3,4,3,99}; assert(solve_puzzle2(v, 5, true) == 1 && "Error verifying puzzle #2");
    v = {3,3,1107,-1,8,3,4,3,99}; assert(solve_puzzle2(v, 8, true) == 0 && "Error verifying puzzle #2");
    v = {3,3,1107,-1,8,3,4,3,99}; assert(solve_puzzle2(v, 9, true) == 0 && "Error verifying puzzle #2");

    v = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9}; assert(solve_puzzle2(v, 0, true) == 0 && "Error verifying puzzle #2");
    v = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9}; assert(solve_puzzle2(v, 9, true) == 1 && "Error verifying puzzle #2");

    v = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1}; assert(solve_puzzle2(v, 0, true) == 0 && "Error verifying puzzle #2");
    v = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1}; assert(solve_puzzle2(v, 9, true) == 1 && "Error verifying puzzle #2");

    v = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
        1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
        999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    assert(solve_puzzle2(v, 7, true) ==  999 && "Error verifying puzzle #2");
    assert(solve_puzzle2(v, 8, true) == 1000 && "Error verifying puzzle #2");
    assert(solve_puzzle2(v, 9, true) == 1001 && "Error verifying puzzle #2");

    assert(solve_puzzle2(data, 5, true) == 447803 && "Error verifying puzzle #2");


    // Solve puzzle #2
    std::cout << "Answer for puzzle #2 "<< solve_puzzle1(data, 5) << std::endl;
}
