//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <stack>
#include <sstream>      // std::stringstream
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, T& incompleteChunks, bool debug=false)
{
    // Build map for open/close characters
    map<char, char> closeToOpen;
    closeToOpen[')'] = '(';
    closeToOpen[']'] = '[';
    closeToOpen['}'] = '{';
    closeToOpen['>'] = '<';

    map<char, int> score;
    score[')'] = 3;
    score[']'] = 57;
    score['}'] = 1197;
    score['>'] = 25137;

    stack<char> chunks;

    long totalScore = 0;

    for(auto d : data)
    {
        bool foundCorruption = false;
        for(auto c: d)
        {
            if(!foundCorruption)
            {
                switch(c)
                {
                    case '(':
                    case '[':
                    case '{':
                    case '<':
                        chunks.push(c);
                        break;
                    case ')':
                    case ']':
                    case '}':
                    case '>':
                        {
                            if(chunks.top() == closeToOpen[c])
                            {
                                chunks.pop();
                            }
                            else
                            {
                                // Corrupted line
                                totalScore += score[c];
                                foundCorruption = true;

                                if(debug)
                                {
                                    cout << " Corrupted entry: " << c
                                    << " for chunk: "  << d << endl;
                                }
                            };
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if(!foundCorruption)
        {
            incompleteChunks.push_back(d);
        }
    }

    return totalScore;
}

// Solve puzzle #2
template <typename T>
constexpr long solve_puzzle2(T data, bool debug = false)
{
    // Build map for open/close characters
    map<char, char> openToClose;
    openToClose['('] = ')';
    openToClose['['] = ']';
    openToClose['{'] = '}';
    openToClose['<'] = '>';

    map<char, int> score; // Using open chunks because already available
    score['('] = 1;
    score['['] = 2;
    score['{'] = 3;
    score['<'] = 4;

    vector<char> chunks;

    vector<long> listOfScores;

    for(auto d : data)
    {
        chunks.clear();

        if(debug)
            cout << d << endl;

        bool foundCorruption = false;
        for(auto c: d)
        {
            switch(c)
            {
                case '(':
                case '[':
                case '{':
                case '<':
                    chunks.push_back(c);
                    break;
                case ')':
                case ']':
                case '}':
                case '>':
                    chunks.pop_back();
                    break;
                default:
                    break;
            }
        }

        if(debug)
        {
            cout << " Remaining chunks: " ;
            for(auto c: chunks)
                cout << c;
            cout << endl;
        }

        // Evaluate score
        long totalScore = 0;

        // Empty chunks in LIFO order, evaluating score as we go
        while (!chunks.empty())
        {
            if(debug)
                cout << openToClose[chunks.back()] << " : "  << score[chunks.back()];

            totalScore = totalScore*5 + score[chunks.back()];
            chunks.pop_back();

            if(debug)
                cout << " : " << totalScore << endl;
        }
        if(debug)
            cout << "TotalScore: " << totalScore << endl;

        // Memorize score for sorting
        listOfScores.push_back(totalScore);
    }

    // Sort in decreasing order
    sort(listOfScores.begin(), listOfScores.end(), greater <>());

    if(debug)
    {
        int counter = 0;
        cout << endl;
        for(auto s : listOfScores)
            cout << ++counter << " : " << s << endl;
    }

    // Return  middle score
    return listOfScores[listOfScores.size()/2];
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
        "[({(<(())[]>[[{[]{<()<>>",
        "[(()[<>])]({[<{<<[]>>(",
        "{([(<{}[<>[]}>{[]{[(<()>",
        "(((({<>}<{<{<>}{[]{[]{}",
        "[[<[([]))<([[{}[[()]]]",
        "[{[{({}]{}}([{[{{{}}([]",
        "{<[[]]>}<{[{[{[]{()[[[]",
        "[<(<(<(<{}))><([]([]()",
        "<{([([[(<>()){}]>(<<{{",
        "<{([{{}}[<[[[<>{}]]]>[]]"
    };

    vector<string> incompleteChunksDemo;
    assert(solve_puzzle1<vector<string>>(example1, incompleteChunksDemo) == 26397 && "Error verifying puzzle #1");

    // Solve puzzle #1
    vector<string> incompleteChunks;
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, incompleteChunks) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const auto example2 = "2";
    assert(solve_puzzle2<vector<string>>(incompleteChunksDemo) == 288957 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(incompleteChunks) << std::endl;
}
