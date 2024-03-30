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
    void rotateLeft(AVLNode<Key,Value>* targetNode);
    void rotateRight(AVLNode<Key,Value>* targetNode);
    void insertFixBalance(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    void removeFixBalance(AVLNode<Key,Value>* targetNode, int difference);
    AVLNode<Key,Value>* successorForAVL(AVLNode<Key,Value>* targetNode);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    newNode->setBalance(0);
  
    //if tree is empty
    if(this->root_ == NULL)
    {
      this->root_ = newNode;
      return;
    }
    
 
    AVLNode<Key, Value>* child = static_cast<AVLNode<Key, Value>*>(this->root_);
    //AVLNode<Key, Value>* parent = NULL;
    while (true)
    {
      //parent = nextChild;
      if(new_item.first == child->getKey())
      {
        child->setValue(new_item.second);
        delete newNode;
        return;
      }
      if(new_item.first < child->getKey())
      {
        if(child->getLeft() == NULL)
        {
          child->setLeft(newNode);
          newNode->setParent(child);
          break;
        }
        child = child->getLeft();
      }
      else 
      {
        if(child->getRight() == NULL)
        {
          child->setRight(newNode);
          newNode->setParent(child);
          break;
        }
        child = child->getRight();
      }
    }
  

    //balance the tree
    //parent is child and newnode is child now
    AVLNode<Key, Value>* parent = child;

    if(parent->getBalance() == 0)
    {
      //right heavy
      if(parent->getRight() == newNode)
      {
        parent->setBalance(1);
      }
      //left heavy
      else
      {
        parent->setBalance(-1);
      }
      insertFixBalance(parent, newNode);
    }
    else //if parent is -1 or 1 set it to 0 and done
    {
      parent->setBalance(0);
      return;
    }
    //delete newNode;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* newNode = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(newNode == NULL)
    {
      return;
    }
    //two children failing
    if(newNode->getLeft() != NULL && newNode->getRight() != NULL)
    {
      AVLNode<Key, Value> *predecessor_node = static_cast<AVLNode<Key, Value>*>(this->predecessor(newNode)); //1
      nodeSwap(newNode, predecessor_node);
    }
    //if one child and this is working
    //also applies
    AVLNode<Key, Value> *child = NULL;
    if(newNode->getLeft() != NULL)
    {
      child = newNode->getLeft();
    }
    else if(newNode->getRight() != NULL)
    {
      child = newNode->getRight();
    }


    int difference = 0;
    AVLNode<Key, Value>* parent = newNode->getParent();
    if(parent == NULL)
    {
      this->root_ = child;
    }
    else
    {
      if(newNode == parent->getLeft())
      {
        parent->setLeft(child);
        difference = 1;
      }
      else
      {
        parent->setRight(child);
        difference = -1;
      }
    }
    if(child != NULL)
    {
      child->setParent(newNode->getParent());
    }
    delete newNode;

    removeFixBalance(parent, difference);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* parentNode)
{
  AVLNode<Key,Value>* rightChild = parentNode->getRight();
  //grandparent
  AVLNode<Key,Value>* grandparent = parentNode->getParent();
  
  //setting grandparent, parent, and targetNode
  rightChild->setParent(grandparent);
  if(grandparent == NULL)
  {
    this->root_ = rightChild;
  }
  else if(grandparent->getLeft() == parentNode)
  {
    grandparent->setLeft(rightChild);
  }
  else if(grandparent->getRight() == parentNode)
  {
    grandparent->setRight(rightChild);
  }
  //now setting targetNode child
  AVLNode<Key,Value>* leftGrandChild = rightChild->getLeft();

  rightChild->setLeft(parentNode);
  parentNode->setParent(rightChild);
  parentNode->setRight(leftGrandChild);
  if(leftGrandChild != NULL)
  {
    leftGrandChild->setParent(parentNode);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* parentNode)
{
  AVLNode<Key,Value>* leftChild = parentNode->getLeft();
  //grandparent
  AVLNode<Key,Value>* grandparent = parentNode->getParent();
  
  //setting grandparent, parent, and targetNode
  leftChild->setParent(grandparent);
  if(grandparent == NULL)
  {
    this->root_ = leftChild;
  }
  else if(grandparent->getRight() == parentNode)
  {
    grandparent->setRight(leftChild);
  }
  else if(grandparent->getLeft() == parentNode)
  {
    grandparent->setLeft(leftChild);
  }
  //now setting targetNode child
  AVLNode<Key,Value>* rightGrandChild = leftChild->getRight();

  leftChild->setRight(parentNode);
  parentNode->setParent(leftChild);
  parentNode->setLeft(rightGrandChild);
  if(rightGrandChild != NULL)
  {
    rightGrandChild->setParent(parentNode);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFixBalance(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child)
{
  //parent and child are balance
  //need a grandparent
  AVLNode<Key,Value>* grandparent = parent->getParent();

  //the height does not change
  if(parent == NULL || grandparent == NULL)
  {
    return;
  }
  if(grandparent->getLeft() == parent)
  {
    grandparent->updateBalance(-1);
    if(grandparent->getBalance() == 0)
    {
      return;
    }
    else if(grandparent->getBalance() == -1)
    {
      insertFixBalance(grandparent, parent);
    }
    else
    {
      if(parent->getLeft() == child)
      {
        rotateRight(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      else
      {
        rotateLeft(parent);
        rotateRight(grandparent);
        if(child->getBalance() == 0)
        {
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else if(child->getBalance() == 1)
        {
          parent->setBalance(-1);
          grandparent->setBalance(0);
        }
        else if(child->getBalance() == -1)
        {
          parent->setBalance(0);
          grandparent->setBalance(1);
        }
        child->setBalance(0);
      }
    }
  }
  else if(grandparent->getRight() == parent) //right
  {
    grandparent->updateBalance(1);
    if(grandparent->getBalance() == 0)
    {
      return;
    }
    else if(grandparent->getBalance() == 1)
    {
      insertFixBalance(grandparent, parent);
    }
    else
    {
      if(parent->getRight() == child)
      {
        rotateLeft(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      else
      {
        rotateRight(parent);
        rotateLeft(grandparent);
        if(child->getBalance() == 0)
        {
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else if(child->getBalance() == -1)
        {
          parent->setBalance(1);
          grandparent->setBalance(0);
        }
        else if(child->getBalance() == 1)
        {
          parent->setBalance(0);
          grandparent->setBalance(-1);
        }
        child->setBalance(0);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFixBalance(AVLNode<Key,Value>* targetNode, int difference)
{
  if(targetNode == NULL)
  {
    return;
  }

  AVLNode<Key,Value>* parent = targetNode->getParent();
  int nextdifference = 0;
  if(parent != NULL)
  {
    if(parent->getLeft() == targetNode)
    {
      nextdifference = 1;
    }
    else
    {
      nextdifference = -1;
    }
    
  }

  if(difference == -1)
  {
    if(targetNode->getBalance() + difference == -2)
    {
      AVLNode<Key,Value>* tallerChild = targetNode->getLeft();
      //if(tallerChild != NULL)
      //{
      if(tallerChild->getBalance() == -1)
      {
        rotateRight(targetNode);
        targetNode->setBalance(0);
        tallerChild->setBalance(0);
        removeFixBalance(parent, nextdifference);
      }
      else if (tallerChild->getBalance() == 0)
      {
        rotateRight(targetNode);
        targetNode->setBalance(-1);
        tallerChild->setBalance(1);
      }
      else
      {
        AVLNode<Key,Value>* grandchild = tallerChild->getRight();
        rotateLeft(tallerChild);
        rotateRight(targetNode);
        if(grandchild->getBalance() == 1)
        {
          targetNode->setBalance(0);
          tallerChild->setBalance(-1);
        }
        else if(grandchild->getBalance() == 0)
        {
          targetNode->setBalance(0);
          tallerChild->setBalance(0);
        }
        else if(grandchild->getBalance() == -1)
        {
          targetNode->setBalance(1);
          tallerChild->setBalance(0);
        }
        grandchild->setBalance(0);
        removeFixBalance(parent, nextdifference);
      }
      //}
    }
    else if(targetNode->getBalance() + difference == -1)
    {
      targetNode->setBalance(-1);
    }
    else if(targetNode->getBalance() + difference == 0)
    {
      targetNode->setBalance(0);
      removeFixBalance(parent, nextdifference);
    }
  }
  else if(difference == 1)
  {
    if(targetNode->getBalance() + difference == 2) 
    {
      AVLNode<Key,Value>* tallerChild = targetNode->getRight();
      if(tallerChild != NULL)
      {
        if(tallerChild->getBalance() == 1)
        {
          rotateLeft(targetNode);
          targetNode->setBalance(0);
          tallerChild->setBalance(0);
          removeFixBalance(parent, nextdifference);
        }
        else if (tallerChild->getBalance() == 0)
        {
          rotateLeft(targetNode);
          targetNode->setBalance(1);
          tallerChild->setBalance(-1);
        }
        else if(tallerChild->getBalance() == -1)
        {
          AVLNode<Key,Value>* grandchild = tallerChild->getLeft();
          rotateRight(tallerChild);
          rotateLeft(targetNode);
          if(grandchild->getBalance() == -1)
          {
            targetNode->setBalance(0);
            tallerChild->setBalance(1);
          }
          else if(grandchild->getBalance() == 0)
          {
            targetNode->setBalance(0);
            tallerChild->setBalance(0);
          }
          else if(grandchild->getBalance() == 1)
          {
            targetNode->setBalance(-1);
            tallerChild->setBalance(0);
          }
          grandchild->setBalance(0);
          removeFixBalance(parent, nextdifference);
        }
      }
    
    }
    else if(targetNode->getBalance() + difference == 1)
    {
      targetNode->setBalance(1);
    }
    else if(targetNode->getBalance() + difference == 0)
    {
      targetNode->setBalance(0);
      removeFixBalance(parent, nextdifference);
    }
  }
}

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::successorForAVL(AVLNode<Key,Value>* targetNode)
{
  if (targetNode == NULL)
  {
    return NULL;
  }
  if(targetNode->getRight() != NULL)
  {
    AVLNode<Key,Value>* successor_ = targetNode->getRight();
    while(successor_->getLeft() != NULL)
    {
      successor_ = successor_->getLeft();
    }
    return successor_;
  }
  else
  {
    AVLNode<Key,Value>* parent = targetNode->getParent();
    while(parent != NULL && targetNode == parent->getRight())
    {
      targetNode = parent;
      parent = parent->getParent();
    }
    return parent;
  }
}


#endif
