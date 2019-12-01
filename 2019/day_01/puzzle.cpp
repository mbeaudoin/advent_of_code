#include <iostream>
#include <vector>
#include <set>
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
    auto tot_fuel = 0;
    for (auto& module_mass : data)
        tot_fuel += compute_fuel(module_mass);

    std::cout << "Answer puzzle #1: "<< tot_fuel << std::endl;

    // Puzzle #2
    tot_fuel = 0;
    for (auto& module_mass : data)
    {
        auto this_module_fuel = compute_fuel(module_mass);

        while ( this_module_fuel > 0)
        {
            tot_fuel += this_module_fuel;
            this_module_fuel = compute_fuel(this_module_fuel);
        }
    }

    std::cout << "Answer puzzle #2: "<< tot_fuel << std::endl;
}
