//
// Copyright (C) Martin Beaudoin. 2019. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include "myutils.h"

using namespace std;

const int imageWidth = 25;
const int imageHeight = 6;
const int nbpPixPerLayer = imageWidth * imageHeight;

typedef array<int, nbpPixPerLayer> imageLayer;

template <typename T>
void
extractImageLayers(T data, vector<imageLayer>& image)
{
    int nbrLayers = data[0].size() / (imageWidth*imageHeight);

    image.resize(nbrLayers);
    int nLayer = 0;
    for(int k=0; k<data[0].size();)
    {
        for(int i = 0; i<nbpPixPerLayer; i++)
        {
            // Convert ASCII to integer
            image[nLayer][i] = data[0][k++] - '0';
        }
        nLayer++;
    }

#if 0
    cout << "nbrLayers: " << nbrLayers << endl;

    for(auto layer : image)
    {
        for(auto p : layer)
        {
            cout << p;
        }
        cout << endl << endl;
    }
#endif
    return;
}

vector<int>
calcNbrPixels(vector<imageLayer> image, int pixValue)
{
    vector<int> pixNbrPerLayer;

    for(auto layer : image)
    {
        int sum = count(layer.begin(), layer.end(), pixValue);
        pixNbrPerLayer.push_back(sum);
    }

#if 0
    cout << "nbr pixvalue: " << pixValue << endl;
    for(auto n : pixNbrPerLayer)
        cout << n << " : " ;
    cout << endl;
#endif

    return pixNbrPerLayer;
}

// Solve puzzle #1
template <typename T>
int solve_puzzle1(T data)
{
    // Extract layers
    vector<imageLayer> image;
    extractImageLayers(data, image);

    vector<int> nbrZeros = calcNbrPixels(image, 0);
    vector<int> nbrOnes = calcNbrPixels(image, 1);
    vector<int> nbrTwos = calcNbrPixels(image, 2);

    // Find layer with minimal number of zeros
    int indexMinZero = distance(nbrZeros.begin(), std::min_element(nbrZeros.begin(), nbrZeros.end()));

    // Return answer
    return nbrOnes[indexMinZero] * nbrTwos[indexMinZero];
}

// Solve puzzle #2
template <typename T>
string solve_puzzle2(T data)
{
    // Extract layers
    vector<imageLayer> image;
    extractImageLayers(data, image);

    const int transparentPixel = 2;

    // Use reverse iterator for handling layers from bottom to top (backward)
    vector<imageLayer>::reverse_iterator rit = image.rbegin();

    imageLayer renderedImage = *rit;

#if 0
    for(auto p : renderedImage)
        cout << p << ":";
    cout << endl;
#endif

    // Backward stepping through all layers, handling transparent pixels
    for (; rit!= image.rend(); ++rit)
    {
        for(int i=0; i<nbpPixPerLayer; i++)
        {
            int pixel = (*rit)[i];
            if(pixel != transparentPixel)
            {
                renderedImage[i] = pixel;
            }
        }
    }

    // Render image using dots and spaces
    cout << endl;
    int k = 0;
    for(int i=0; i<nbpPixPerLayer; i++)
    {
        cout << (renderedImage[k++] == 1 ? "."  : " ");
        cout << ((i+1) % imageWidth == 0 ? "\n" : "");
    }
    cout << endl;

    return "KFABY";
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
    auto data = myutils::read_file<string, std::vector<string> >(filename);

    // --------- Puzzle #1 ---------
    // Verify puzzle1 examples
    assert(solve_puzzle1(data) == 1716 && "Error verifying puzzle #1");

    // Solve puzzle #1
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(data) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    assert(solve_puzzle2(data) == "KFABY" && "Error verifying puzzle #2");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(data) << std::endl;
}
