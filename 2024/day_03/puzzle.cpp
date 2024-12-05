//
// Copyright (C) Martin Beaudoin. 2024. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <numeric>
#include <algorithm>
#include <regex>
#include "myutils.h"

using namespace std;

// DO/DONT state
const int DO = 1;
const int DONT = 0;

// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, bool debug=false)
{
    if(debug)
    {
        for(auto s : data)
            cout<< s <<"\n";
    }

    long long sum = 0L;

    for(auto s : data)
    {
        int index = 0;
        size_t pos = 0;

        while((pos = s.find("mul(", index)) != string::npos)
        {
            index = pos+1;

            string sub = s.substr(pos);

            sub.erase(0, 4);

            istringstream is(sub);

            string mulToken;
            int int1, int2;
            char comma, closingParent;
            is >> int1 >> comma >> int2 >> closingParent;

            if(debug)
            {
                cout << "mulToken: " << mulToken << endl;
                cout << "int1: " << int1 << endl;
                cout << "int2: " << int2 << endl;
                cout << "comma: " << comma << endl;
                cout << "closingParent: " << closingParent << endl;
            }

            if(comma == ',' && closingParent == ')')
            {
                sum += int1 * int2;
            }
        }
    }
    return sum;
}

std::vector<int> markDoDont( std::string s, int& curState)
{
    std::vector<int> doFlags(s.size(), curState);

    int matchPos = 0;
    int index = 0;

    // We start with the 'curState' state, so find the next change

    while((matchPos = s.find("do", index)) != string::npos)
    {
        index = matchPos+1;

        if(std::strncmp(s.c_str()+matchPos, "do()", 4) == 0 && doFlags[matchPos] != DO)
        {
            curState = DO;
            std::for_each(doFlags.begin() + matchPos, doFlags.end(), [](int& v) { v = DO;});
        }
        else if(std::strncmp(s.c_str()+matchPos, "don't()", 7) == 0 && doFlags[matchPos] != DONT)
        {
            curState = DONT;
            std::for_each(doFlags.begin() + matchPos, doFlags.end(), [](int& v) { v = DONT;});
        }
    }
    return doFlags;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data, bool debug = false)
{
    if(debug)
    {
        for(auto s : data)
            cout<< s <<"\n";
    }

    long long sum = 0L;
    int curState = DO;

    for(auto s : data)
    {
        int index = 0;
        size_t matchPos = 0;

        // Create a vector of DO/DONT flags at each character position
        // Once we get to a valid 'mul' operation, we just check this vector
        // at the current 'mul'  position to validate the operation.
        std::vector<int> doDont = markDoDont(s, curState);

        while((matchPos = s.find("mul(", index)) != string::npos)
        {
            index = matchPos+1;

            string sub = s.substr(matchPos);

            sub.erase(0, 4);

            istringstream is(sub);

            string mulToken;
            int int1, int2;
            char comma, closingParent;
            is >> int1 >> comma >> int2 >> closingParent;

            if(debug)
            {
                cout << "mulToken: " << mulToken << endl;
                cout << "int1: " << int1 << endl;
                cout << "int2: " << int2 << endl;
                cout << "comma: " << comma << endl;
                cout << "closingParent: " << closingParent << endl;
            }

            // Same as puzzle #1, with an additional DO/DONT test
            if(comma == ',' && closingParent == ')' && (doDont.at(matchPos) == DO))
            {
                sum += int1 * int2;
            }
        }
    }
    return sum;
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
    std::vector<string> data;
    bool keepSpace = true;
    bool keepEmptyLine = false;
    bool debug = false;

    myutils::read_file(data, filename, keepSpace, keepEmptyLine, debug);

    const std::vector<string> example1 = {
        "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
        };

    const std::vector<string> example2 = {
        "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"
        };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example1) == 161 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 185797128

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example2) == 48 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 89798695
}
