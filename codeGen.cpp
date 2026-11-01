// Author: WAM2021
// Date: 05/10/2025

#include "codeGen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace std;

// Helper to extract number from a string like +39 or k10
string extractNumber(const string& token) {
    string number;
    for (char c : token) {
        if (isdigit(c) || c == '-') number += c;
    }
    return number;
}
// Makes variables that are usable in VM
string makeVar(const string& token) {
    string number = extractNumber(token);
    return "p" + number;
}

void generateCode(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);

    if (!in || !out) {
        cerr << "Error opening file." << endl;
        return;
    }

    vector<string> tokens;
    string token;
    while (in >> token) {
        tokens.push_back(token);
    }

    unordered_set<string> declared;
    unordered_set<string> used;

    for (size_t i = 0; i < tokens.size(); ++i) {
        string t = tokens[i];

        if (t == "\"") { // Declare memory slot
            if (i + 1 < tokens.size()) {
                string var = makeVar(tokens[++i]);
                if (declared.find(var) == declared.end()) {
                    out << "LOAD 0\n";
                    out << "STORE " << var << "\n";
                    declared.insert(var);
                }
            }
        }
        else if (t == "#") { // Read input to variable
            if (i + 1 < tokens.size()) {
                string var = makeVar(tokens[++i]);
                out << "READ " << var << "\n";
                out << "LOAD " << var << "\n";
                declared.insert(var);
                used.insert(var);
            }
        }
        else if (t == "%") { // Assignment: prev = value
            if (i > 0 && i + 1 < tokens.size()) {
                string var = makeVar(tokens[i - 1]);
                string valToken = tokens[++i];

                if (valToken[0] == 'k') {
                    string val = extractNumber(valToken);
                    out << "LOAD -" << val << "\n";
                } else {
                    string val = makeVar(valToken);
                    out << "LOAD " << val << "\n";
                    used.insert(val);
                }

                out << "STORE " << var << "\n";
                declared.insert(var);
                used.insert(var);
            }
        }

        else if (t == "!") { // Multiply by -1
            if (i + 1 < tokens.size()) {
                string var = makeVar(tokens[++i]);
                out << "LOAD " << var << "\n";
                out << "MULT -1\n";
                out << "STORE " << var << "\n";
                declared.insert(var);
                used.insert(var);
            }
        }
        else if (t == "$") { // Write value
            if (i + 1 < tokens.size()) {
                string var = makeVar(tokens[++i]);
                out << "WRITE " << var << "\n";
                used.insert(var);
            }
        }
    }

    out << "STOP\n";

    // Write initialized memory declarations
    unordered_set<string> all;
    for (const string& d : declared) all.insert(d);
    for (const string& u : used) all.insert(u);

    for (const string& var : all) {
        out << var << " 0\n";
    }
    std:: cout << "Generated Code written to 'vm_output.txt'\n";
}
