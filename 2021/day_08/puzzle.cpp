//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <sstream>      // std::stringstream
#include <algorithm>
#include "myutils.h"

using namespace std;

set<char>
convertToSet(string s)
{
    set<char> retValue;
    for(auto c: s)
    {
        retValue.insert(c);
    }
    return retValue;
}

string
convertToString(set<char> s)
{
    string retValue;
    for(auto c : s)
    {
        retValue += c;
    }
    return retValue;
}

void
findSegmentsMapping(vector<string> tests, map<string, int>& segmentPairing)
{
    map<int, set<char> > segments;

    // Find mapping for simpler digits '1', '4', '7' and '8'
    for(auto t : tests)
    {
        if(t.size() == 2)
        {
            segments[1] = convertToSet(t);  // 1
        }
        else if(t.size() == 4)
        {
            segments[4] = convertToSet(t);  // 4
        }
        else if(t.size() == 3)
        {
            segments[7] = convertToSet(t);  // 7
        }
        else if(t.size() == 7)
        {
            segments[8] = convertToSet(t);  // 8
        }
    }

    // Cleanup
    for(auto seg : segments)
    {
        tests.erase(std::remove(tests.begin(), tests.end(), convertToString(seg.second)), tests.end());
    }

    // Find 9 and 0:
    // - only one segment different with digit '8'
    // - and none in common with digit '1' and '4' -- 9
    // - else this is '0'
    set<char> segs8 = segments[8];
    set<char> s_diff_8_vs_9;
    for(auto t: tests)
    {
        set<char> curSet = convertToSet(t);

        s_diff_8_vs_9.clear();

        std::set_difference
            (
                segs8.begin(), segs8.end(),
                curSet.begin(), curSet.end(),
                std::inserter(s_diff_8_vs_9, s_diff_8_vs_9.begin())
            );

        if(  // if one segment difference, and segment is not part of digit '1'
            s_diff_8_vs_9.size() == 1 &&
            segments[1].find(*(s_diff_8_vs_9.begin())) == segments[1].end()
        )
        {
            if( // if segment not in '4' -- > this is digit '9'
                segments[4].find(*(s_diff_8_vs_9.begin())) == segments[4].end()
            )
            {
                segments[9] = convertToSet(t);  // 9
                // cout << "Found 9: "  << convertToString(segments[9]) << endl;
            }
            else  // this is digit '0'
            {
                segments[0] = convertToSet(t);  // 0
                // cout << "Found 0: "  << convertToString(segments[0]) << endl;
            }
            //break;
        }
    }

    // Cleanup
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[0])), tests.end());
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[9])), tests.end());

    // Find digit '6'
    // - Only one remaining with 6 segment
    for(auto t : tests)
    {
        if(t.size() == 6)
        {
            segments[6] = convertToSet(t);  // 6
            // cout << "Found 6: "  << convertToString(segments[6]) << endl;
        }
    }
    // Cleanup
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[6])), tests.end());

    // Find digit '3'
    // -- Only remaining digit with common segments with all segment from digit '1'
    for(auto t : tests)
    {
        set<char> curSet = convertToSet(t);
        set<char> s_common;

        std::set_intersection
            (
                segments[1].begin(), segments[1].end(),
                curSet.begin(), curSet.end(),
                std::inserter(s_common, s_common.begin())
            );

        if(s_common.size() == 2)
        {
            segments[3] = convertToSet(t);  // 3
            // cout << "Found 3: "  << convertToString(segments[3]) << endl;
        }
    }

    // Cleanup
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[3])), tests.end());

    // Find digit '5'
    // - Only one with 5 segments in common with '6'
    for(auto t : tests)
    {
        set<char> curSet = convertToSet(t);
        set<char> s_common;

        s_common.clear();

        std::set_intersection
            (
                segments[6].begin(), segments[6].end(),
                curSet.begin(), curSet.end(),
                std::inserter(s_common, s_common.begin())
            );

        if(s_common.size() == 5)
        {
            segments[5] = convertToSet(t);  // 5
            // cout << "Found 5: "  << convertToString(segments[5]) << endl;
        }
        else
        {
            segments[2] = convertToSet(t);  // 2
            // cout << "Found 2: "  << convertToString(segments[2]) << endl;
        }
    }

    // Cleanup
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[5])), tests.end());
    tests.erase(std::remove(tests.begin(), tests.end(), convertToString(segments[2])), tests.end());

    // Should be empty
    if(!tests.empty())
    {
        cout << " Error in mapping: " << endl;

        for(auto t : tests)
        {
            cout << " t apres: "  << t  << " : "  << t.size() <<endl;
        }
        cout << "---------" << endl;
    }

    // Return reverse map : scrambled string to digit
    for(auto seg : segments)
    {
        segmentPairing[convertToString(seg.second)] = seg.first;
    }

    return;
}

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    typedef std::pair<vector<string>, vector<string>> sevenSegDigitTest;

    vector<sevenSegDigitTest> listTests;

    // Extract data
    for(auto d : data)
    {
        sevenSegDigitTest entry;

        std::stringstream ss(d);
        std::string token;

        for(int i=0; i<10; i++)
        {
            ss >> token;
            entry.first.push_back(token);
        }

        ss >> token; // remove pipe char

        for(int i=0; i<4; i++)
        {
            ss >> token;
            entry.second.push_back(token);
        }

        listTests.push_back(entry);

#if 0
        for(auto t : entry.first)
            cout << t << " ";

        cout << " | ";

        for(auto t : entry.second)
            cout << t << " ";
        cout << endl;
#endif
    }

    // Initialize 7-segment digits
    map<int, string> sevenSegmentDigits;
    sevenSegmentDigits[0] = "abcefg";
    sevenSegmentDigits[1] = "cf";
    sevenSegmentDigits[2] = "acdeg";
    sevenSegmentDigits[3] = "acdfg";
    sevenSegmentDigits[4] = "bcdf";
    sevenSegmentDigits[5] = "abdfg";
    sevenSegmentDigits[6] = "abdefg";
    sevenSegmentDigits[7] = "acf";
    sevenSegmentDigits[8] = "abcdefg";
    sevenSegmentDigits[9] = "abcdfg";

    long sumOfEasyNums = 0;

    for(auto l: listTests)
    {
        for(auto s: l.second)
        {
            if
                (
                    s.size() ==  sevenSegmentDigits[1].size() ||
                    s.size() ==  sevenSegmentDigits[4].size() ||
                    s.size() ==  sevenSegmentDigits[7].size() ||
                    s.size() ==  sevenSegmentDigits[8].size()
                )
                sumOfEasyNums++;
        }
    }

    // cout << "sumOfEasyNums: " << sumOfEasyNums << endl;
    return sumOfEasyNums;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    typedef std::pair<vector<string>, vector<string>> sevenSegDigitTest;

    vector<sevenSegDigitTest> listTests;

    // Extract data
    for(auto d : data)
    {
        sevenSegDigitTest entry;

        std::stringstream ss(d);
        std::string token;

        for(int i=0; i<10; i++)
        {
            ss >> token;
            sort(token.begin(), token.end());
            entry.first.push_back(token);
        }

        ss >> token; // remove pipe char

        for(int i=0; i<4; i++)
        {
            ss >> token;
            sort(token.begin(), token.end());
            entry.second.push_back(token);
        }

        listTests.push_back(entry);

#if 0
        for(auto t : entry.first)
            cout << t << " ";

        cout << " | ";

        for(auto t : entry.second)
            cout << t << " ";
        cout << endl;
#endif
    }

    long sumDigits = 0;
    for(auto l: listTests)
    {
        string fullDigit;
        map<string, int> sevenSegmentMap;

        findSegmentsMapping(l.first, sevenSegmentMap);

        // Dump digits
        for(auto s : l.second)
        {
            fullDigit += std::to_string(sevenSegmentMap[s]);
        }
        // cout << "Full digit: " << fullDigit << endl;

        sumDigits += std::stoi(fullDigit);
    }
    return sumDigits;
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
        "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
        "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
        "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
        "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
        "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
        "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
        "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
        "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
        "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
        "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"
    };
    assert(solve_puzzle1<vector<string>>(example1) == 26 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2<vector<string>>(example1) == 61229 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
