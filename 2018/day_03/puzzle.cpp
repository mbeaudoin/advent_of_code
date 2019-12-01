#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "include/myutils.h"

using namespace std;

struct patch {
    int number;
    int offset_x;
    int offset_y;
    int size_x;
    int size_y;

    friend std::istream & operator >>(std::istream & in, patch & p)
    {
        char c;
        in >> c >> p.number >> c >> p.offset_x >> c >> p.offset_y >> c >> p.size_x >> c >> p.size_y ;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& os, const patch& p)
    {
        return os << "entry: " << p.number
            << " : offset_x: " << p.offset_x
            << " offset_y: " << p.offset_y
            << " size_x: " << p.size_x
            << " size_y: " << p.size_y;
    }
};

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

    // Reading the data
    std::vector<patch> listPatch = myutils::read_file<patch, std::vector<patch> >(filename);

    // Puzzle #1
    // Puzzle #1
    int maxSizeX = 0;
    int maxSizeY = 0;

    for (auto& p : listPatch)
    {
        int sizeX = p.offset_x + p.size_x;
        int sizeY = p.offset_y + p.size_y;

        if(sizeX > maxSizeX)
            maxSizeX = sizeX;

        if(sizeY > maxSizeY)
            maxSizeY = sizeY;
    }

    // std::cout << "maxSizeX: "<< maxSizeX << std::endl;
    // std::cout << "maxSizeY: "<< maxSizeY << std::endl;

    int cloth[maxSizeX][maxSizeY];
    // std::cout << "Size of cloth: " << sizeof(cloth) << std::endl;

    std::fill( &cloth[0][0], &cloth[0][0] + maxSizeX*maxSizeY, 0 );

    // Fill cloth with patches
    for (auto& p : listPatch)
    {
        for (int x=p.offset_x; x<p.offset_x + p.size_x; x++)
        {
            for (int y=p.offset_y; y<p.offset_y + p.size_y; y++)
            {
                cloth[x][y] += 1;
            }
        }

    }

    // Compute common area
    int commonArea = 0;
    for(int x=0; x< maxSizeX; x++)
    {
        for (int y=0; y< maxSizeY; y++)
        {
            if (cloth[x][y] > 1)
            {
                commonArea++;
            }
        }
    }

    std::cout << "Answer puzzle #1: " << commonArea << std::endl;

    // Puzzle #2
    // intact claim

    for (auto& p : listPatch)
    {
        bool overLap = false;

        for (int x=p.offset_x; x<p.offset_x + p.size_x; x++)
        {
            for (int y=p.offset_y; y<p.offset_y + p.size_y; y++)
            {
                if(cloth[x][y] != 1)
                {
                    overLap = true;
                }
            }
        }

        if(!overLap)
        {
            std::cout << "Answer puzzle #2: " << p.number << std::endl;
        }

    }

}
