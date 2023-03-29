#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
		void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); // TODO
		void removeFix(AVLNode<Key, Value>* n, int diff); // TODO
		void rotateRight(AVLNode<Key, Value>* c); // TODO
		void rotateLeft(AVLNode<Key, Value>* c); // TODO
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		AVLNode<Key, Value>* p = NULL;
		AVLNode<Key, Value>* c = NULL;
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>* >(this->root_);

		if(!this->root_)
		{
			this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, p);
			this->root_->setLeft(NULL);
			this->root_->setRight(NULL);
			return;
		}
		else
		{
			while(true)
			{
				if(temp->getKey() == new_item.first)
				{
					temp->setValue(new_item.second);
					break;
				}
				else if(temp->getKey() > new_item.first)
				{
					if(!temp->getLeft())
					{
						p = temp; 
						c = new AVLNode<Key, Value>(new_item.first, new_item.second, p);
						temp->setLeft(c);
						p->updateBalance(-1);
						if(p->getBalance() != 0)
						{
							insertFix(p, c);
						}
						break;
					}
					temp = temp->getLeft();
				}
				else if(temp->getKey() < new_item.first)
				{
					if(!temp->getRight())
					{
						p = temp; 
						c = new AVLNode<Key, Value>(new_item.first, new_item.second, p);
						temp->setRight(c);
						p->updateBalance(1);
						if(p->getBalance() != 0)
						{
							insertFix(p, c);
						}
						break;
					}
					temp = temp->getRight();
				}
			}
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
		if(!this->root_)
		{
			return;
		}
		int difference; 
		AVLNode<Key, Value>* p; 
		AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>* >(BinarySearchTree<Key, Value>::internalFind(key));
		if(n)
		{
			p = n->getParent();
		}
		else
		{
			return; 
		}

		if(n->getLeft() && n->getRight())
		{
			nodeSwap(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(n)), n); 
			p = n->getParent(); 
		}

		if(p)
		{
			if(p->getLeft() != n)
			{
				difference = -1; 
			}
			else
			{
				difference = 1;
			}
		}

		if(!n->getLeft() && !n->getRight())
		{
			if(this->root_ == n)
			{
				delete n;
				n = NULL;
				this->root_ = NULL;
				return; 
			}
			else
			{
				if(n == p->getLeft())
				{
					p->setLeft(NULL);
				}
				else 
				{
					p->setRight(NULL);
				}
				delete n;
				n = NULL; 
			}
		}
		else if(!n->getLeft() && n->getRight())
		{
			if(this->root_ == n)
			{
				this->root_ = n->getRight();
				this->root_->setParent(NULL);
			}
			if(n && p)
			{
				if(p->getLeft() == n)
				{
					p->setLeft(n->getRight());
				}
				else
				{
					p->setRight(n->getRight());
				}
				n->getRight()->setParent(p);
			}
			delete n; 
			n = NULL; 
		}
		else if(n->getLeft() && !n->getRight())
		{
			if(this->root_ == n)
			{
				this->root_ = this->root_->getLeft();
				this->root_->setParent(NULL);
			}
			if(n && p)
			{
				if(p->getLeft() == n)
				{
					p->setLeft(n->getLeft());
				}
				else
				{
					p->setRight(n->getLeft());
				}
				n->getLeft()->setParent(p);
			}
			delete n; 
			n = NULL; 
		}
		removeFix(p, difference);
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

