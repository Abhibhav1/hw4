#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
// Helper function to check if all leaf nodes are at the same depth
bool checkPaths(Node* root, int depth, int& leafDepth) {
    if (!root) return true; // Base case for empty trees

    if (!root->left && !root->right) { // If it's a leaf node
        if (leafDepth == -1) { // First leaf encountered
            leafDepth = depth;
        }
        return depth == leafDepth; // Compare with first leaf depth
    }

    return checkPaths(root->left, depth + 1, leafDepth) &&
           checkPaths(root->right, depth + 1, leafDepth);
}

bool equalPaths(Node* root) {
    int leafDepth = -1; // Store depth of first encountered leaf
    return checkPaths(root, 0, leafDepth);
}

