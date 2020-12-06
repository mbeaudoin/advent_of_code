//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    int totalYesAnswers = 0;
    set<int> group;

    for(auto d: data)
    {
        if(!d.empty())
        {
            for(auto c: d)
            {
                group.insert(c);
            }
        }
        else
        {
            totalYesAnswers += group.size();
            group.clear();
        }
    }

    return totalYesAnswers;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2(T data)
{
    // We build an histogram, and count the elements where the tally is equal to the number of
    // people in the group
    std::vector<int> histo(26);
    int groupSize = 0;
    int totalYesAnswers = 0;

    for(auto d: data)
    {
        if(!d.empty())
        {
            sort(d.begin(), d.end());

            groupSize++;

            for(auto c: d)
            {
                histo[c - 'a'] += 1;
            }
        }
        else
        {
            // Tally votes from this group
            for(auto v : histo)
            {
                if(v == groupSize) // number of votes == groupSize
                    totalYesAnswers++;
            }

            // Reset histo
            histo.clear();
            histo.resize(26);
            groupSize = 0;
        }
    }

    return totalYesAnswers;
}

// Solve puzzle #2
template <typename T>
int solve_puzzle2_usingSets(T data)
{
    int totalYesAnswers = 0;

    // We use sets and intersections
    std::vector<string> groupVotes;

    for(auto d: data)
    {
        if(!d.empty())
        {
            sort(d.begin(), d.end());
            groupVotes.push_back(d);
        }
        else if(groupVotes.size() > 0)
        {
            string intersection;
            string::iterator it;

            intersection.resize(26);

            intersection = groupVotes[0];

            for(int i=1; i<groupVotes.size(); i++)
            {
                it = std::set_intersection(
                    groupVotes[i].begin(),
                    groupVotes[i].end(),
                    intersection.begin(),
                    intersection.end(),
                    intersection.begin());

                intersection.resize(it-intersection.begin());
            }

            totalYesAnswers += intersection.size();
            groupVotes.clear();
        }
    }

    return totalYesAnswers;
}

template <typename T>
int solve_puzzle2_usingSetsCleaner(T data)
{
    int totalYesAnswers = 0;

    // We use sets and intersections, using data inplace
    string intersection;

    bool newGroup = true;

    for(auto d: data)
    {
        if(!d.empty())
        {
            // set_intersection need sorted arrays
            sort(d.begin(), d.end());

            if(newGroup)
            {
                intersection = d;
                newGroup ^= true;
            }
            else
            {
                string::iterator it = std::set_intersection(
                    d.begin(),
                    d.end(),
                    intersection.begin(),
                    intersection.end(),
                    intersection.begin());

                intersection.resize(it-intersection.begin());
            }
        }
        else
        {
            totalYesAnswers += intersection.size();
            newGroup ^= true;
        }
    }
    return totalYesAnswers;
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
        "abc",
        "",
        "a",
        "b",
        "c",
        "",
        "ab",
        "ac",
        "",
        "a",
        "a",
        "a",
        "a",
        "",
        "b",
        ""
    };

    assert(solve_puzzle1<vector<string>>(example1) == 11 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1) == 6 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;

    // Verify puzzle2 examples using sets and intersections
    assert(solve_puzzle2_usingSets<vector<string>>(example1) == 6 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2 using sets and intersections: "<< solve_puzzle2_usingSets(data) << std::endl;

    // Verify puzzle2 examples using sets and intersections
    assert(solve_puzzle2_usingSetsCleaner<vector<string>>(example1) == 6 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2 using sets and intersections (cleaner): "<< solve_puzzle2_usingSetsCleaner(data) << std::endl;


}
