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
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void rebalanceAfterInsert(AVLNode<Key, Value>* node);
    void rebalanceAfterRemove(AVLNode<Key, Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* parent = nullptr;
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    
    while (curr) {
        parent = curr;
        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } else if(new_item.first > curr->getKey()) {
            curr = curr->getRight();
        }
        else {
            curr->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (!parent) {
        this->root_ = newNode;
    } else if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    rebalanceAfterInsert(newNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!node) return;
    
    if (node->getLeft() && node->getRight()) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
    }
    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = node->getLeft() ? node->getLeft() : node->getRight();
    if (child) child->setParent(node->getParent());
    
    if (!node->getParent()) {
        this->root_ = child;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(child);
    } else {
        node->getParent()->setRight(child);
    }
    delete node;
    rebalanceAfterRemove(parent);
}

// Rotation Functions
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* rightChild = node->getRight();
    if (!rightChild) return;
    node->setRight(rightChild->getLeft());
    if (rightChild->getLeft()) rightChild->getLeft()->setParent(node);
    rightChild->setParent(node->getParent());
    if (!node->getParent()) {
        this->root_ = rightChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(rightChild);
    } else {
        node->getParent()->setRight(rightChild);
    }
    rightChild->setLeft(node);
    node->setParent(rightChild);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* leftChild = node->getLeft();
    if (!leftChild) return;
    node->setLeft(leftChild->getRight());
    if (leftChild->getRight()) leftChild->getRight()->setParent(node);
    leftChild->setParent(node->getParent());
    if (!node->getParent()) {
        this->root_ = leftChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(leftChild);
    } else {
        node->getParent()->setRight(leftChild);
    }
    leftChild->setRight(node);
    node->setParent(leftChild);
}

// Balancing functions
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterInsert(AVLNode<Key, Value>* node) {
    while (node) {
        int balance = this->getHeight(node->getLeft()) - this->getHeight(node->getRight());
        if (balance > 1) {
            if (this->getHeight(node->getLeft()->getLeft()) >= this->getHeight(node->getLeft()->getRight())) {
                rotateRight(node);
            } else {
                rotateLeft(node->getLeft());
                rotateRight(node);
            }
        } else if (balance < -1) {
            if (this->getHeight(node->getRight()->getRight()) >= this->getHeight(node->getRight()->getLeft())) {
                rotateLeft(node);
            } else {
                rotateRight(node->getRight());
                rotateLeft(node);
            }
        }
        node = node->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterRemove(AVLNode<Key, Value>* node) {
    while (node) {
        int balance = this->getHeight(node->getLeft()) - this->getHeight(node->getRight());
        if (balance > 1) {
            if (this->getHeight(node->getLeft()->getLeft()) >= this->getHeight(node->getLeft()->getRight())) {
                rotateRight(node);
            } else {
                rotateLeft(node->getLeft());
                rotateRight(node);
            }
        } else if (balance < -1) {
            if (this->getHeight(node->getRight()->getRight()) >= this->getHeight(node->getRight()->getLeft())) {
                rotateLeft(node);
            } else {
                rotateRight(node->getRight());
                rotateLeft(node);
            }
        }
        node = node->getParent();
    }
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
