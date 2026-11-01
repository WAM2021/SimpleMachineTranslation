// Author: WAM2021
// Date: 05/10/2025

#include <iostream>
#include <fstream>
#include "scanner.h"
#include "parser.h"
#include "testTree.h"
#include "semantics.h"
#include "codeGen.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [optional_filename]\n";
        return 1;
    }

    std::ifstream file;
    if (argc == 2) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Failed to open file: " << argv[1] << "\n";
            return 1;
        }
        std::cin.rdbuf(file.rdbuf());
    }
    std::string outputFile = "vm_output.txt";
    std::string inputFile = argv[1];
    TreeNode* root = parser();
    
    //printTree(root, 0);
    //Print out ParseTree from P2
    checkSemantics(root);
    generateCode(inputFile, outputFile);
    return 0;
}
