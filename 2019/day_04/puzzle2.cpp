#include <iostream>
#include "include/myutils.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Missing input" << endl;
        return EXIT_FAILURE;
    }

    auto input = myutils::read_file(argv[1]);

    auto answer = 42;

    std::cout << "Answer: " << answer << endl;
}
