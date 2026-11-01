// Author: WAM2021
// Date: 05/10/2025

#ifndef TREENODE_H
#define TREENODE_H

#include <string>
#include <vector>

struct TreeNode {
    std::string label;
    std::string tokenInstance; // for terminal nodes
    std::string decoration;    // for printing or annotations

    std::vector<TreeNode*> children;

    TreeNode(const std::string& lbl) : label(lbl), tokenInstance(""), decoration("") {}
    TreeNode(const std::string& lbl, const std::string& token) : label(lbl), tokenInstance(token), decoration("") {}
};

#endif
