// Author: WAM2021
// Date: 05/10/2025

#include "scanner.h"
#include <cctype>
#include <sstream>
#include <unordered_set>

static std::istream* input = &std::cin;
static int lineNumber = 0;
static std::string currentLine;
static size_t index = 0;

// Characters considered single-symbol tokens (t1)
const std::unordered_set<char> t1Set = {
    '!', '"', '#', '$', '%', '&', '\'', '(', ')'
};

void setInputStream(std::istream& in) {
    input = &in;
    lineNumber = 0;
    currentLine.clear();
    index = 0;
}

static void skipWhitespaceAndComments() {
    while (true) {
        if (index >= currentLine.size()) {
            if (!std::getline(*input, currentLine)) {
                return;
            }
            currentLine += '\n'; // preserve newlines
            index = 0;
            ++lineNumber;
        }

        // Skip whitespace
        while (index < currentLine.size() && std::isspace(currentLine[index])) {
            ++index;
        }

        // Handle comments (*...*)
        if (index < currentLine.size() && currentLine[index] == '*') {
            ++index;
            while (true) {
                if (index >= currentLine.size()) {
                    if (!std::getline(*input, currentLine)) {
                        std::cerr << "SCANNER ERROR: Unterminated comment at line " << lineNumber << "\n";
                        exit(1);
                    }
                    currentLine += '\n';
                    index = 0;
                    ++lineNumber;
                }
                if (currentLine[index] == '*') {
                    ++index;
                    break;
                }
                ++index;
            }
        } else {
            break;
        }
    }
}

Token scanner() {
    skipWhitespaceAndComments();

    if (index >= currentLine.size()) {
        return { "EOF", "EOF", lineNumber == 0 ? 1 : lineNumber };
    }

    char ch = currentLine[index];

    // Check t1 tokens (single symbol)
    if (t1Set.count(ch)) {
        std::string sym(1, ch);
        ++index;
        return { "t1", sym, lineNumber };
    }

    // Check t2: +<digits>
    if (ch == '+') {
        size_t start = index;
        ++index;
        if (index < currentLine.size() && std::isdigit(currentLine[index])) {
            while (index < currentLine.size() && std::isdigit(currentLine[index])) {
                ++index;
            }
            return { "t2", currentLine.substr(start, index - start), lineNumber };
        } else {
            std::cerr << "SCANNER ERROR: Invalid t2 token at line " << lineNumber << ": " << currentLine.substr(start, index - start + 1) << "\n";
            exit(1);
        }
    }

    // Check t3: letter followed by digits
    if (std::isalpha(ch)) {
        size_t start = index;
        ++index;
        if (index < currentLine.size() && std::isdigit(currentLine[index])) {
            while (index < currentLine.size() && std::isdigit(currentLine[index])) {
                ++index;
            }
            return { "t3", currentLine.substr(start, index - start), lineNumber };
        } else {
            std::cerr << "SCANNER ERROR: Invalid t3 token at line " << lineNumber << ": " << currentLine.substr(start, index - start + 1) << "\n";
            exit(1);
        }
    }

    // Catch-all invalid character error
    std::string invalidChar(1, ch);
    std::cerr << "SCANNER ERROR: Invalid character '" << invalidChar << "' at line " << lineNumber << "\n";
    exit(1);
}
