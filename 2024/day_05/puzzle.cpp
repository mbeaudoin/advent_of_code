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


// Specialized function object for sorting vector of ints using arbitrary rules
struct myFunctionObject
{
    std::list<std::pair<int, int>> _rules;  // first is always smaller than second

    // Comparator function
    bool operator() (int i,int j)
    {
        bool retValue = false;
        for(auto r: _rules)
        {
            if(r.first == i && r.second == j)
            {
                retValue = true;
                break;
            }
        }
        return (retValue);
    }

    // Constructor
    myFunctionObject(std::list<std::pair<int, int>> sortRules) : _rules(sortRules)
    {
    }
};


// Solve puzzle #1
template <typename T>
long long solve_puzzle1(T data, bool debug=false)
{
    if(debug)
    {
        for(auto s : data)
            std::cout<< s <<"\n";
    }

    std::list<std::pair<int, int>> rules;
    std::list<std::vector<int>>    updates;

    // Start my memorizing rules, then switch to updates after blank line
    bool memorizeRules = true;
    for(auto s : data)
    {
        if(s.size() == 0)
        {
            memorizeRules = false;
        }
        else if(memorizeRules)
        {
            std::string token;
            std::istringstream iss(s);
            std::pair<int, int> p;
            std::getline(iss, token, '|');
            p.first = stoi(token);
            std::getline(iss, token, '|');
            p.second = stoi(token);
            rules.push_back(p);
        }
        else
        {
            std::istringstream iss(s);
            std::string token;
            std::vector<int> update;

            while (std::getline(iss, token, ','))
            {
                update.push_back(stoi(token));
            }
            updates.push_back(update);
        }
    }

    // Initialize comparator
    myFunctionObject myComparator(rules);

    long sumMiddlePageNumbers = 0;

    for(auto u: updates)
    {
        if(debug)
        {
            std::cout << "Update: ";
            for(auto e: u)
            {
                std::cout << e << ",";
            }
            std::cout << endl;
        }

        // Clone current update, and sort it
        std::vector<int> sorted_u(u);

        std::sort(sorted_u.begin(), sorted_u.end(), myComparator);

        if(sorted_u == u)
        {
            if(debug)
            {
                std::cout << "This sequence is well ordered: ";
                for(auto e: u)
                    std::cout << e << ", ";
                std::cout << endl;
            }

            sumMiddlePageNumbers += u[u.size()/2];
        }
    }

    return sumMiddlePageNumbers;
}


// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data, bool debug = false)
{
    if(debug)
    {
        for(auto s : data)
            std::cout<< s <<"\n";
    }

    std::list<std::pair<int, int>> rules;
    std::list<std::vector<int>>    updates;

    // Start my memorizing rules, then switch to updates after blank line
    bool memorizeRules = true;
    for(auto s : data)
    {
        if(s.size() == 0)
        {
            memorizeRules = false;
        }
        else if(memorizeRules)
        {
            std::string token;
            std::istringstream iss(s);
            std::pair<int, int> p;
            std::getline(iss, token, '|');
            p.first = stoi(token);
            std::getline(iss, token, '|');
            p.second = stoi(token);
            rules.push_back(p);
        }
        else
        {
            std::istringstream iss(s);
            std::string token;
            std::vector<int> update;

            while (std::getline(iss, token, ','))
            {
                update.push_back(stoi(token));
            }
            updates.push_back(update);
        }
    }

    // Initialize comparator
    myFunctionObject myComparator(rules);

    long sumMiddlePageNumbers = 0;

    for(auto u: updates)
    {
        if(debug)
        {
            std::cout << "Update: ";
            for(auto e: u)
            {
                std::cout << e << ",";
            }
            std::cout << endl;
        }

        // Clone current update, and sort it
        std::vector<int> sorted_u(u);

        std::sort(sorted_u.begin(), sorted_u.end(), myComparator);

        if(sorted_u != u)
        {
            if(debug)
            {
                std::cout << "This sequence is badly ordered: ";
                for(auto e: u)
                    std::cout << e << ", ";
                std::cout << endl;
            }

            sumMiddlePageNumbers += sorted_u[sorted_u.size()/2];
        }
    }

    return sumMiddlePageNumbers;
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
    bool keepEmptyLine = true;
    bool debug = false;

    myutils::read_file(data, filename, keepSpace, keepEmptyLine, debug);

    const std::vector<string> example = {
        "47|53",
        "97|13",
        "97|61",
        "97|47",
        "75|29",
        "61|13",
        "75|53",
        "29|13",
        "97|29",
        "53|29",
        "61|53",
        "97|53",
        "61|29",
        "47|13",
        "75|47",
        "97|75",
        "47|61",
        "75|61",
        "47|29",
        "75|13",
        "53|13",
        "",
        "75,47,61,53,29",
        "97,61,53,29,13",
        "75,29,13",
        "75,97,47,61,53",
        "61,13,29",
        "97,13,75,29,47"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 143 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 5129

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 123 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 4077
}
