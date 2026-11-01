// Author: WAM2021
// Date: 05/10/2025

#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <iostream>
#include <fstream>

// Token structure
struct Token {
    std::string tokenType;  // t1, t2, t3, EOF, ERR
    std::string instance;   // actual lexeme
    int lineNumber;         // line where found
};

Token scanner();

void setInputStream(std::istream& in);

#endif
