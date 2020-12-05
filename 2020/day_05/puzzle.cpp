//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <functional>   // std::multiplies
#include <numeric>      // std::adjacent_difference
#include "myutils.h"

using namespace std;

std::pair<int, int>
findRowCol(string& d, vector<int>& rows, vector<int>& columns)
{
    int row = 0;
    int col = 0;

    string::iterator moveRow = d.begin();
    string::iterator moveCol = d.begin() + 7;

    vector<int>::iterator firstRow = rows.begin();
    vector<int>::iterator lastRow = rows.end();

    vector<int>::iterator firstCol = columns.begin();
    vector<int>::iterator lastCol = columns.end();

    while(*moveRow == 'F' || *moveRow == 'B')
    {
        int jump = std::distance(firstRow, lastRow) / 2;

        if(*moveRow == 'F')
        {
            lastRow -= jump;
        }
        else
        {
            firstRow += jump;
        }

        //cout << "Row: "  << *moveRow << "  : distance: "  << jump << endl;
        moveRow++;
    }

    while(*moveCol == 'L' || *moveCol == 'R')
    {
        int jump = std::distance(firstCol, lastCol) / 2;

        if(*moveCol == 'L')
        {
            lastCol -= jump;
        }
        else
        {
            firstCol += jump;
        }

        // cout << "Col: "  << *moveCol << "  : distance: "  << jump << endl;
        moveCol++;
    }

    row = *firstRow;
    col = *firstCol;

    // cout << "Row: "  << row << "  : col: "  << col << endl;

    return std::make_pair(row, col);
}

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data, vector<int>& listOfIDs, int nbrRows, int nbrColumns, bool debug=false)
{
    int highestSeatID = 0;
    listOfIDs.clear();

    std::vector<int> rows(nbrRows);
    std::vector<int> columns(nbrColumns);

    // Generate row and column values
    std::generate(rows.begin(), rows.end(), [n = 0] () mutable { return n++; });
    std::generate(columns.begin(), columns.end(), [n = 0] () mutable { return n++; });

    // Find [row, col] for this ticket
    for(auto d: data)
    {
        if(!d.empty())
        {
            std::pair<int, int> row_col = findRowCol(d, rows, columns);

            int seatID = row_col.first * 8 + row_col.second;
            listOfIDs.push_back(seatID);

            if(debug)
            {
                cout << d
                    << ": row " << row_col.first
                    << ", column "  << row_col.second
                    << ", seat ID " << seatID
                    << endl;
            }
            highestSeatID = std::max(seatID, highestSeatID);
        }
    }

    return highestSeatID;
}

// Solve puzzle #2
int solve_puzzle2(vector<int>& listOfIDs)
{
    int myID = 0;

    // Sort the id list
    std::sort (listOfIDs.begin(), listOfIDs.end());
    std::vector<int> diffs(listOfIDs.size());

    // Compute adjacent difference
    std::adjacent_difference (listOfIDs.begin(), listOfIDs.end(), diffs.begin());

    myID = listOfIDs.front() + distance(diffs.begin(), std::find(diffs.begin(), diffs.end(), 2));

#if 0
    // I still prefer this form.... Looks simpler
    for(vector<int>::iterator id = listOfIDs.begin(); id != listOfIDs.end(); id++)
    {
        if(*(id+1) - *id == 2)
        {
            myID = *id + 1;
            break;
        }
    }
#endif

    return myID;
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
    const std::vector<string> example1 = {
        "FBFBBFFRLR",
        "BFFFBBFRRR",
        "FFFBBBFRRR",
        "BBFFBBFRLL"
    };

    std::vector<int> listOfIDS;

    assert(solve_puzzle1<vector<string>>(example1, listOfIDS, 128, 8) == 820 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data, listOfIDS, 128, 8) << std::endl;

    // --------- Puzzle #2 ---------

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(listOfIDS) << std::endl;
}
