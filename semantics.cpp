// Author: WAM2021
// Date: 05/10/2025


#include "semantics.h"
#include <iostream>
#include <unordered_set>

std::unordered_set<std::string> definedVars;

void semanticTraverse(TreeNode* node) {
    if (!node) return;

    // Variable definition: look for C or A nodes with t1: " or #
    if (node->label == "C" || node->label == "A") {
        if (node->children.size() >= 2) {
            TreeNode* declToken = node->children[0];
            TreeNode* varToken = node->children[1];

            if ((declToken->label == "t1" && (declToken->tokenInstance == "\"" || declToken->tokenInstance == "#"))
                && varToken->label == "t2") {

                std::string varName = varToken->tokenInstance;
                if (definedVars.find(varName) != definedVars.end()) {
                    std::cerr << "SEMANTIC ERROR: Variable redefinition: " << varName << "\n";
                    exit(1);
                } else {
                    definedVars.insert(varName);
                }
            }
        }
    }

    // Variable usage: look for F nodes with t2 or t3
    if (node->label == "F" && node->children.size() == 1) {
        TreeNode* varToken = node->children[0];
        if (varToken->label == "t2") {
            std::string varName = varToken->tokenInstance;
            if (definedVars.find(varName) == definedVars.end()) {
                std::cerr << "SEMANTIC ERROR: Variable used before definition: " << varName << "\n";
                exit(1);
            }
        }
    }

    for (TreeNode* child : node->children) {
        semanticTraverse(child);
    }
}

void checkSemantics(TreeNode* root) {
    definedVars.clear();
    semanticTraverse(root);

    std::cout << "Symbol Table:\n";
    for (const auto& var : definedVars) {
        std::cout << var << "\n";
    }
    }
