#include <iostream>
#include <cassert>
#include <vector>
#include <numeric>
#include <math.h>       /* floor */

#include "include/myutils.h"

using namespace std;

int compute_fuel(int module_mass)
{
    // Fuel cannot be negative
    return std::max(0, static_cast<int>(floor(module_mass/3.0)) - 2) ;
}

// Solve puzzle #1
template <typename T>
constexpr int solve_puzzle1(T data)
{
    // Puzzle #1
    auto accum_fuel_module_only = [](int sum, int val)
    {
        return std::move(sum) + compute_fuel(val);
    };

    int tot_fuel1 = std::accumulate(
        data.begin(),
        data.end(),
        0,
        accum_fuel_module_only
    );

    return tot_fuel1;
}

// Solve puzzle #2
template <typename T>
constexpr int solve_puzzle2(T data)
{
    auto accum_fuel_module_and_fuel = [](int sum, int val)
    {
        int this_module_fuel = val;
        do
        {
            this_module_fuel = compute_fuel(this_module_fuel);
            sum += this_module_fuel;
        }
        while (this_module_fuel > 0);

        return std::move(sum);
    };

    int tot_fuel2 = std::accumulate(
        data.begin(),
        data.end(),
        0,
        accum_fuel_module_and_fuel);

    return tot_fuel2;
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
    auto data = myutils::read_file<int, std::vector<int> >(filename);

    // --------- Puzzle #1 ---------
    // Verify examples
    assert(solve_puzzle1<vector<int>>({12})     == 2     && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({14})     == 2     && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({1969})   == 654   && "Error verifying puzzle #1");
    assert(solve_puzzle1<vector<int>>({100756}) == 33583 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify examples
    assert(solve_puzzle2<vector<int>>({14})     == 2     && "Error verifying puzzle #2");
    assert(solve_puzzle2<vector<int>>({1969})   == 966   && "Error verifying puzzle #2");
    assert(solve_puzzle2<vector<int>>({100756}) == 50346 && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
