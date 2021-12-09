//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>      // std::stringstream
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    const vector vowels = {'a', 'e', 'i', 'o', 'u'};

    int nbrNiceString = 0;

    for(auto d : data)
    {
        //cout << d << endl;

        int numVowels = 0;
        for (const char target: {'a', 'e', 'i', 'o', 'u'})
        {
            numVowels += std::count(d.cbegin(), d.cend(), target);
        }
        // cout << "Number vowels: " << numVowels << '\n';

        bool hasDoubleSeq = false;

        auto e = end(d), i = std::adjacent_find(begin(d), e);

        if(i != e)
        {
            hasDoubleSeq = true;
        }
        // cout << "Has doubled letters: " << d << " : "  << hasDoubleSeq << endl;

        int numSpecialTokens = 0;
        for (const string target: {"ab" , "cd", "pq" , "xy"})
        {
            numSpecialTokens += (d.find(target) != string::npos);
        }
        //cout << "Number special tokens: " << numSpecialTokens << endl;

        // Is this string nice?
        if(
            numVowels >=3 &&
            hasDoubleSeq &&
            numSpecialTokens == 0
        )
        {
            nbrNiceString++;
        }
    }
    return nbrNiceString;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int nbrNiceString = 0;

    for(auto d : data)
    {
        // Find non-overlapping pairs
        bool hasNonOverlappingPairs = false;
        for (int i=0; i<d.size()-1; i++)
        {
            std::string subS = d.substr (i,2);

            if(d.find(subS, i+2) !=std::string::npos)
            {
                hasNonOverlappingPairs = true;
                break;
            }
        }

        // Find repeats with one letter between
        bool hasRepatedLetters = false;
        for (int i=0; i<d.size()-2; i++)
        {
            if(d[i] == d[i+2])
            {
                hasRepatedLetters = true;
                break;
            }
        }

        // Is this a nice string?
        if(hasNonOverlappingPairs && hasRepatedLetters)
        {
            nbrNiceString++;
        }
    }

    return nbrNiceString;
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
    const std::vector<std::string> example1 = {
        "ugknbfddgicrmopn",
        "aaa",
        "jchzalrnumimnmhp",
        "haegwjzuvuyypxyu",
        "dvszwmarrgswjxmb"
    };
    assert(solve_puzzle1<vector<string>>(example1) == 2 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const std::vector<std::string> example2 = {
        "qjhvhtzxzqqjkmpb",
        "xxyxx",
        "uurcxstgmygtbstg",
        "ieodomkazucvgmuy"
    };

    assert(solve_puzzle2<vector<string>>(example2) == 2 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
