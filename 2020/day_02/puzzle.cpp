//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"
#include <sstream>
#include <string>

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    int nbrValidPasswd = 0;
    for(auto s : data)
    {
        int minNumber;
        int maxNumber;
        char charMustBePresent;
        string passwd;
        char delim;

        std::stringstream ss(s);
        ss >> minNumber >> delim >> maxNumber >> charMustBePresent >> delim >> passwd;

        int c = std::count(passwd.begin(), passwd.end(), charMustBePresent);

        if(c >= minNumber && c <= maxNumber)
            nbrValidPasswd++;
    }
    return nbrValidPasswd;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int nbrValidPasswd = 0;

    for(auto s : data)
    {
        int pos1;
        int pos2;
        char charMustBePresent;
        string passwd;
        char delim;

        std::stringstream ss(s);
        ss >> pos1 >> delim >> pos2 >> charMustBePresent >> delim >> passwd;

        if((passwd[pos1-1] == charMustBePresent) ^ (passwd[pos2-1] == charMustBePresent))
            nbrValidPasswd++;
    }
    return nbrValidPasswd;
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
    auto data = myutils::read_file<string, std::vector<string> >(filename, true);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const std::vector<std::string> example = {
        "1-3 a: abcde",
        "1-3 b: cdefg",
        "2-9 c: ccccccccc"
    };

    assert(solve_puzzle1<vector<string>>({example}) == 2 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>({example}) == 1 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
