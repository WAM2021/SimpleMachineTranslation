// Author: WAM2021
// Date: 05/10/2025

#include "testTree.h"
#include <iostream>
#include <string>

// testTree.cpp
void printTree(TreeNode* root, int level) {
    if (root == nullptr) return;
    // Print the root label
    std::cout << std::string(level * 4, ' ') << root->label;
    if (!root->decoration.empty()) {
        std::cout << ": " << root->decoration;
    }
    std::cout << std::endl;
    
    // Recursively print the children
    for (auto* child : root->children) {
        printTree(child, level + 1);
    }
}
