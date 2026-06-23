#ifndef TREEIMPL_HPP
#define TREEIMPL_HPP

#include "Tree.hpp"

namespace vishnevskiy
{
  template <class Key, class Value>
  Node<Key, Value>::Node():
    key(nullptr),
    val(nullptr),
    parent(nullptr),
    left(nullptr),
    right(nullptr),
    height(0)
  {}

  template <class Key, class Value>
  Node<Key, Value>::Node(const Key* k, const Value* v, Node* p):
    key(k),
    val(v),
    parent(p),
    left(nullptr),
    right(nullptr),
    height(1)
  {}

  template <class Key, class Value>
  Node<Key, Value>::~Node()
  {
    delete key;
    delete val;
  }

  template <class Key, class Value>
  bool Node<Key, Value>::isLeaf() const {
    return key == nullptr && val == nullptr && left == this && right == this;
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::fallRight()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
      }
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::fallLeft()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
      }
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::next()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
        fallLeft();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> right)
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::next()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
        fallRight();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> left)
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }
}
