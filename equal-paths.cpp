#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root)
{
	if(root == NULL)
	{
		return 0;
	}
	return 1 + max(height(root->left), height(root->right));
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL)
    {
      return true;
    }
    int leftH = height(root->left);
	  int rightH = height(root->right);


    if(abs(leftH-rightH) != 0 && leftH != 0 && rightH != 0)
    {
      return false;
    }
    return equalPaths(root->left) && equalPaths(root->right);
}

