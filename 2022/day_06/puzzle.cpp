//
// Copyright (C) Martin Beaudoin. 2022. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;


// Solve puzzle
template <typename T>
long solve_puzzle(T data, int nConsecutiveChars, bool debug = false)
{
    long firstMarkerIndex = 0L;

    if(debug)
        cout << "Data: " << data << endl;

    for(int i=0; i<data.size() - nConsecutiveChars; i++)
    {
        std::map<char, int> countChar;
        countChar.clear();

        for(int l=0; l<nConsecutiveChars; l++)
        {
            countChar[data[i+l]]++;
        }

        // Needs as many entries as nConsecutiveChars
        if(countChar.size() == nConsecutiveChars)
        {
            // Found it!
            firstMarkerIndex = i + nConsecutiveChars;
            break;
        }
    }

    if(debug)
        cout << "firstMakerIndex : " << firstMarkerIndex << endl;

    return firstMarkerIndex;
}

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T& data, bool debug = false)
{
    int nConsecutiveChars  = 4;
    return solve_puzzle(data, nConsecutiveChars, debug);
}


// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data, bool debug = false)
{
    int nConsecutiveChars = 14;
    return solve_puzzle(data, nConsecutiveChars, debug);
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
    std::string data;

    bool keepSpaces = true;
    bool keepEmptyLines = false;
    myutils::read_file(data, filename, keepSpaces, keepEmptyLines);

    const vector<string> example = {
        "mjqjpqmgbljsphdztnvjfqwrcgsmlb",
        "bvwbjplbgvbhsrlpgdmjqwftvncz",
        "nppdvjthqldpwncqszvftbrmjlhg",
        "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg",
        "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(example[0]) == 7  && "Error verifying puzzle #1, example 1");
    assert(solve_puzzle1(example[1]) == 5  && "Error verifying puzzle #1, example 2");
    assert(solve_puzzle1(example[2]) == 6  && "Error verifying puzzle #1, example 3");
    assert(solve_puzzle1(example[3]) == 10 && "Error verifying puzzle #1, example 4");
    assert(solve_puzzle1(example[4]) == 11 && "Error verifying puzzle #1, example 5");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(example[0]) == 19  && "Error verifying puzzle #1, example 1");
    assert(solve_puzzle2(example[1]) == 23  && "Error verifying puzzle #1, example 2");
    assert(solve_puzzle2(example[2]) == 23  && "Error verifying puzzle #1, example 3");
    assert(solve_puzzle2(example[3]) == 29  && "Error verifying puzzle #1, example 4");
    assert(solve_puzzle2(example[4]) == 26  && "Error verifying puzzle #1, example 5");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
