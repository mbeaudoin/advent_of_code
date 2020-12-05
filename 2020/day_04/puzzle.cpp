//
// Copyright (C) Martin Beaudoin. 2020. All Rights Reserved.
//
// See the repository's LICENSE file for the full license details.
//

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <map>

#include "myutils.h"

using namespace std;

typedef enum
{
    BYR,
    IYR,
    EYR,
    HGT,
    HCL,
    ECL,
    PID,
    CID
} passportEntries;

class Passport
{
private:
    map<passportEntries, string> _data;

    bool verifyBYR()
        {
            int val = stoi(_data[BYR]);
            return val >= 1920 && val <= 2002;
        }

    bool verifyIYR()
        {
            int val = stoi(_data[IYR]);
            return val >= 2010 && val <= 2020;
        }

    bool verifyEYR()
        {
            int val = stoi(_data[EYR]);
            return val >= 2020 && val <= 2030;
        }

    bool verifyHGT()
        {
            bool isOk = false;

            string strValue = _data[HGT];
            size_t pos;

            pos = strValue.find("cm");
            if(pos != string::npos)
            {
                strValue.erase(pos, 2);
                int val = stoi(strValue);
                isOk = (val >= 150 && val <= 193);
            }
            else
            {
                pos = strValue.find("in");
                if(pos != string::npos)
                {
                    strValue.erase(pos, 2);
                    int val = stoi(strValue);
                    isOk = (val >= 59 && val <= 76);
                }
            }
            return isOk;
        }

    bool verifyHCL()
        {
            bool isOk = false;
            string val =  _data[HCL];

            if(val.at(0) == '#' && val.length() == 7)
            {
                try
                {
                    long value = stol(val.substr(1), 0, 16);  // hexadecimal
                    isOk = true;
                }
                catch(const std::invalid_argument){
                    cerr << "stol: Invalid argument: " << val.substr(1) << "\n";
                }
            }


            return isOk;
        }

    bool verifyECL()
        {
            string val = _data[ECL];

            return
                val.length() == 3 &&
                (
                    val.compare("amb") == 0 ||
                    val.compare("blu") == 0 ||
                    val.compare("brn") == 0 ||
                    val.compare("gry") == 0 ||
                    val.compare("grn") == 0 ||
                    val.compare("hzl") == 0 ||
                    val.compare("oth") == 0
                );
        }

    bool verifyPID()
        {
            bool isOk = false;
            string val = _data[PID];

            if(val.length() == 9)
            {
                try
                {
                    long value = stol(val);  // decimal
                    isOk = true;
                }
                catch(const std::invalid_argument){
                    cerr << "stol: Invalid argument: " << val << "\n";
                }
            }

            return isOk;
        }

public:
    Passport() {}
    Passport(const Passport& p) : _data(p._data) {}

    Passport& operator=(Passport other)
        {
            _data = other._data;
            return *this;
        }

    void clear()
        {
            _data.clear();
        }

    void insert(passportEntries key, string& value)
        {
            _data[key] = value;
        }


    bool isPassportValid()
        {
            vector<passportEntries> neededEntries = {
                BYR,
                IYR,
                EYR,
                HGT,
                HCL,
                ECL,
                PID,
                CID
            };

            // Lets check this passport validity
            int nbrValidEntries = 0;

            for(auto e : neededEntries)
            {
                if(_data.find(e) != _data.end())
                    nbrValidEntries++;
            }

            if(
                nbrValidEntries == neededEntries.size() ||      // All entries present
                (
                    nbrValidEntries == neededEntries.size() -1 &&  // Or only cid is missing
                    _data.find(CID) == _data.end()
                )
            )
            {
                return true;
            }

            return false;
        }

    bool isPassportValidAndVerified(bool debug=false)
        {
            if(debug)
            {
                cout << "isPassportValidAndVerified: BYR: " << verifyBYR() << endl;
                cout << "isPassportValidAndVerified: IYR: " << verifyIYR() << endl;
                cout << "isPassportValidAndVerified: EYR: " << verifyEYR() << endl;
                cout << "isPassportValidAndVerified: HGT: " << verifyHGT() << endl;
                cout << "isPassportValidAndVerified: HCL: " << verifyHCL() << endl;
                cout << "isPassportValidAndVerified: ECL: " << verifyECL() << endl;
                cout << "isPassportValidAndVerified: PID: " << verifyPID() << endl;
            }

            return
                verifyBYR() &&
                verifyIYR() &&
                verifyEYR() &&
                verifyHGT() &&
                verifyHCL() &&
                verifyECL() &&
                verifyPID();     // We ignore CID
        }