//HELPER FUNCTIONS
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
	// TODO
	if(!p || !p->getParent())
	{
		return; 
	}
	AVLNode<Key, Value>* g = p->getParent(); 
	if(g->getLeft() && g->getLeft() == p)
	{
		g->updateBalance(-1);
		if(g->getBalance() == -1)
		{
			insertFix(g, p); 
		}
		else if(g->getBalance() == -2)
		{
			if(g->getLeft()->getLeft() == n)
			{
				rotateRight(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else
			{
				rotateLeft(p);
				rotateRight(g);
				if(n->getBalance() == -1)
				{
					p->setBalance(0);
					g->setBalance(1); 
				}
				else if(n->getBalance() == 0)
				{
					p->setBalance(0);
					g->setBalance(0); 
				}
				else if(n->getBalance() == 1)
				{
					p->setBalance(-1);
					g->setBalance(0); 
				}
				n->setBalance(0);
			}
		}
	}
	else if(g->getRight() && g->getRight() == p)
	{
		g->updateBalance(1);
		if(g->getBalance() == 1)
		{
			insertFix(g, p);
		}
		else if(g->getBalance() == 2)
		{
			if(g->getRight()->getRight() == n)
			{
				rotateLeft(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else
			{
				rotateRight(p);
				rotateLeft(g);
				if(n->getBalance() == 1)
				{
					p->setBalance(0);
					g->setBalance(-1);
				}
				else if(n->getBalance() == 0)
				{
					p->setBalance(0);
					g->setBalance(0);
				}
				else if(n->getBalance() == -1)
				{
					p->setBalance(1);
					g->setBalance(0);
				}
				n->setBalance(0);
			}
		}
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
  // TODO
	if(n == NULL)
	{ 
		return;
	}
	AVLNode<Key, Value>* p = NULL; 
	AVLNode<Key, Value>* c = NULL; 
	int difference = 0; 
	p = n->getParent();
	if(p)
	{
		if(n->getKey() == p->getLeft()->getKey())
		{
			difference = 1; 
		}
		else if(n->getKey() == p->getRight()->getKey())
		{
			difference = -1; 
		}
	}

	if(diff == -1)
	{
		if(n->getBalance() + diff == -2)
		{
			c = n->getLeft();
			if(c->getBalance() == -1)
			{
				rotateRight(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, difference);
			}
			else if(c->getBalance() == 0)
			{
				rotateRight(n);
				n->setBalance(-1);
				c->setBalance(1);
				return; 
			}
			else if(c->getBalance() == 1)
			{
				AVLNode<Key, Value>* r = c->getRight();
				rotateLeft(c);
				rotateRight(n);
				if(r)
				{
					if(r->getBalance() == 1)
					{
						n->setBalance(0);
            c->setBalance(-1);
            r->setBalance(0);
					}
					else if(r->getBalance() == 0)
					{
						n->setBalance(0);
            c->setBalance(0);
            r->setBalance(0);
					}
					else if(r->getBalance() == -1)
					{
						n->setBalance(1);
            c->setBalance(0);
            r->setBalance(0);
					}
				}
				removeFix(p, difference);
			}
		}
		else if(diff + n->getBalance() == -1)
		{
			n->setBalance(-1);
			return; 
		}
		else if(diff + n->getBalance() == 0)
		{
			n->setBalance(0);
			removeFix(p, difference);
		}
	}
	else if(diff == 1)
	{
		if(n->getBalance() + diff == 2)
		{
			AVLNode<Key, Value>* c = n->getRight();
			if(c)
			{
				if(c->getBalance() == 1)
				{
					rotateLeft(n);
					n->setBalance(0);
					c->setBalance(0);
					removeFix(p, difference);
				}
				else if(c->getBalance() == 0)
				{
					rotateLeft(n);
					n->setBalance(1);
					c->setBalance(-1);
					return; 
				}
				else if(c->getBalance() == -1)
				{
					AVLNode<Key, Value>* g = c->getLeft();
					rotateRight(c);
					rotateLeft(n);
					if(g->getBalance() == -1)
					{
						n->setBalance(0);
						c->setBalance(1);
						g->setBalance(0);
					}
					else if(g->getBalance() == 0)
					{
						n->setBalance(0);
						c->setBalance(0);
						g->setBalance(0);
					}
					else if(g->getBalance() == 1)
					{
						n->setBalance(-1);
						c->setBalance(0);
						g->setBalance(0);
					}
					removeFix(p, difference);
				}
			}
		}
		else if(n->getBalance() + diff == 1)
		{
			n->setBalance(1);
			return;
		}
		else if(n->getBalance() + diff == 0)
		{
			n->setBalance(0);
			removeFix(p, difference);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* c)
{
  // TODO
	AVLNode<Key,Value>* p = c->getLeft();
	AVLNode<Key,Value>* k = p->getRight();
	if(!c->getParent())
	{
		p->setParent(NULL);
		this->root_ = p; 
	}
	else 
	{
		p->setParent(c->getParent());
		if(c->getParent()->getLeft() && c->getParent()->getLeft() == c)
		{
			c->getParent()->setLeft(p);
		}
		else if(c->getParent()->getRight() && c->getParent()->getRight() == c)
		{
			c->getParent()->setRight(p);
		}
	}

	if(k)
	{
		c->setLeft(k);
		k->setParent(c);
	}
	else
	{
		c->setLeft(NULL);
	}

	p->setRight(c);
	c->setParent(p);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* c)
{
  // TODO
	AVLNode<Key,Value>* p = c->getRight();
	AVLNode<Key,Value>* k = p->getLeft();
	if(!c->getParent())
	{
		p->setParent(NULL);
		this->root_ = p; 
	}
	else 
	{
		p->setParent(c->getParent());
		if(c->getParent()->getLeft() && c->getParent()->getLeft() == c)
		{
			c->getParent()->setLeft(p);
		}
		else if(c->getParent()->getRight() && c->getParent()->getRight() == c)
		{
			c->getParent()->setRight(p);
		}
	}

	if(k)
	{
		c->setRight(k);
		k->setParent(c);
	}
	else
	{
		c->setRight(NULL);
	}

	p->setLeft(c);
	c->setParent(p);
}


#endif
