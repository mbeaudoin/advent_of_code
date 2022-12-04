//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <map>
#include <numeric>
#include <algorithm>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
    enum RPC {Rock, Paper, Scissors};

    std::map<char, RPC> convertTable;

    // Conversion table
    convertTable['A'] = Rock;
    convertTable['B'] = Paper;
    convertTable['C'] = Scissors;
    convertTable['X'] = Rock;
    convertTable['Y'] = Paper;
    convertTable['Z'] = Scissors;

    // Match results
    enum WLD {Win, Lose, Draw};

    std::map<WLD, int> scores;
    scores[Win]  = 6;
    scores[Draw] = 3;
    scores[Lose] = 0;

    // Rock, Paper, Scissors win/loss
    std::map< std::pair<RPC, RPC>, int> rpsOutcome;

    // The draws
    rpsOutcome[std::make_pair(Rock, Rock)]         = 3;
    rpsOutcome[std::make_pair(Paper, Paper)]       = 3;
    rpsOutcome[std::make_pair(Scissors, Scissors)] = 3;

    // The wins
    rpsOutcome[std::make_pair(Rock, Paper)]     = 6;    // "paper covers rock"
    rpsOutcome[std::make_pair(Paper, Scissors)] = 6;    // "scissors cuts paper")
    rpsOutcome[std::make_pair(Scissors, Rock)]  = 6;    // "rock crushes scissors"

    // The losses
    rpsOutcome[std::make_pair(Paper, Rock)]     = 0;    // "paper covers rock"
    rpsOutcome[std::make_pair(Scissors, Paper)] = 0;    // "scissors cuts paper")
    rpsOutcome[std::make_pair(Rock, Scissors)]  = 0;    // "rock crushes scissors"

    // Item Score
    std::map<RPC, int> itemScore;
    itemScore[Rock] = 1;
    itemScore[Paper] = 2;
    itemScore[Scissors] = 3;

    // Guide
    std::list<std::pair<RPC, RPC>> guide;

    // Evaluate the guide
    long totalScore = 0;

    for(auto var : data)
    {
        if(var.size() > 0)
        {
            // std::cout<<var;

            RPC opponent = convertTable[var[0]];
            RPC me       = convertTable[var[2]];

            // std::cout << " : " << itemScore[me] << " : " << rpsOutcome[std::make_pair(opponent, me)] << std::endl;

            totalScore += itemScore[me] + rpsOutcome[std::make_pair(opponent, me)];
        }
    }

    return totalScore;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
    enum RPC {Rock, Paper, Scissors};

    std::map<char, RPC> convertTable;

    // Conversion table
    convertTable['A'] = Rock;
    convertTable['B'] = Paper;
    convertTable['C'] = Scissors;


    // Match results
    enum WLD {Win, Lose, Draw};

    std::map<char, WLD> expectedOutcome;
    expectedOutcome['X'] = Lose;
    expectedOutcome['Y'] = Draw;
    expectedOutcome['Z'] = Win;


    std::map<WLD, int> scores;
    scores[Win]  = 6;
    scores[Draw] = 3;
    scores[Lose] = 0;

    // Rock, Paper, Scissors win/loss
    std::map< std::pair<RPC, WLD>, RPC> whatDoINeed;

    // The draws
    whatDoINeed[std::make_pair(Rock,     Draw)] = Rock;
    whatDoINeed[std::make_pair(Paper,    Draw)] = Paper;
    whatDoINeed[std::make_pair(Scissors, Draw)] = Scissors;

    // The wins
    whatDoINeed[std::make_pair(Rock,     Win)]  = Paper;    // "paper covers rock"
    whatDoINeed[std::make_pair(Paper,    Win)]  = Scissors; // "scissors cuts paper")
    whatDoINeed[std::make_pair(Scissors, Win)]  = Rock;     // "rock crushes scissors"

    // The losses
    whatDoINeed[std::make_pair(Rock,     Lose)] = Scissors; // "rock crushes scissors"
    whatDoINeed[std::make_pair(Paper,    Lose)] = Rock;     // "paper covers rock"
    whatDoINeed[std::make_pair(Scissors, Lose)] = Paper;    // "scissors cuts paper")

    // Item Score
    std::map<RPC, int> itemScore;
    itemScore[Rock] = 1;
    itemScore[Paper] = 2;
    itemScore[Scissors] = 3;

    // Guide
    std::list<std::pair<RPC, RPC>> guide;

    // Evaluate the guide
    long totalScore = 0;

    for(auto var : data)
    {
        if(var.size() > 0)
        {
            // std::cout<<var;

            RPC opponent    = convertTable[var[0]];
            WLD expectedRes = expectedOutcome[var[2]];

            RPC me = whatDoINeed[std::make_pair(opponent, expectedRes)];

            // std::cout << " : " << itemScore[me] << " : " << scores[expectedRes] << std::endl;

            totalScore += itemScore[me] + scores[expectedRes];
        }
    }

    return totalScore;
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
    myutils::read_file(data, filename, true, true);

    const vector<string> example = {
        "A Y",
        "B X",
        "C Z"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 15 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example) == 12 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
