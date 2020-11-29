//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include "myutils.h"

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

using namespace std;

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    typedef std::basic_string<unsigned char> ustring;

    // ustring d = data;
    unsigned char d [] = "pqrstuv1048970";
    unsigned char md[1024];

    //d = u"asdfsdfsdf";

    MD5(d);

    cout << "md: " << md << endl;
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
    auto data = myutils::read_file<string, std::vector<string> >(filename);

    cout << data[0] << endl;
    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data[0]) == 42 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data[0]) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const auto example2 = 2;
    assert(solve_puzzle2<vector<int>>({example2}) == 42 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
