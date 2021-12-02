//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;

typedef enum
{
    FORWARD,
    UP,
    DOWN
} direction;

// Solve puzzle #1
template <typename T>
constexpr long solve_puzzle1(T data)
{
    std::map<string, direction> mapDirection = {
        {"forward", FORWARD},
        {"up",      UP},
        {"down",    DOWN}
    };

    long coord_x = 0;
    long coord_y = 0;

    for(auto str: data)
    {
        std::string dir;
        int distance;

        std::stringstream strstr(str);
        strstr >> dir >> distance;

        switch(mapDirection[dir])
        {
            case FORWARD:
                coord_x += distance;
                break;
            case UP:
                coord_y -= distance;
                break;
            case DOWN:
                coord_y += distance;
                break;
        }
    }
    return coord_x * coord_y;
}

// Solve puzzle #2
template <typename T>
constexpr long solve_puzzle2(T data)
{
    std::map<string, direction> mapDirection = {
        {"forward", FORWARD},
        {"up",      UP},
        {"down",    DOWN}
    };

    long coord_x = 0;
    long coord_y = 0;
    long aim     = 0;

    for(auto str: data)
    {
        std::string dir;
        int distance;

        std::stringstream strstr(str);
        strstr >> dir >> distance;

        switch(mapDirection[dir])
        {
            case FORWARD:
                coord_x += distance;
                coord_y += aim * distance;
                break;
            case UP:
                aim -= distance;
                break;
            case DOWN:
                aim += distance;
                break;
        }
    }

    return coord_x * coord_y;
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
    const auto example1 = {
        "forward 5",
        "down 5",
        "forward 8",
        "up 3",
        "down 8",
        "forward 2"
        };

    assert(solve_puzzle1(example1) == 150 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(example1) == 900 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
