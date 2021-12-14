//
// Copyright (C) Martin Beaudoin. 2021. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <map>
#include <sstream>      // std::stringstream
#include <climits>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr long solve_puzzle1(T data, int nbrSteps, int debug=0)
{
    if(debug)
    {
        cout << "Initial data: " << endl;
        for(auto d: data)
            cout << d << endl;
    }

    list<char> polymerTemplate;
    map<pair<char, char>, char> pairInsertion;
    map<char, long> countElement;

    for(auto c: data[0])
    {
        polymerTemplate.push_back(c);
    }

    for(int i=1; i<data.size(); i++)
    {
        std::stringstream ss(data[i]);
        string ruleLeft, ruleRight, scrap;

        ss >> ruleLeft >> scrap >> ruleRight;

        pair<char, char> elementPair = std::make_pair(ruleLeft[0], ruleLeft[1]);
        pairInsertion[elementPair] = ruleRight[0];

        // Initialize counter
        countElement[ruleLeft[0]] = 0;
        countElement[ruleLeft[1]] = 0;
        countElement[ruleRight[0]] = 0;
    }

    // Since the growing of the polymer is exponential in size, we will blow up
    // when trying to reach 40 steps. Since every pair of the initial template
    // can be grown independantly, let's grow every single initial pair individually
    // and recombine the total of elements along the way.

    list<char>::iterator p_it1 = polymerTemplate.begin();
    list<char>::iterator p_it2 = p_it1; p_it2++;

    // March the template, pair by pair, for n steps
    while(p_it2 != polymerTemplate.end())
    {
        // Grow this pair of the polymer
        list<char> polyPair;
        polyPair.push_back(*p_it1);
        polyPair.push_back(*p_it2);

        cout << "Evaluate pair: " << *p_it1 << " : "  << *p_it2 << endl;

        for(int i=0; i<nbrSteps; i++)
        {
            cout << "Step# : "  << i+1 << endl;
            list<char>::iterator it1 = polyPair.begin();
            list<char>::iterator it2 = it1; it2++;

            while(it2 != polyPair.end())
            {
                pair<char, char> curPair = std::make_pair(*it1, *it2);

                map<pair<char, char>, char>::iterator ip = pairInsertion.find(curPair);

                if(ip != pairInsertion.end())
                {
                    //cout << "Inserting: " << ip->second << endl;
                    polyPair.insert(it2, ip->second);
                    it1++;
                }
                it1++, it2++;
            }
        }

        // Count number of elements from this pair
        // Need to handle correctly the second member of the pair
        // So we don't count it twice on the next pair evaluation
        for(auto p: polyPair)
            countElement[p] += 1;

        // Since the last element will be counted again in next evaluation,
        // let's remove it
        countElement[*p_it2] -= 1;

        // Next pair
        p_it1++; p_it2++;
    }

    countElement[polymerTemplate.back()] += 1;

    // Find min and max, compute result
    long maxElement = INT_MIN;
    long minElement = INT_MAX;

    for(auto c : countElement)
    {
        maxElement = std::max(maxElement, c.second);
        minElement = std::min(minElement, c.second);
    }

    cout << "Result: "  << maxElement - minElement << endl;
    return maxElement - minElement;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data, int debug=0)
{
    if(debug)
    {
        cout << "Initial data: " << endl;
        for(auto d: data)
            cout << d << endl;
    }
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
    vector<string> data;
    myutils::read_file(data, filename, true, false);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const vector<string> example1 = {
        "NNCB",
        "CH -> B",
        "HH -> N",
        "CB -> H",
        "NH -> C",
        "HB -> C",
        "HC -> B",
        "HN -> C",
        "NN -> C",
        "BH -> H",
        "NC -> B",
        "NB -> B",
        "BN -> B",
        "BB -> N",
        "BC -> B",
        "CC -> N",
        "CN -> C"
    };
    assert(solve_puzzle1<vector<string>>(example1, 10, 1) == 1588 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, 10, 1) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle1<vector<string>>(example1, 40, 1) == 2188189693529 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
