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

bool isReportSafe(vector<int> reportLevels, bool debug)
{

    bool isIncreasing = false;
    bool isDecreasing = false;
    bool isSafe = false;

    auto incr = std::adjacent_find(reportLevels.begin(), reportLevels.end(), std::greater_equal<int>());
    auto decr = std::adjacent_find(reportLevels.begin(), reportLevels.end(), std::less_equal<int>());

    if(incr == reportLevels.end())
    {
        isIncreasing = true;

        if(debug)
            cout << " is increasing";
    }

    else if(decr == reportLevels.end())
    {
        isDecreasing = true;

        if(debug)
            cout << " is decreasing";
    }

    if(isIncreasing || isDecreasing)
    {
        auto ptr = reportLevels.begin();
        auto ptrNext = ptr + 1;

        while(ptrNext != reportLevels.end())
        {
            int delta = abs(*ptr - *ptrNext);
            if(delta > 0 && delta <= 3)
            {
                isSafe = true;
            }
            else
            {
                isSafe = false;
                break;
            }
            ptr++;
            ptrNext++;
        }

        if(debug)
            cout << " : isSafe: " << isSafe;
    }

    if(debug)
        cout << endl;

    return isSafe;
}

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data, bool debug=false)
{
    if(debug)
    {
        for(auto s : data)
            cout<< s <<"\n";
    }

    std::vector<int> reports;
    int nbrSafeReports = 0;

    for(auto report: data)
    {
        std::istringstream is( report );

        string str;

        std::vector<int> levels;

        while (getline(is, str, ' ') )
        {
            int level = stoi(str);
            levels.push_back(level);
        }

        if(debug)
        {
            cout << "Report: " ;
            for(auto r : levels)
                cout << r << ' ';
        }

        if(isReportSafe(levels, debug))
            nbrSafeReports++;
    }

    return nbrSafeReports;
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

    std::vector<int> reports;
    int nbrSafeReports = 0;

    for(auto report: data)
    {
        std::istringstream is( report );

        string str;

        std::vector<int> levels;

        while (getline(is, str, ' ') )
        {
            int level = stoi(str);
            levels.push_back(level);
        }

        if(debug)
        {
            cout << "Report: " ;
            for(auto r : levels)
                cout << r << ' ';
        }

        // Check full report
        if(isReportSafe(levels, debug))
            nbrSafeReports++;
        else
        {
            //Check full report while removing only one level

            if(debug)
                cout << "Checking damped levels" << endl;

            for(int i=0; i<levels.size(); i++)
            {
                std::vector<int> dampedLevels(levels);

                dampedLevels.erase(dampedLevels.begin()+i);

                if(debug)
                {
                    cout << "New Report: " ;
                    for(auto r : dampedLevels)
                        cout << r << ' ';
                }

                if(isReportSafe(dampedLevels, debug))
                {
                    // This report is considered safe, we're done with that one.
                    nbrSafeReports++;
                    break;
                }
            }
        }
    }

    return nbrSafeReports;
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
        "7 6 4 2 1",
        "1 2 7 8 9",
        "9 7 6 2 1",
        "1 3 2 4 5",
        "8 6 4 4 1",
        "1 3 6 7 9"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example, false) == 2 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 54644

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example, false) == 4 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 53348

}
