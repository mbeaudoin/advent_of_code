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
    unsigned char md[MD5_DIGEST_LENGTH];
    bool found = false;
    long counter = 0;
    while(!found)
    {
        string newKey = data + std::to_string(counter++);

        MD5
        (
            (const unsigned char*)(newKey.c_str()),
            newKey.size(),
            md
        );

        // NB: hex is 4 bits...
        if(md[0] == 0 && md[1] == 0 && md[2] <= 16)
        {
            cout << "Found hash: " << newKey << " : " ;

            for(int i=0; i <MD5_DIGEST_LENGTH; i++)
            {
                printf("%02x",md[i]);
            }
            cout << " : Counter: "  << --counter << endl;

            found = true;
        }
    }

    return counter;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    unsigned char md[MD5_DIGEST_LENGTH];
    bool found = false;
    long counter = 0;
    while(!found)
    {
        string newKey = data + std::to_string(counter++);

        MD5
        (
            (const unsigned char*)(newKey.c_str()),
            newKey.size(),
            md
        );

        // NB: hex is 4 bits...
        if(md[0] == 0 && md[1] == 0 && md[2] == 0)
        {
            cout << "Found hash: " << newKey << " : " ;

            for(int i=0; i <MD5_DIGEST_LENGTH; i++)
            {
                printf("%02x",md[i]);
            }
            cout << " : Counter: "  << --counter << endl;

            found = true;
        }
    }

    return counter;
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
    assert(solve_puzzle1(string("abcdef")) == 609043 && "Error verifying puzzle #1");
    assert(solve_puzzle1(string("pqrstuv")) == 1048970 && "Error verifying puzzle #1");
    solve_puzzle1(data[0]);

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data[0]) << std::endl;

    // --------- Puzzle #2 ---------
    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data[0]) << std::endl;
}
