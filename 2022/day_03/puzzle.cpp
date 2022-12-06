//
// Copyright (C) Martin Beaudoin. 2022. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <set>
#include <numeric>
#include <algorithm>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
    long priority = 0L;

    for(auto val: data)
    {
        int len = val.size();

        if(len > 0)
        {
            std::string compartment1 = val.substr(0, len/2);
            std::string compartment2 = val.substr(len/2, len/2);

            std::list<char> commonItems;

            std::sort(compartment1.begin(), compartment1.end());
            std::sort(compartment2.begin(), compartment2.end());

            std::set_intersection(
                compartment1.begin(), compartment1.end(),
                compartment2.begin(), compartment2.end(),
                std::back_inserter(commonItems));

            char commonItem = *commonItems.begin();

            if (isupper(commonItem))
            {
                priority += commonItem - 'A' + 1 + 26;
            }
            else
            {
                priority += commonItem - 'a' + 1;
            }
        }
    }

    return priority;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
    long priority = 0L;

    typename T::iterator it1 = data.begin();

    while(it1 < data.end())
    {
        int len = it1->size();

        if(len > 0)
        {
            typename T::iterator it2 = it1+1;
            typename T::iterator it3 = it2+1;

            std::list<char> commonItems12;
            std::list<char> commonItems123;

            std::sort(it1->begin(), it1->end());
            std::sort(it2->begin(), it2->end());
            std::sort(it3->begin(), it3->end());

            std::set_intersection(
                it1->begin(), it1->end(),
                it2->begin(), it2->end(),
                std::back_inserter(commonItems12));

            // First intersection result is sorted.

            std::set_intersection(
                it3->begin(), it3->end(),
                commonItems12.begin(), commonItems12.end(),
                std::back_inserter(commonItems123));

            char commonItem = *commonItems123.begin();

            if (isupper(commonItem))
            {
                priority += commonItem - 'A' + 1 + 26;
            }
            else
            {
                priority += commonItem - 'a' + 1;
            }
        }

        it1 += 3;
    }

    return priority;
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
    myutils::read_file(data, filename, true, true);

    const vector<string> example = {
        "vJrwpWtwJgWrhcsFMMfFFhFp",
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
        "PmmdzqPrVvPwwTWBwg",
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
        "ttgJtRGJQctTZtZT",
        "CrZsJsPPZsGzwwsLwLmpwMDw"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 157 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 70 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
