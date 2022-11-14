#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void insertFix(AVLNode<Key, Value>* P, AVLNode<Key,Value>* N);
		void removeFix(AVLNode<Key, Value>* N, int8_t diff);
		void rotateLeft(AVLNode<Key, Value>* N);
		void rotateRight(AVLNode<Key, Value>* N);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* P, AVLNode<Key,Value>* N)
{
	if (P == NULL || P -> getParent() == NULL)
	{
		return;
	}
	AVLNode<Key, Value>* G = P->getParent();
	//getleft
	if (G ->getLeft() == P)
	{
		G->updateBalance(-1);
		if(G->getBalance() == 0)
		{
			return;
		}
		else if (G->getBalance() == -1)
		{
			insertFix(G,P);
		}
		else if (G->getBalance() == -2)
		{
			//zig-zig
			if(G->getLeft() == P && P ->getLeft() == N)
			{
				rotateRight(G);
				P ->setBalance(0);
				G ->setBalance(0);
			}
			else if(G->getRight() == P && P ->getRight() == N)
			{
				rotateRight(G);
				P ->setBalance(0);
				G ->setBalance(0);
			}
			//zig zag
			else 
			{
				rotateLeft(P);
				rotateRight(G);
				if (N -> getBalance() == -1)
				{
					P->setBalance(0);
					G-> setBalance(1);
					N-> setBalance(0);
				}
				else if (N -> getBalance() == 0)
				{
					P->setBalance(0);
					G-> setBalance(0);
					N-> setBalance(0);
				}
				else if (N -> getBalance() == 1)
				{
					P->setBalance(-1);
					G-> setBalance(0);
					N-> setBalance(0);
				}
			}
		}
	}
	//getRight
	else if (G ->getRight() == P)
	{
		G->updateBalance(1);
		if(G->getBalance() == 0)
		{
			return;
		}
		else if (G->getBalance() == 1)
		{
			insertFix(G,P);
		}
		else if (G->getBalance() == 2)
		{
			//zig-zig
			if(G->getRight() == P && P ->getRight() == N)
			{
				rotateLeft(G);
				P ->setBalance(0);
				G ->setBalance(0);
			}
			else if(G->getLeft() == P && P ->getLeft() == N)
			{
				rotateLeft(G);
				P ->setBalance(0);
				G ->setBalance(0);
			}
			//zig zag
			else 
			{
				rotateRight(P);
				rotateLeft(G);
				if (N -> getBalance() == 1)
				{
					P->setBalance(0);
					G-> setBalance(-1);
					N-> setBalance(0);
				}
				else if (N -> getBalance() == 0)
				{
					P->setBalance(0);
					G-> setBalance(0);
					N-> setBalance(0);
				}
				else if (N -> getBalance() == -1)
				{
					P->setBalance(1);
					G-> setBalance(0);
					N-> setBalance(0);
				}
			}
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
	// TODO
	//nothing in tree
	//.first = key .second = value
	AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*) (BinarySearchTree<Key, Value>::root_);
	//empty tree
	if (BinarySearchTree<Key, Value>::root_ == NULL)
	{
		BinarySearchTree<Key, Value>::root_ = new AVLNode<Key,Value>(new_item.first,new_item.second, NULL);
		return; 
	}
	while(true)
	{
		//less than go left 
		if (temp->getKey() > new_item.first)
		{
				//left has no kid
				if (temp -> getLeft() == NULL)
				{
					//parent = temp
					AVLNode<Key,Value>* left=new AVLNode<Key,Value>(new_item.first,new_item.second, temp);
					temp -> setLeft(left); 
					break;
				}
				temp = temp -> getLeft();
		}
		// greater than go right
		else if (temp->getKey() < new_item.first)
		{
			//right has no children
			if (temp->getRight() == NULL)
			{
				//parent = temp
				AVLNode<Key,Value>* right =new AVLNode<Key,Value>(new_item.first,new_item.second, temp);
				temp->setRight(right); 
				break;
			}
			temp = temp->getRight();
		}
			// equal to change value
			else 
			{
				temp->setValue(new_item.second);
				break;
			}
	}
	//updating balance
	if(temp->getBalance() == -1)
	{
		temp->setBalance(0);
	}
	else if (temp->getBalance() == 1)
	{
		temp->setBalance(0);
	}
	else if (temp->getBalance() == 0)
	{
		//left
		if(temp->getLeft() != NULL && temp->getLeft()->getItem() == new_item)
		{
			temp->setBalance(-1);
			insertFix(temp, temp->getLeft());
		}
		//right
		else if(temp->getRight() != NULL && temp->getRight()->getItem() == new_item)
		{
			temp->setBalance(1);
			insertFix(temp, temp->getRight());
		}
	}
}

//remove-fix
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* N, int8_t diff)
{
	int8_t ndiff;
	if(N == NULL)
	{
		return;
	}
	//next recursive call
	AVLNode<Key, Value>* P = N->getParent();
	if (P != NULL)
	{
		if (P-> getLeft() == N)
		{
			ndiff = 1;
		}
		else 
		{
			ndiff = -1;
		}
	}
	//case 1
	if ((N ->getBalance() + diff) == -2)
	{
		AVLNode<Key, Value>* C = N ->getLeft();
		//zig zig case
		//case 1a
		if (C -> getBalance() == -1)
		{
			rotateRight(N);
			N -> setBalance(0);
			C -> setBalance(0);
			removeFix(P, ndiff);
		}
		//zig zig case 
		//case 1b
		else if (C -> getBalance() == 0)
		{
			rotateRight(N);
			N -> setBalance(-1);
			C -> setBalance(1);
			return;
		}
		//zig zag case
		//case 1c
		else if (C -> getBalance() == 1)
		{
			AVLNode<Key, Value>* G = C-> getRight();
			rotateLeft(C);
			rotateRight(N);
			if (G->getBalance() == 1)
			{
				N -> setBalance(0);
				C -> setBalance(-1);
				G -> setBalance(0);
			}
			else 	if (G->getBalance() == 0)
			{
				N -> setBalance(0);
				C -> setBalance(0);
				G -> setBalance(0);
			}
			else 	if (G->getBalance() == -1)
			{
				N -> setBalance(1);
				C -> setBalance(0);
				G -> setBalance(0);
			}
			removeFix(P, ndiff);
		}
	}
	//CASE2
	else if ((N ->getBalance() + diff) == 2)
	{
		AVLNode<Key, Value>* C = N ->getRight();
		//zig zig case
		//case 1a
		if (C -> getBalance() == 1)
		{
			rotateLeft(N);
			N -> setBalance(0);
			C -> setBalance(0);
			removeFix(P, ndiff);
		}
		//zig zig case 
		//case 1b
		else if (C -> getBalance() == 0)
		{
			rotateLeft(N);
			N -> setBalance(1);
			C -> setBalance(-1);
			return;
		}
		//zig zag case
		//case 1c
		else if (C -> getBalance() == -1)
		{
			AVLNode<Key, Value>* G = C-> getLeft();
			rotateRight(C);
			rotateLeft(N);
			if (G->getBalance() == -1)
			{
				N -> setBalance(0);
				C -> setBalance(1);
				G -> setBalance(0);
			}
			else 	if (G->getBalance() == 0)
			{
				N -> setBalance(0);
				C -> setBalance(0);
				G -> setBalance(0);
			}
			else 	if (G->getBalance() == 1)
			{
				N -> setBalance(-1);
				C -> setBalance(0);
				G -> setBalance(0);
			}
			removeFix(P, ndiff);
		}
	}
	//Case3
	else if((N->getBalance() + diff) == -1)
	{
		N->setBalance(-1);
	}
	else if((N->getBalance() + diff) == 1)
	{
		N->setBalance(1);
	}
	else if((N->getBalance() + diff) == 0)
	{
		N->setBalance(0);
		removeFix(P,ndiff);
	}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
	// TODO
	AVLNode <Key,Value> *temp = (AVLNode<Key,Value>*)(BinarySearchTree<Key, Value>::internalFind(key));
	int8_t diff;
	if (temp == NULL) 
	{
		return;
	}
	//– 2 children,, Swap the value with its in-order successor or predecessor and then remove from its new location
	if (temp -> getLeft() != NULL && temp -> getRight() != NULL)
	{
		nodeSwap(temp, (AVLNode<Key,Value>*)(BinarySearchTree<Key, Value>::predecessor(temp)));
	}
	AVLNode <Key,Value> *P = temp -> getParent();
	if (P != NULL)
	{
		if (P ->getLeft() == temp)
		{
			diff = 1;
		}
		else if (P ->getRight() == temp)
		{
			diff = -1;
		}
	}
	//0 children //remove leaf node
	if (temp -> getLeft() == NULL && temp -> getRight() == NULL)
	{
		if(temp -> getParent() == NULL)
		{
			BinarySearchTree<Key, Value>::root_ = NULL;
		}
		else 
		{
			//parent's right child is now null
			if(temp -> getParent() -> getRight() == temp)
			{
				temp -> getParent() -> setRight(NULL);
			} 
			//parent's left child is now null
			else if(temp -> getParent() -> getLeft() == temp)
			{
				temp -> getParent() -> setLeft(NULL);
			} 
		}
		delete temp;
	}
	//1 child,  Promote the child into the node's location
	else 
	{
		//if parent is null
		if(temp -> getParent() == NULL)
		{
			//make sure child is new root, run through if statement to see which one
			if (temp -> getLeft() == NULL)
			{
				BinarySearchTree<Key, Value>::root_ = temp -> getRight();
			}
			if (temp -> getRight() == NULL)
			{
				BinarySearchTree<Key, Value>::root_ = temp -> getLeft();
			}
		}
		else 
		{
			if (temp -> getLeft() == NULL)
			{
        //if temp is the right child of parent,, changing parents right child to temps right child
				if (temp-> getParent() -> getRight() == temp)
				{
					temp-> getParent() -> setRight(temp -> getRight());
				}
        //if temp is the right child of parent,, changing parents left child to temps right child
				else if (temp-> getParent() -> getLeft() == temp)
				{
					temp-> getParent() -> setLeft(temp -> getRight());
				}
			}
			if (temp -> getRight() == NULL)
			{
        //if temp is the left child of parent,, changing parents right child to temps left child
				if (temp-> getParent() -> getRight() == temp)
				{
					temp-> getParent() -> setRight(temp -> getLeft());
				}
        //if temp is the left child of parent,, changing parents left child to temps left child
				else if (temp-> getParent() -> getLeft() == temp)
				{
					temp-> getParent() -> setLeft(temp -> getLeft());
				}
			}
		}
    //sets leftchild parent to temp's parents
		if (temp -> getRight() == NULL)
		{
			temp -> getLeft() -> setParent(temp -> getParent());
		}
    //sets rightchild parent to temp's parents
		else if (temp -> getLeft() == NULL)
		{
			temp -> getRight() -> setParent(temp -> getParent());
		}
		delete temp;
	}
	removeFix(P, diff);
}