    string serialize()
        {
            string retValue;

            retValue =
                " byr: " + _data[BYR] +
                " iyr: " + _data[IYR] +
                " eyr: " + _data[EYR] +
                " hgt: " + _data[HGT] +
                " hcl: " + _data[HCL] +
                " ecl: " + _data[ECL] +
                " pid: " + _data[PID] +
                " cid: " + _data[CID];
            return retValue;
        }
};

std::ostream& operator<<(std::ostream& os, Passport& p)
{
    return os << p.serialize();
}


// Solve puzzle #1
template <typename T>
constexpr int extractPassportInfo(T data, vector<Passport>& listPassports)
{
    map<string, passportEntries> _strToEntry = {
        {"byr", BYR},
        {"iyr", IYR},
        {"eyr", EYR},
        {"hgt", HGT},
        {"hcl", HCL},
        {"ecl", ECL},
        {"pid", PID},
        {"cid", CID}
    };

    Passport passport;

    // Add a blank line at the end of the data array
    data.push_back("");

    listPassports.clear();

    for(auto d: data)
    {
        if(!d.empty())
        {
            // Extract current passport entries
            std::replace(d.begin(), d.end(), ':', ' ');  // replace ':' by ' '

            // Tokenize
            std::stringstream ifs(d);
            string f1, f2;

            while(ifs >> f1 >> f2)
            {
                passport.insert (_strToEntry[f1],f2);
            }
        }
        else
        {
            listPassports.push_back(passport);

            // Clear passport for possible next entry
            passport.clear();
        }
    }
    return listPassports.size();
}

// Solve puzzle #1
int solve_puzzle1(vector<Passport>& candidatePassports)
{
    int nbrValidPassport = 0;
    for(auto p = std::begin(candidatePassports); p != std::end(candidatePassports); )
    {
        if(p->isPassportValid())
        {
            nbrValidPassport++;
            p++;
        }
        else
        {
            candidatePassports.erase(p);
        }
    }
    return nbrValidPassport;
}

// Solve puzzle #2
int solve_puzzle2(vector<Passport>& candidatePassports, bool debug=false)
{
    int nbrValidPassport = 0;
    for(auto p : candidatePassports)
    {
        if(debug)
            cout << p << endl;

        if(p.isPassportValidAndVerified(debug))
            nbrValidPassport++;
    }
    if(debug)
        cout << " Nbr valid and verified passports: "  << nbrValidPassport << endl;

    return nbrValidPassport;
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
    const vector<string> example1 = {
        "ecl:gry pid:860033327 eyr:2020 hcl:#fffffd",
        "byr:1937 iyr:2017 cid:147 hgt:183cm",
        "",
        "iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884",
        "hcl:#cfa07d byr:1929",
        "",
        "hcl:#ae17e1 iyr:2013",
        "eyr:2024",
        "ecl:brn pid:760753108 byr:1931",
        "hgt:179cm",
        "",
        "hcl:#cfa07d eyr:2025 pid:166559648",
        "iyr:2011 ecl:brn hgt:59in"
    };

    vector<Passport> candidatePassports;
    extractPassportInfo<vector<string>>(example1, candidatePassports);

    assert(solve_puzzle1(candidatePassports) == 2 && "Error verifying puzzle #1");

    // Solve puzzle #1
    extractPassportInfo<vector<string>>(data, candidatePassports);
    std::cout << "Answer for puzzle #1: "<< solve_puzzle1(candidatePassports) << std::endl;

    // --------- Puzzle #2 ---------
    // Verify puzzle2 examples
    const vector<string> example2Bad = {
        "eyr:1972 cid:100",
        "hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926",
        "",
        "iyr:2019",
        "hcl:#602927 eyr:1967 hgt:170cm",
        "ecl:grn pid:012533040 byr:1946",
        "",
        "hcl:dab227 iyr:2012",
        "ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277",
        "",
        "hgt:59cm ecl:zzz",
        "eyr:2038 hcl:74454a iyr:2023",
        "pid:3556412378 byr:2007"
    };

    vector<Passport> badPassports;
    extractPassportInfo<vector<string>>(example2Bad, badPassports);
    assert(solve_puzzle2(badPassports) == 0 && "Error verifying puzzle #2 with bad examples");

    const vector<string> example2Good = {
        "pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980",
        "hcl:#623a2f",
        "",
        "eyr:2029 ecl:blu cid:129 byr:1989",
        "iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm",
        "",
        "hcl:#888785",
        "hgt:164cm byr:2001 iyr:2015 cid:88",
        "pid:545766238 ecl:hzl",
        "eyr:2022",
        "",
        "iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719"
    };

    vector<Passport> goodPassports;
    extractPassportInfo<vector<string>>(example2Good, goodPassports);
    assert(solve_puzzle2(goodPassports) == 4 && "Error verifying puzzle #2 with good examples");

    // Solve puzzle #2
    std::cout << "Answer for puzzle #2: "<< solve_puzzle2(candidatePassports) << std::endl;
}
