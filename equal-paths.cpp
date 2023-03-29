#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node * root)
{
    if(!root)
    {
        return 0;
    }
    return (std::max(height(root->left), height(root->right)) + 1);
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL)
    {
        return true; 
    }
    if(root->left == NULL && root->right == NULL)
    {
        return true; 
    }
    if(!root->left)
    {
        return height(root->right) == 1; 
    }
    if(!root->right)
    {
        return height(root->left) == 1; 
    }
    return height(root->left) == height(root->right); 
}