//rotateRight
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* N)
{
	//z == N
	//temp = y
	AVLNode <Key,Value> *temp = N ->getLeft();
	if (temp == NULL)
	{
		return;
	}

	N ->setLeft(temp -> getRight());
	//fix children of two nodes
	if (temp -> getRight() != NULL)
	{
		temp ->getRight() -> setParent(N);
	}
	AVLNode <Key,Value> *P = N ->getParent();
	//change parent/pointers
	if (P == NULL)
	{
		BinarySearchTree<Key, Value>:: root_ = temp;
	}
	else if (P -> getLeft() == N)
	{
		P ->setLeft(temp);
	}
	else 
	{
		P -> setRight(temp);
	}
	temp -> setParent(P);

	N -> setParent(temp);
	temp -> setRight(N);
}

//rotateLeft
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* N)
{
	//z == N
	//temp = y
	AVLNode <Key,Value> *temp = N ->getRight();
	if (temp == NULL)
	{
		return;
	}

	N ->setRight(temp -> getLeft());
	//fix children of two nodes
	if (temp -> getLeft() != NULL)
	{
		temp ->getLeft() -> setParent(N);
	}
	AVLNode <Key,Value> *P = N ->getParent();
	//change parent/pointers
	if (P == NULL)
	{
		BinarySearchTree<Key, Value>:: root_ = temp;
	}
	else if (P -> getRight() == N)
	{
		P ->setRight(temp);
	}
	else 
	{
		P -> setLeft(temp);
	}
	temp -> setParent(P);

	N -> setParent(temp);
	temp -> setLeft(N);
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
