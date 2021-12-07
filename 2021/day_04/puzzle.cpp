//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <sstream>      // std::stringstream

#include "myutils.h"

using namespace std;

class BingoBoard
{
private:
    int          boardId_;
    bool         foundBingo_;
    vector<int>  boardNumbers_;
    vector<bool> bingoMatch_ = std::vector<bool>(25, false);

public:
    // Default constructor
    BingoBoard() {};

    // Copy constructor
    BingoBoard(const BingoBoard &b)
    {
        boardId_      = b.boardId_;
        foundBingo_   = b.foundBingo_;
        boardNumbers_ = b.boardNumbers_;
        bingoMatch_   = b.bingoMatch_;
    };

    int getId()
    {
        return boardId_;
    }

    // Initialize from a list of string, 5 digit per string
    void init(const int id, const vector<string>& boardNumbers)
    {
        boardId_ = id;
        foundBingo_ = false;

        // Convert board lines into vector of int
        for(int i=0; i<boardNumbers.size(); i++)
        {
            std::stringstream ss(boardNumbers[i]);
            while(!ss.eof())
            {
                string token;
                ss >> token;
                boardNumbers_.push_back(stoi(token));
            }
        }
    }

    void checkNumber(int n)
    {
        int index = 0;

        if(!foundBingo_)
        {
            for(auto b: boardNumbers_)
            {
                if(n == b)
                {
                    bingoMatch_[index] = true;
                }
                index++;
            }
        }
    }

    bool foundBingo()
    {
        bool newBingo = false;

        if(!foundBingo_)
        {
            // Check rows and columns
            for(int i=0; i<5; i++)
            {
                // Rows
                if (
                    bingoMatch_[i*5]   &&
                    bingoMatch_[i*5+1] &&
                    bingoMatch_[i*5+2] &&
                    bingoMatch_[i*5+3] &&
                    bingoMatch_[i*5+4]
                )
                {
                    foundBingo_ = true;
                    newBingo = true;
                    break;
                }

                // Columns
                if(
                    bingoMatch_[i]    &&
                    bingoMatch_[i+5]  &&
                    bingoMatch_[i+10] &&
                    bingoMatch_[i+15] &&
                    bingoMatch_[i+20])
                {
                    foundBingo_ = true;
                    newBingo = true;
                    break;
                }
            }
        }

        return newBingo;
    }

    long sumUnmarkedNumbers()
    {
        int sumUnmarkedNum = 0;

        for(int i = 0; i<25; i++)
        {
            if(!bingoMatch_[i])
            {
                sumUnmarkedNum += boardNumbers_[i];
            }
        }
        return sumUnmarkedNum;
    }

    friend ostream& operator<<(ostream& os, const BingoBoard& board);

};

ostream& operator<<(ostream& os, const BingoBoard& board)
{
    int counter=0;

    cout << " BoardId: " << board.boardId_ << endl;
    cout << " foundBingo: " << board.foundBingo_ << endl;

    for(auto b: board.boardNumbers_)
    {
        cout << setw(3) << b ;
        counter++;

        if(counter % 5 == 0)
            cout << endl;
        if(counter % 25 == 0)
            cout << endl;

    }
    for(auto b: board.bingoMatch_)
    {
        cout << setw(3) << b ;

        counter++;

        if(counter % 5 == 0)
            cout << endl;
        if(counter % 25 == 0)
            cout << endl;
    }
    cout << endl;

    return os;
}


// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    int score = 0;
    int nBoards = (data.size() - 1)/5;

    vector<int>  draws;

    // Convert first data line (draws) to vector of int
    std::stringstream ss(data[0]);
    std::string token;

    while(std::getline(ss, token, ','))
    {
        draws.push_back(stoi(token));
    }

    vector<BingoBoard> boards(nBoards);

    // Initialize list of boards
    for(int i=0; i< nBoards; i++)
    {
        typename T::const_iterator first = data.begin() + 1 + i*5;
        typename T::const_iterator last =  first + 5;
        T boardNums(first, last);
        boards[i].init(i, boardNums);
    }

    // Check bingo after each draw
    bool bingo = false;

    for(auto d : draws)
    {
        // Over every boards
        for(auto & b: boards)
        {
            b.checkNumber(d);

            if(b.foundBingo())
            {
                score = d * b.sumUnmarkedNumbers();
                bingo = true;
                break;
            }
        }
        if(bingo)
            break;
    }
    return score;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    int score = 0;
    int nBoards = (data.size() - 1)/5;

    vector<int>  draws;

    // Convert first data line (draws) to vector of int
    std::stringstream ss(data[0]);
    std::string token;

    while(std::getline(ss, token, ','))
    {
        draws.push_back(stoi(token));
    }

    vector<BingoBoard> boards(nBoards);

    // Initialize list of boards
    for(int i=0; i< nBoards; i++)
    {
        typename T::const_iterator first = data.begin() + 1 + i*5;
        typename T::const_iterator last =  first + 5;
        T boardNums(first, last);
        boards[i].init(i, boardNums);
    }

    int lastWiningScores = 0;

    // Check bingo after each draw
    for(auto d : draws)
    {
        // update and check
        for(auto &b : boards)
        {
            b.checkNumber(d);

            if(b.foundBingo())
            {
                lastWiningScores = d * b.sumUnmarkedNumbers();
            }
        }
    }
    return lastWiningScores;
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
    const std::vector<std::string> example1 = {
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1",

        "22 13 17 11  0",
        "8  2 23  4 24",
        "21  9 14 16  7",
        " 6 10  3 18  5",
        " 1 12 20 15 19",

        " 3 15  0  2 22",
        " 9 18 13 17  5",
        "19  8  7 25 23",
        "20 11 10 24  4",
        "14 21 16 12  6",

        "14 21 17 24  4",
        "10 16 15  9 19",
        "18  8 23 26 20",
        "22 11 13  6  5",
        " 2  0 12  3  7"
    };

    assert(solve_puzzle1<vector<string>>(example1) == 4512 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1) == 1924 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
