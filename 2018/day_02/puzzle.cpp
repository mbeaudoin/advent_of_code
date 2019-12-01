#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "include/myutils.h"

using namespace std;

// Function for puzzle #2
std::string find_differences(std::string s1, std::string s2)
{
  std::string retValue = "";

  for (int k=0; k< s1.size(); k++)
  {
    if (s1[k] == s2[k])
      retValue.append(1, s1[k]);
  }
  return retValue;

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
    std::vector<std::string> ids;

    ids = myutils::read_file<string, std::vector<string> >(filename);

    // Puzzle #1
    int nbrs2 = 0;
    int nbrs3 = 0;
    for (auto& idd : ids)
    {
        std::map<char, int> char_counter;
        std::set<int>       multiples;

        // count occurences of characters
        for (auto& c : idd)
        {
            char_counter[c] = char_counter[c] + 1;
        }

        // Use a set to find individual totals
        for(auto&p : char_counter)
        {
            multiples.insert(p.second);
            // std::cout << "Key: " << p.first << " : Value: " << p.second << std::endl;
        }

        // Find 2s and 3s
        for(auto&p : multiples)
        {
            // std::cout << "multiples: " << p << std::endl;

            if (p == 2)
                nbrs2++;
            else if (p == 3)
                nbrs3++;
        }
    }
    int checksum = nbrs2 * nbrs3;
    // Report
    // std::cout << "Nbr of 2s: " << nbrs2 << std::endl;
    // std::cout << "Nbr of 3s: " << nbrs3 << std::endl;

    std::cout << "Answer puzzle #1: " << checksum << std::endl;

    // Puzzle #2
    for (int i=0; i<ids.size(); i++)
    {
        for (int j=i; j<ids.size(); j++)
        {
            std::string common_chars = find_differences(ids[i], ids[j]);

            if(common_chars.size() == ids[i].size() - 1)
            {
                std::cout << "Answer puzzle #2: " << common_chars << std::endl;
            }
        }
    }

}
