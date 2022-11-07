#include "equal-paths.h"
#include <algorithm>
using namespace std;


// You may add any prototypes of helper functions here
//returns int that recursively finds the height of tree
int depth(Node * root)
{
	if (root == nullptr)
	{
		return 0; //nothing more to add
	}
	else 
	{
		//there's more nodes
		//go to next one either left or right, increase size up recursive call
		return 1 + max(depth(root->left),(depth(root->right))); 
	}
}

bool equalPaths(Node * root)
{
  // Add your code below
	//nullptr return true, if it's empty already equal path
	if (root == nullptr)
	{
		return true;
	}
	if((root->left == nullptr) && (root->right == nullptr))
	{
		return true;
	}
	if((root->left != nullptr) && (root->right != nullptr))
	{
		int left = depth(root->left);
		int right = depth(root->right);
		//checking if same amount of nodes
		//true = same amount
		//false = not same amount
		return left == right; 
	}
	if((root->left != nullptr) && (root->right == nullptr))
	{
		return equalPaths(root->left);
	}
	if((root->left == nullptr) && (root->right != nullptr))
	{
		return equalPaths(root->right);
	}
}

