//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <list>
#include <map>
#include "myutils.h"

#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

typedef map<string, unsigned long long> Ingredient;

typedef struct Reaction
{
    string     name;
    unsigned long long  prodMin;
    unsigned long long  leftOvers;
    Ingredient input;

   Reaction()
        : name(""), prodMin(0ull), leftOvers(0ull)
        {
            input.clear();
        }

    Reaction(const Reaction& r)
        : name(r.name), prodMin(r.prodMin), leftOvers(r.leftOvers), input(r.input)
        {}

} Reaction;

std::ostream& operator<<(std::ostream& os, const Ingredient& ingredient)
{
    for(auto i : ingredient)
        os << i.second << " " << i.first << " + ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Reaction& r)
{
    return os << r.input << " => " << r.prodMin << " " << r.name ;
}

Reaction
extractReaction(string& str)
{
    Reaction r;

    std::string delimiter = "=>";

    // Cleanup: remove possible ',' separator from multiple input ingredients
    str.erase(std::remove(str.begin(), str.end(), ','), str.end());

    // Find input and output
    string input  = str.substr(0, str.find(delimiter));
    string output = str.substr(str.find(delimiter) + delimiter.size(), str.size());

    istringstream inputStream(input);
    istringstream outputStream(output);

    // Output tokens
    vector<string> tokensOut{
        istream_iterator<string>{outputStream},
        istream_iterator<string>{}};

    // Input tokens
    vector<string> tokensIn{
        istream_iterator<string>{inputStream},
        istream_iterator<string>{}};

    if(tokensOut.size() != 2)
    {
        cout << ">>>>> Error parsing recipe. Output string: " << output << endl;
    }

    // Convert output tokens
    r.name = tokensOut.back();
    tokensOut.pop_back();
    r.prodMin = stoull(tokensOut.back());

    // Convert input tokens
    while(tokensIn.size())
    {
        string name;
        unsigned long long qty;

        name = tokensIn.back();
        tokensIn.pop_back();
        qty = stoull(tokensIn.back());
        tokensIn.pop_back();

        r.input[name] = qty;
    }

    return r;
}

unsigned long long computeNeedeORE(string ingredientName, unsigned long long quantity, map<string, Reaction>& recipe, bool debug = false)
{
    unsigned long long neededORE = 0;

    // Find the reaction for this element
    Reaction mainR = recipe[ingredientName];

    // Find the minimal quantity for this element
    unsigned long long minqtyOutput = mainR.prodMin;

    if(debug)
        cout << "computeNeedeORE: need to produce: " << quantity << "  " << mainR.name << " (minimum batch): " << mainR.prodMin << endl;

    if(ingredientName == "ORE")
        neededORE = quantity;
    else
    {
        // We ask for quantity of product. The minimum we can produce at a time is minqtyOutput
        unsigned long long qtyProduce = 0ull;
        unsigned long long multipleOfMinBatch = 1ull;

        // First, check in the leftovers
        if(recipe[ingredientName].leftOvers >= quantity)
        {
            qtyProduce = 0ull;
            recipe[ingredientName].leftOvers -= quantity;
        }

        // Need to produce new ingredient
        else if(quantity <= minqtyOutput)
        {
            qtyProduce = minqtyOutput;
            recipe[ingredientName].leftOvers += qtyProduce - quantity;
        }
        else if(quantity % minqtyOutput == 0ull)   // Exact multiple of a minimum batch. No leftovers. Multiply input accordingly
        {
            multipleOfMinBatch = quantity/minqtyOutput;
            qtyProduce = multipleOfMinBatch * minqtyOutput;

        }
        else  // We will have leftovers
        {
            multipleOfMinBatch = quantity/minqtyOutput + 1ull;
            qtyProduce = multipleOfMinBatch * minqtyOutput;
            recipe[ingredientName].leftOvers += qtyProduce - quantity;

            // Did we overshoot based on available leftovers?
            if(recipe[ingredientName].leftOvers >= minqtyOutput)
            {
                recipe[ingredientName].leftOvers -= minqtyOutput;
                qtyProduce -= minqtyOutput;
                multipleOfMinBatch--;
            }
        }

        if(debug)
            cout << "qtyProduce for : " << ingredientName << " : " << qtyProduce << endl;

        if(qtyProduce)
        {
            // Iterate over list of input
            for(auto& r : mainR.input)
            {
                unsigned long long qtyInput = multipleOfMinBatch * r.second;

                if(debug)
                    cout << "Input: Need " << qtyInput << "  " << r.first << " batch of: " << r.second << " for : " << qtyProduce << " " << ingredientName << endl;

                neededORE += computeNeedeORE(r.first, qtyInput, recipe);
            }
        }
    }
    if(debug)
    {
        cout << "Element: " << ingredientName << " need " << neededORE << " ORE   .";
        cout << "Leftovers: " ;
        for(auto r : recipe)
            cout << r.first << "=" << r.second.leftOvers << " ";
        cout << endl;
    }
    return neededORE;
}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data, int numberOfFuelNeeded = 1, bool debug = false)
{
    map<string, Reaction> recipe;  // tuple name : Reaction
    for(auto s : data)
    {
        Reaction r = extractReaction(s);
        recipe[r.name] = r;
    }

    string basicReaction("1 ORE => 1 ORE");
    Reaction r = extractReaction(basicReaction);
    recipe[r.name] = r;

    if(debug)
    {
        for(auto r : recipe)
            cout << "Reaction: " << r.second << endl;
    }
    int nbrORE = computeNeedeORE("FUEL", numberOfFuelNeeded, recipe);

    if(debug)
        cout << "Number of ORE needed: " << nbrORE << endl;

    return nbrORE;
}

