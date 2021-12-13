//
// Copyright (C) Martin Beaudoin. 2021. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>      // std::stringstream
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, bool revisitSmallCave, int debug = 0)
{
    multimap<string, string> tree;
    for(auto s : data)
    {
        std::stringstream ss(s);
        std::string token1;
        std::string token2;
        std::getline(ss, token1, '-');
        std::getline(ss, token2, '-');

        tree.insert(pair <string, string> (token1, token2));
        tree.insert(pair <string, string> (token2, token1));

        if(debug)
            cout << "Tokens: " << token1 << " --- " << token2 << endl;
    }

    if(debug)
    {
        for(auto t: tree)
        {
            cout << "Tree[" << t.first << "]: " << t.second << endl;
        }
    }

    // lambda function: is string all uppercase?
    auto isUpperCase = [](string &s) ->bool {
        return find_if(s.begin(), s.end(), [](char c){return islower(c); }) == s.end();
    };

    // lambda function: count number of occurences for 'entry' in vector 'v'
    auto countNumberOfEntry = [](vector<string> &v, string entry) ->int {
        int nbrEntry = 0;
        for(auto e: v)
        {
            if(e == entry)
            {
                nbrEntry++;
            }
        }
        return nbrEntry;
    };


    // Recursive lambda function: add node to path and recurse from there
    std::function<void(string, vector<string>, bool, int&)> addNode;
    addNode = [
        &tree,
        &isUpperCase,
        &countNumberOfEntry,
        debug,
        &addNode](string node, vector<string> path, bool revisitSmallCave, int& nbrValidPaths)->void
        {
            if(debug)
                cout << "Adding node: " << node << endl;

            path.push_back(node);

            if(node != "end" && node != "start")
            {
                auto nextNodes = tree.equal_range(node);

                for(auto it = nextNodes.first; it != nextNodes.second; it++)
                {
                    // No more than 2 visits to only one small cave
                    if(
                        revisitSmallCave &&
                        !isUpperCase(node) &&
                        countNumberOfEntry(path, node) == 2
                    )
                    {
                        revisitSmallCave = false;
                    }

                    if(
                        find(path.begin(), path.end(), it->second) == path.end() ||
                        revisitSmallCave ||
                        isUpperCase(it->second)
                    )
                    {
                        addNode(it->second, path, revisitSmallCave, nbrValidPaths);
                    }
                }
            }
            else if(node == "end")
            {
                nbrValidPaths++;

                if(debug)
                {
                    cout << "New path: ";
                    for(auto p : path)
                    {
                        cout << p << ',';
                    }
                    cout << endl;
                }
            }
        };

    // Start at 'start'
    auto node = tree.equal_range("start");

    int nbrValidPaths = 0;
    for(auto it = node.first; it != node.second; it++)
    {
        if(debug)
            cout << it->first << "-" << it->second << endl;

        vector<string> path;
        path.push_back(it->first);
        addNode(it->second, path, revisitSmallCave, nbrValidPaths);
    }
    return nbrValidPaths;
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
    const vector<string> example1_1 = {
        "start-A",
        "start-b",
        "A-c",
        "A-b",
        "b-d",
        "A-end",
        "b-end"
    };
    const vector<string> example1_2 = {
        "dc-end",
        "HN-start",
        "start-kj",
        "dc-start",
        "dc-HN",
        "LN-dc",
        "HN-end",
        "kj-sa",
        "kj-HN",
        "kj-dc"
    };
    const vector<string> example1_3 = {
        "fs-end",
        "he-DX",
        "fs-he",
        "start-DX",
        "pj-DX",
        "end-zg",
        "zg-sl",
        "zg-pj",
        "pj-he",
        "RW-he",
        "fs-DX",
        "pj-RW",
        "zg-RW",
        "start-pj",
        "he-WI",
        "zg-he",
        "pj-fs",
        "start-RW"
    };

    bool revisitSmallCave = false;
    assert(solve_puzzle1<vector<string>>(example1_1, revisitSmallCave) == 10 && "Error verifying puzzle #1-1");
    assert(solve_puzzle1<vector<string>>(example1_2, revisitSmallCave) == 19 && "Error verifying puzzle #1-2");
    assert(solve_puzzle1<vector<string>>(example1_3, revisitSmallCave) == 226 && "Error verifying puzzle #1-3");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, revisitSmallCave) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle1 examples with new rules
    revisitSmallCave = true;
    assert(solve_puzzle1<vector<string>>(example1_1, revisitSmallCave) == 36 && "Error verifying puzzle #2");
    assert(solve_puzzle1<vector<string>>(example1_2, revisitSmallCave) == 103 && "Error verifying puzzle #2");
    assert(solve_puzzle1<vector<string>>(example1_3, revisitSmallCave) == 3509 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle1(data, revisitSmallCave) << std::endl;
}
