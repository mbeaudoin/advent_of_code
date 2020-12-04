//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, int slope_right, int slope_down)
{
    int nbrTrees = 0;

    int patternLength = data[0].length();

    int x_offset = slope_right;  // offset computed using module(patternLength)

    // NB: Skip first line
    for(int i=slope_down; i< data.size(); i+=slope_down)
    {
        if(data[i][x_offset % patternLength] == '#')
            nbrTrees++;

        x_offset +=slope_right;
    }

    return nbrTrees;
}

// Solve puzzle #2
template <typename T>
constexpr unsigned long solve_puzzle2(T data)
{
    unsigned long tot = 1;
    tot *= solve_puzzle1(data, 1, 1);
    tot *= solve_puzzle1(data, 3, 1);
    tot *= solve_puzzle1(data, 5, 1);
    tot *= solve_puzzle1(data, 7, 1);
    tot *= solve_puzzle1(data, 1, 2);

    return tot;
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
    vector<string> data;
    myutils::read_file(data, filename, true, false);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const vector<string> example = {
        "..##.......",
        "#...#...#..",
        ".#....#..#.",
        "..#.#...#.#",
        ".#...##..#.",
        "..#.##.....",
        ".#.#.#....#",
        ".#........#",
        "#.##...#...",
        "#...##....#",
        ".#..#...#.#",
    };

    assert(solve_puzzle1<vector<string>>(example, 3, 1) == 7 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 3, 1) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 336 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
