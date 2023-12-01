//
// Copyright (C) Martin Beaudoin. 2023. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <list>
#include <numeric>
#include <algorithm>
#include <regex>
#include "myutils.h"

using namespace std;

// Solve puzzle #1
template <typename T>
long solve_puzzle1(T data)
{
#if 0
    for(auto var : data)
    {
    	cout<<var<<"\n";
    }
#endif

    long sumCalibrationNumbers = 0L;

    auto toDigit = [](auto c)
        {
            return c - '0';
        };

    auto isDigit = [](auto c)
        {
            return isdigit(c);
        };

    for(auto s : data )
    {
        std::string onlyDigits;

        std::copy_if(s.begin(), s.end(), std::back_inserter(onlyDigits), isDigit);

        if(onlyDigits.size() > 0)
        {
            int calibCode = 10*toDigit(onlyDigits[0]) + toDigit(onlyDigits[onlyDigits.size()-1]);

            // std::cout << "Code: "<< s << " : digits: " << onlyDigits << ": " << calibCode << std::endl;

            sumCalibrationNumbers += calibCode;
        }
    }
    // Return sum
    return sumCalibrationNumbers;
}

// Solve puzzle #2
template <typename T>
long solve_puzzle2(T data)
{
#if 0
    for(auto var : data)
    {
    	cout<<var<<"\n";
    }
#endif

    long sumCalibrationNumbers = 0L;

    auto toDigit = [](auto c)
        {
            return c - '0';
        };

    auto isDigit = [](auto c)
        {
            return isdigit(c);
        };

    for(auto s : data )
    {
        std::string onlyDigits;

        //std::cout << "Original string: " << s << "  --> ";

        // First, replace string instances (combo)
        s = std::regex_replace(s, std::regex("zerone"),    "01");
        s = std::regex_replace(s, std::regex("twone"),     "21");
        s = std::regex_replace(s, std::regex("sevenine"),  "79");
        s = std::regex_replace(s, std::regex("eightwo"),   "82");
        s = std::regex_replace(s, std::regex("eighthree"), "83");
        s = std::regex_replace(s, std::regex("threeight"), "38");
        s = std::regex_replace(s, std::regex("fiveight"),  "58");
        s = std::regex_replace(s, std::regex("oneight"),   "18");

        // Next standalone instances
        s = std::regex_replace(s, std::regex("zero"),  "0");
        s = std::regex_replace(s, std::regex("one"),   "1");
        s = std::regex_replace(s, std::regex("two"),   "2");
        s = std::regex_replace(s, std::regex("three"), "3");
        s = std::regex_replace(s, std::regex("four"),  "4");
        s = std::regex_replace(s, std::regex("five"),  "5");
        s = std::regex_replace(s, std::regex("six"),   "6");
        s = std::regex_replace(s, std::regex("seven"), "7");
        s = std::regex_replace(s, std::regex("eight"), "8");
        s = std::regex_replace(s, std::regex("nine"),  "9");

        // std::cout << "cleaned string: " << s << std::endl;

        std::copy_if(s.begin(), s.end(), std::back_inserter(onlyDigits), isDigit);

        if(onlyDigits.size() > 0)
        {
            int calibCode = 10*toDigit(onlyDigits[0]) + toDigit(onlyDigits[onlyDigits.size()-1]);

            // std::cout << "Code: "<< s << " : digits: " << onlyDigits << ": " << calibCode << std::endl;

            sumCalibrationNumbers += calibCode;
        }
    }
    // Return sum
    return sumCalibrationNumbers;
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
    myutils::read_file(data, filename, true, true, false);

    const vector<string> example = {
        "1abc2",
        "pqr3stu8vwx",
        "a1b2c3d4e5f",
        "treb7uchet"
    };

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1<vector<string>>(example) == 142 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;  // 54644

    const vector<string> example2 = {
        "two1nine",
        "eightwothree",
        "abcone2threexyz",
        "xtwone3four",
        "4nineeightseven2",
        "zoneight234",
        "7pqrstsixteen"
    };

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example2) == 281 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;  // 53348

}
