//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <regex>
#include "myutils.h"

using namespace std;

typedef std::tuple<int, string> Bags;
typedef std::multimap<string, Bags> bagRegulations;
typedef std::multimap<string, string> containedBy;
typedef bagRegulations::iterator bIterator;
typedef containedBy::iterator cIterator;

std::ostream& operator<<(std::ostream& os, const Bags& b)
{
    return os << "|" <<std::get<0>(b)
        << "|"
        << std::get<1>(b)
        << "|";
}

int traverseContained(containedBy &contained, string& colour, set<string>& resultSet)
{
    resultSet.insert(colour);

    std::pair<cIterator, cIterator> result = contained.equal_range(colour);

    for (cIterator it = result.first; it != result.second; it++)
    {
        resultSet.insert(it->second);

        traverseContained(contained, it->second, resultSet);
    }

    return resultSet.size();

}

int
countMyBags(bagRegulations& bagReg, string& colour)
{
    int nbrBags = 0;

    std::pair<bIterator, bIterator> result = bagReg.equal_range(colour);

    for (bIterator it = result.first; it != result.second; it++)
        nbrBags += std::get<0>(it->second) * (1 + countMyBags(bagReg, std::get<1>(it->second)));

    return nbrBags;

}
template <typename T>
void processBagRules(T& data, bagRegulations& reg, containedBy &contained)
{
    for(auto d : data)
    {
        if(!d.empty())
        {
            string splitEntry = " bags contain ";
            string containerBag = d.substr(0, d.find(splitEntry));
            string content = d.substr(d.find(splitEntry) + splitEntry.length());

            // Remove " bag", " bags" junk
            content = std::regex_replace (content, std::regex("\\b( bag)([ s.]*)"), "");
            content = std::regex_replace (content, std::regex(",  "), ",");

            stringstream contains(content);
            string bagInfo;

            while(getline(contains, bagInfo, ','))
            {
                stringstream bagStr(bagInfo);
                int nbr;
                string colour;

                bagStr >> nbr;
                getline(bagStr, colour, ',');

                // cleanup
                if(colour.find(' ') == 0)
                    colour.erase(0, 1);

                // Memorize container/content mmap
                Bags b = std::make_tuple(nbr, colour);
                reg.insert({containerBag, b});

                // Memorize contained/container mmap
                contained.insert({colour, containerBag});
            }
        }
    }
}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T& data)
{
    bagRegulations reg;
    containedBy    contained;
    string initColour("shiny gold");
    set<string> resultSet;

    processBagRules(data, reg, contained);

    return traverseContained(contained, initColour, resultSet) - 1; // Remove the shiny gold envelope
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    bagRegulations reg;
    containedBy    contained;
    string initColour("shiny gold");

    processBagRules(data, reg, contained);

    return countMyBags(reg, initColour);
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
        "light red bags contain 1 bright white bag, 2 muted yellow bags.",
        "dark orange bags contain 3 bright white bags, 4 muted yellow bags.",
        "bright white bags contain 1 shiny gold bag.",
        "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.",
        "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.",
        "dark olive bags contain 3 faded blue bags, 4 dotted black bags.",
        "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.",
        "faded blue bags contain no other bags.",
        "dotted black bags contain no other bags."
    };

    assert(solve_puzzle1(example1) == 4 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const vector<string> example2 = {
        "shiny gold bags contain 2 dark red bags.",
        "dark red bags contain 2 dark orange bags.",
        "dark orange bags contain 2 dark yellow bags.",
        "dark yellow bags contain 2 dark green bags.",
        "dark green bags contain 2 dark blue bags.",
        "dark blue bags contain 2 dark violet bags.",
        "dark violet bags contain no other bags."
    };

    assert(solve_puzzle2(example1) == 32 && "Error verifying puzzle #2 with example1");
    assert(solve_puzzle2(example2) == 126 && "Error verifying puzzle #2 with example2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
