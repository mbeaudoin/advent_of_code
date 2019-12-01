#include <iostream>
#include <vector>
#include <set>
#include "include/myutils.h"

using namespace std;

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

    auto delta_freq = myutils::read_file<int, std::vector<int> >(argv[1]);

    // Puzzle #1
    auto final_freq = 0;
    for (auto& df : delta_freq)
        final_freq += df;

    std::cout << "Answer puzzle #1: "<< final_freq << std::endl;

    // Puzzle #2
    int cur_freq = 0;
    std::set<int> visited_freq;
    bool found = false;

    while(!found)
    {
        for (auto& df : delta_freq)
	{
            cur_freq += df;
            const bool is_in = visited_freq.find(cur_freq) != visited_freq.end();
            if (is_in)
	    {
                std::cout << "Answer puzzle #2: " << cur_freq << std::endl;
                found = true;
                break;
	    }
            else
	    {
                //std::cout << "Delta_freq: " << df << ": Cur freq: " << cur_freq << std::endl;
                visited_freq.insert(cur_freq);
	    }
	}
    }

}