// Solve puzzle #2
template <typename T>
unsigned long long solve_puzzle2(T data, bool debug = false)
{
    // Extract all reactions
    map<string, Reaction> recipe;  // tuple name : Reaction
    for(auto s : data)
    {
        Reaction r = extractReaction(s);
        recipe[r.name] = r;
    }

    string basicReaction("1 ORE => 1 ORE");
    Reaction r = extractReaction(basicReaction);
    recipe[r.name] = r;

    unsigned long long cargo = 1000000000000ull;
    unsigned long long numberOfFuelProduced = 0ull;
    unsigned long long production = 0ull;

    // Rapidely find the nearest block of 100000, then slice this bracket by 10, until we converge to a unit bracket
    unsigned long long bracket = 100000ull;

    do
    {
        production = 0ull;

        while(production <= cargo)
        {
            numberOfFuelProduced += bracket;
            production = computeNeedeORE("FUEL", numberOfFuelProduced, recipe);

            if(debug)
                cout << numberOfFuelProduced << " FUEL will need: " << production << " ORE" << endl;
        }

        // We've exceed the current bracket, moving back one bracket, and using a smaller bracket
        numberOfFuelProduced -= bracket;
        bracket /= 10ull;

    } while(bracket >= 1ull);

    if(debug)
        cout << "Number of Fuel produced: " << numberOfFuelProduced << endl;

    return numberOfFuelProduced;
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
    bool keepWhiteSpace = true;
    auto data = myutils::read_file<string, std::vector<string> >(filename, keepWhiteSpace);  // Keep spaces

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    const vector<string> example1 =
    {
        "10 ORE => 10 A",
        "1 ORE => 1 B",
        "7 A, 1 B => 1 C",
        "7 A, 1 C => 1 D",
        "7 A, 1 D => 1 E",
        "7 A, 1 E => 1 FUEL"
    };
    assert(solve_puzzle1(example1) == 31 && "Error verifying puzzle #1");

    const vector<string> example2 =
    {
        "9 ORE => 2 A",
        "8 ORE => 3 B",
        "7 ORE => 5 C",
        "3 A, 4 B => 1 AB",
        "5 B, 7 C => 1 BC",
        "4 C, 1 A => 1 CA",
        "2 AB, 3 BC, 4 CA => 1 FUEL"
    };
    assert(solve_puzzle1(example2) == 165 && "Error verifying puzzle #1");

    const vector<string> example3 =
    {
        "157 ORE => 5 NZVS",
        "165 ORE => 6 DCFZ",
        "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL",
        "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ",
        "179 ORE => 7 PSHF",
        "177 ORE => 5 HKGWZ",
        "7 DCFZ, 7 PSHF => 2 XJWVT",
        "165 ORE => 2 GPVTF",
        "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT"
    };
    assert(solve_puzzle1(example3) == 13312 && "Error verifying puzzle #1");

    const vector<string> example4 =
    {
        "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG",
        "17 NVRVD, 3 JNWZP => 8 VPVL",
        "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL",
        "22 VJHF, 37 MNCFX => 5 FWMGM",
        "139 ORE => 4 NVRVD",
        "144 ORE => 7 JNWZP",
        "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC",
        "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV",
        "145 ORE => 6 MNCFX",
        "1 NVRVD => 8 CXFTF",
        "1 VJHF, 6 MNCFX => 4 RFSQX",
        "176 ORE => 6 VJHF"
    };
    assert(solve_puzzle1(example4) == 180697 && "Error verifying puzzle #1");

    const vector<string> example5 =
    {
        "171 ORE => 8 CNZTR",
        "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL",
        "114 ORE => 4 BHXH",
        "14 VRPVC => 6 BMBT",
        "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL",
        "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT",
        "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW",
        "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW",
        "5 BMBT => 4 WPTQ",
        "189 ORE => 9 KTJDG",
        "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP",
        "12 VRPVC, 27 CNZTR => 2 XDBXC",
        "15 KTJDG, 12 BHXH => 5 XCVML",
        "3 BHXH, 2 VRPVC => 7 MZWV",
        "121 ORE => 7 VRPVC",
        "7 XCVML => 6 RJRHP",
        "5 BHXH, 4 VRPVC => 5 LTCX"
    };
    assert(solve_puzzle1(example5) == 2210736 && "Error verifying puzzle #1");

    // Solve puzzle #1
    assert(solve_puzzle1(data) == 397771 && "Error verifying puzzle #1");
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(example3) == 82892753ull && "Error verifying puzzle #2");
    assert(solve_puzzle2(example4) == 5586022ull && "Error verifying puzzle #2");
    assert(solve_puzzle2(example5) == 460664ull && "Error verifying puzzle #2");
    assert(solve_puzzle2(data)     == 3126714ull && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
