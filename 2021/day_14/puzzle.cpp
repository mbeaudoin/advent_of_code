//
// Copyright (C) Martin Beaudoin. 2021. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <map>
#include <unordered_map>
#include <sstream>      // std::stringstream
#include <climits>
#include <algorithm>
#include "myutils.h"

using namespace std;

typedef unsigned long long ullong;

// Solve puzzle #1
template <typename T>
constexpr unsigned long long solve_puzzle1(T data, int nbrSteps, int debug=0)
{
    if(debug)
    {
        cout << "Initial data: " << endl;
        for(auto d: data)
            cout << d << endl;
    }

    list<char> polymerTemplate;
    map<pair<char, char>, char> pairInsertionRule;
    map<pair<char, char>, ullong> pairCounter[nbrSteps+1];

    for(auto c: data[0])
    {
        polymerTemplate.push_back(c);
    }

    for(int i=1; i<data.size(); i++)
    {
        std::stringstream ss(data[i]);
        string ruleLeft, ruleRight, scrap;

        ss >> ruleLeft >> scrap >> ruleRight;

        pairInsertionRule[{ruleLeft[0], ruleLeft[1]}] = ruleRight[0];

        pairCounter[0][{ruleLeft[0], ruleLeft[1]}] = 0;
    }

    // Initialize pairCounter at step 0
    list<char>::iterator it1 = polymerTemplate.begin();
    list<char>::iterator it2 = it1; it2++;

    while(it2 != polymerTemplate.end())
    {
        pairCounter[0][{*it1, *it2}]++;
        // Next pair
        it1++; it2++;
    }

    // Iterate through nSteps, counting pairs
    for(int  i=1; i<=nbrSteps; i++)
    {
        for(auto &p : pairCounter[i-1])
        {
            ullong nbrNewpair = p.second;
            char newElem = pairInsertionRule[{p.first.first,p.first.second}];

            // Create new pairs using new element
            pairCounter[i][{p.first.first,newElem}] += nbrNewpair;
            pairCounter[i][{newElem, p.first.second}] += nbrNewpair;
        }
    }

    // Tally nbr of elements
    map<char, ullong> countElement;

    // Count left-most element from sequence
    countElement[polymerTemplate.front()] = 1;

    // Count right-most element of all pairs
    for(auto &p : pairCounter[nbrSteps])
    {
        countElement[p.first.second] += p.second;
    }

    if(debug)
    {
        for(auto &c : countElement)
        {
            cout << "Element: " << c.first << " : "  << c.second << endl;
        }
    }

    // Compute min/max
    unsigned long long maxElement = max_element(countElement.begin(), countElement.end(),
        [](const auto& l, const auto& r) { return l.second < r.second; })->second;

    unsigned long long minElement = min_element(countElement.begin(), countElement.end(),
        [](const auto& l, const auto& r) { return l.second < r.second; })->second;

    return maxElement - minElement;
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
    const vector<string> example1 = {
        "NNCB",
        "BC -> B",
        "BN -> B",
        "CH -> B",
        "HC -> B",
        "NB -> B",
        "NC -> B",
        "CN -> C",
        "HB -> C",
        "HN -> C",
        "NH -> C",
        "NN -> C",
        "BH -> H",
        "CB -> H",
        "BB -> N",
        "CC -> N",
        "HH -> N"
    };
    assert(solve_puzzle1<vector<string>>(example1, 10, 0) == 1588 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 10, 0) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle1<vector<string>>(example1, 40, 0) == 2188189693529 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle1(data, 40) << std::endl;
}
