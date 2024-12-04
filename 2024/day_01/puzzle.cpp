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

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
#if 0
    for(auto s : data)
    {
    	cout<< s <<"\n";
    }
#endif

    std::vector<long> leftList;
    std::vector<long> rightList;

    for(auto s: data)
    {
        std::istringstream is( s );
        long n1, n2;
        is >> n1 >> n2;
        leftList.push_back(n1);
        rightList.push_back(n2);
    }

    // Sort both arrays
    std::sort (leftList.begin(), leftList.end());
    std::sort (rightList.begin(), rightList.end());

    // Compute total distance
    long totDistance=0;
    for(int i=0; i<leftList.size(); i++)
    {
        totDistance += abs(leftList[i] - rightList[i]);
    }

    // Return sum
    return totDistance;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
#if 0
    for(auto s : data)
    {
    	cout<< s <<"\n";
    }
#endif

    std::vector<long> leftList;
    std::vector<long> rightList;
    std::vector<long> mult;
    for(auto s: data)
    {
        std::istringstream is( s );
        long n1, n2;
        is >> n1 >> n2;
        leftList.push_back(n1);
        rightList.push_back(n2);
    }

    // Compute similarity score
    long long simScore=0L;

    for(auto v: leftList)
    {
        int n = std::count(rightList.begin(), rightList.end(), v);
        simScore += n * v;
    }

    // Return score
    return simScore;
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

    const vector<string> example = {
        "3 4",
        "4 3",
        "2 5",
        "1 3",
        "3 9",
        "3 3"  };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 11 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 54644

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 31 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 53348

}
