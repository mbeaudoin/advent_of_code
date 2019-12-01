#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>       /* floor */

#include "include/myutils.h"

using namespace std;

int compute_fuel(int module_mass)
{
    return int(floor(float(module_mass)/3.0)) - 2 ;
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

    std::cout << "Answer puzzle #1: "<< tot_fuel1 << std::endl;

    // Puzzle #2
    auto accum_fuel_module_and_fuel = [](int sum, int val)
    {
        auto this_module_fuel = compute_fuel(val);

        while (this_module_fuel > 0)
        {
            sum += this_module_fuel;
            this_module_fuel = compute_fuel(this_module_fuel);
        }
        return std::move(sum);
    };

    int tot_fuel2 = std::accumulate(
        data.begin(),
        data.end(),
        0,
        accum_fuel_module_and_fuel);

    std::cout << "Answer puzzle #2: "<< tot_fuel2 << std::endl;
}
