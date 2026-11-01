// Author: WAM2021
// Date: 05/10/2025

#include "parser.h"
#include <iostream>
#include <stdexcept>

// Forward declarations for nonterminals
TreeNode* S();
TreeNode* A();
TreeNode* B();
TreeNode* C();
TreeNode* D();
TreeNode* E();
TreeNode* F();
TreeNode* G();

Token tk; // current token

void advanceToken() {
    tk = scanner();
}

Token match(const std::string& expected) {
    if (tk.tokenType != expected) {
        std::cerr << "PARSER ERROR: expected " << expected
                  << ", got " << tk.tokenType << " (" << tk.instance
                  << ") on line " << tk.lineNumber << "\n";
        exit(1);
    }
    Token matched = tk;
    advanceToken();
    return matched;
}

TreeNode* parser() {
    advanceToken(); // initialize token stream
    TreeNode* root = S();
    if (tk.tokenType != "EOF") {
        std::cerr << "PARSER ERROR: Unexpected token after valid parse: "
                  << tk.instance << " on line " << tk.lineNumber << "\n";
        exit(1);
    }
    return root;
}

TreeNode* S() {
    TreeNode* node = new TreeNode("S");
    node->children.push_back(A());

    Token leftParen = match("t1"); // (
    if (leftParen.instance != "(") {
        std::cerr << "PARSER ERROR in S: expected '(', got " << leftParen.instance << "\n";
        exit(1);
    }
    node->children.push_back(new TreeNode("t1", leftParen.instance));

    node->children.push_back(B());
    node->children.push_back(B());

    Token rightParen = match("t1"); // )
    if (rightParen.instance != ")") {
        std::cerr << "PARSER ERROR in S: expected ')', got " << rightParen.instance << "\n";
        exit(1);
    }
    node->children.push_back(new TreeNode("t1", rightParen.instance));

    return node;
}

TreeNode* A() {
    TreeNode* node = new TreeNode("A");
    while ((tk.tokenType == "t1" || tk.tokenType == "t2") && tk.instance != "(") {
        node->children.push_back(new TreeNode(tk.tokenType, tk.instance));
        advanceToken();
    }
    if (node->children.empty()) {
        node->children.push_back(new TreeNode("empty"));
    }
    return node;
}


TreeNode* B() {
    TreeNode* node = new TreeNode("B");
    if (tk.tokenType == "t1" && tk.instance == "(") {
        node->children.push_back(S());
    }
    else if (tk.tokenType == "t1" && (tk.instance == "#" || tk.instance == "!")) {
        node->children.push_back(C());
    }
    else if (tk.tokenType == "t1" && tk.instance == "$") {
        node->children.push_back(D());
    }
    else if (tk.tokenType == "t1" && tk.instance == "'") {
        node->children.push_back(E());
    }
    else if (tk.tokenType == "t2") {
        node->children.push_back(G());
    }
    else {
        std::cerr << "PARSER ERROR in B: unexpected token "
                  << tk.tokenType << " (" << tk.instance
                  << ") on line " << tk.lineNumber << "\n";
        exit(1);
    }
    return node;
}

TreeNode* C() {
    TreeNode* node = new TreeNode("C");
    if (tk.tokenType == "t1" && tk.instance == "#") {
        node->children.push_back(new TreeNode("t1", tk.instance));
        advanceToken();
        Token t2 = match("t2");
        node->children.push_back(new TreeNode("t2", t2.instance));
    }
    else if (tk.tokenType == "t1" && tk.instance == "!") {
        node->children.push_back(new TreeNode("t1", tk.instance));
        advanceToken();
        node->children.push_back(F());
    } else {
        std::cerr << "PARSER ERROR in C\n";
        exit(1);
    }
    return node;
}

TreeNode* D() {
    TreeNode* node = new TreeNode("D");
    Token dollar = match("t1");
    if (dollar.instance != "$") {
        std::cerr << "PARSER ERROR in D: expected '$', got " << dollar.instance << "\n";
        exit(1);
    }
    node->children.push_back(new TreeNode("t1", dollar.instance));
    node->children.push_back(F());
    return node;
}

TreeNode* E() {
    TreeNode* node = new TreeNode("E");
    Token quote = match("t1");
    if (quote.instance != "'") {
        std::cerr << "PARSER ERROR in E: expected ''', got " << quote.instance << "\n";
        exit(1);
    }
    node->children.push_back(new TreeNode("t1", quote.instance));
    node->children.push_back(F());
    node->children.push_back(F());
    node->children.push_back(F());
    node->children.push_back(B());
    return node;
}

TreeNode* F() {
    TreeNode* node = new TreeNode("F");
    if (tk.tokenType == "t2" || tk.tokenType == "t3") {
        node->children.push_back(new TreeNode(tk.tokenType, tk.instance));
        advanceToken();
    }
    else if (tk.tokenType == "t1" && tk.instance == "&") {
        node->children.push_back(new TreeNode("t1", tk.instance));
        advanceToken();
        node->children.push_back(F());
        node->children.push_back(F());
    } else {
        std::cerr << "PARSER ERROR in F: unexpected token "
                  << tk.tokenType << " (" << tk.instance
                  << ") on line " << tk.lineNumber << "\n";
        exit(1);
    }
    return node;
}

TreeNode* G() {
    TreeNode* node = new TreeNode("G");
    Token t2 = match("t2");
    node->children.push_back(new TreeNode("t2", t2.instance));

    Token pct = match("t1");
    if (pct.instance != "%") {
        std::cerr << "PARSER ERROR in G: expected '%', got " << pct.instance << "\n";
        exit(1);
    }
    node->children.push_back(new TreeNode("t1", pct.instance));
    node->children.push_back(F());
    return node;
}
