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
#include <algorithm>
#include "myutils.h"

using namespace std;

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
    map<pair<char, char>, char> pairInsertion;
    map<char, unsigned long long> countElement;

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
    // when trying to reach 40 steps. Let's evaluate using recursion instead

    list<char>::iterator it1 = polymerTemplate.begin();
    list<char>::iterator it2 = it1; it2++;

    countElement[*it1] += 1;

    // Define a recursive lambda function: add element, and recurse new created pair
    std::function<void(char, char, int)> insertElement;
    insertElement = [
        &pairInsertion,
        &countElement,
        &insertElement](char left, char right, int nbrSteps)->void
        {
            // cout << "Steps remaining: " << nbrSteps << endl;

            pair<char, char> curPair = std::make_pair(left, right);
            map<pair<char, char>, char>::iterator ip = pairInsertion.find(curPair);

            if(ip != pairInsertion.end())
            {
                char newElement = ip->second;
                countElement[newElement] += 1;

                if(--nbrSteps > 0)
                {
                    insertElement(left, newElement, nbrSteps);
                    insertElement(newElement, right, nbrSteps);
                }
            }
        };

    // March the template, pair by pair, for n steps
    while(it2 != polymerTemplate.end())
    {
        cout << " Evaluate pair: " << *it1 << ":" << *it2 << endl;
        countElement[*it2] += 1;

        insertElement(*it1, *it2, nbrSteps);

        // Next pair
        it1++; it2++;
    }

    // Find min and max, compute result
    unsigned long long minElement = std::min_element(countElement.begin(), countElement.end(),
        [](const auto& l, const auto& r) { return l.second < r.second; })->second;

    unsigned long long maxElement = std::max_element(countElement.begin(), countElement.end(),
            [](const auto& l, const auto& r) { return l.second < r.second; })->second;

    cout << "minElement: " << minElement << endl;
    cout << "maxElement: " << maxElement << endl;
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
    //assert(solve_puzzle1<vector<string>>(example1, 40, 1) == 2188189693529 && "Error verifying puzzle #2");

    // Solve puzzle #2
    //std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
