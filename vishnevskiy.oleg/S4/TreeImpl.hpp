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
  void BSIterator<Key, Value>::prev()
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

  template <class Key, class Value>
  Value* BSIterator<Key, Value>::getVal()
  {
    if (curr)
    {
      return curr -> val;
    }
  }

  template <class Key, class Value>
  Value* BSIterator<Key, Value>::getKey()
  {
    if (curr)
    {
      return curr -> key;
    }
  }

  template <class Key, class Value>
  Node<Key, Value>* BSIterator<Key, Value>::getNode()
  {
    return curr;
  }

  template <class Key, class Value>
  void BSConstIterator<Key, Value>::fallRight()
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
  void BSConstIterator<Key, Value>::fallLeft()
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
  void BSConstIterator<Key, Value>::next()
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
  void BSConstIterator<Key, Value>::prev()
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

  template <class Key, class Value>
  const Value* BSConstIterator<Key, Value>::getVal() const
  {
    if (curr)
    {
      return curr -> val;
    }
  }

  template <class Key, class Value>
  const Value* BSConstIterator<Key, Value>::getKey() const
  {
    if (curr)
    {
      return curr -> key;
    }
  }

  template <class Key, class Value>
  const Node<Key, Value>* BSConstIterator<Key, Value>::getNode() const
  {
    return curr;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree():
    nodes(0)
  {
    fakeLeaf = new Node<Key, Value>();
    fakeLeaf -> left = fakeLeaf;
    fakeLeaf -> right = fakeLeaf;
    fakeLeaf -> parent = fakeLeaf;
    fakeLeaf -> height = 0;
    fakeRoot = new Node<Key, Value>();
    fakeRoot -> left = fakeLeaf;
    fakeRoot -> right = fakeLeaf;
    fakeRoot -> parent = nullptr;
    fakeRoot -> height = 0;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::~BSTree()
  {
    Node<Key, Value>* curr = fakeRoot -> right;
    if (curr && !curr -> isLeaf())
    {
      while (!curr -> isLeaf())
      {
        if (curr -> left && !curr -> left -> isLeaf())
        {
          curr = curr -> left;
        } 
        else if (curr -> right && !curr -> right -> isLeaf())
        {
          curr = curr -> right;
        }
        else
        {
          Node<Key, Value>* parent = curr -> parent;
          if (parent -> left == curr)
          {
            parent -> left = fakeLeaf;
          }
          else if (parent -> right == curr)
          {
            parent -> right = fakeLeaf;
          }
          delete curr;
          curr = parent;
          if (curr == fakeRoot)
          {
            break;
          }
        }
      }
    }
    delete fakeRoot;
    delete fakeLeaf;
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value>* BSTree<Key, Value, Compare>::copy(Node<Key, Value>* node,Node<Key, Value>* parent)
  {
    if (!node || node -> isLeaf())
    {
      return fakeLeaf
    }

    Node<Key, Value>* n = new Node<Key, Value>(new Key(*(node -> key)), new Value(*(node -> val)), parent);
    n -> height = node -> height;
    n -> left = copyTree(node -> left, n);
    n -> right = copyTree(node -> right, n);
    return n;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(const BSTree& other):
    nodes(0)
  {
    fakeLeaf = new Node<Key, Value>();
    fakeLeaf -> left = fakeLeaf;
    fakeLeaf -> right = fakeLeaf;
    fakeLeaf -> parent = fakeLeaf;
    fakeLeaf -> height = 0;
    fakeRoot = new Node<Key, Value>();
    fakeRoot -> left = fakeLeaf;
    fakeRoot -> right = fakeLeaf;
    fakeRoot -> parent = nullptr;
    fakeRoot -> height = 0;
    if (other.fakeRoot -> right && !other.fakeRoot -> right -> isLeaf())
    {
      fakeRoot -> right = copy(other.fakeRoot -> right, fakeRoot);
      nodes = other.nodes;
    }
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree& operator=(const BSTree& other)
  {
    if (this != &other)
    {
      destroyTree(fakeRoot -> right);
      fakeRoot -> right = fakeLeaf;
      nodes = 0;
      if (other.fakeRoot -> right && !other.fakeRoot -> right -> isLeaf())
      {
        fakeRoot -> right = copyTree(other.fakeRoot -> right, fakeRoot);
        nodes = other.nodes;
      }
    }
    return *this;
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value>* BSTree<Key, Value, Compare>::findByKey(const Key& k) const
  {
    Node<Key, Value>* curr = fakeRoot -> right;
    while (curr && !curr -> isLeaf())
    {
      if (cmp(k, *(curr->key)))
      {
        curr = curr -> left;
      }
      else if (cmp(*(curr -> key), k))
      {
        curr = curr -> right;
      }
      else
      {
        return curr;
      }
    }
    return fakeLeaf;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::updateHeight(Node<Key, Value>* node)
  {
    if (node && !(node -> isLeaf()))
    {
      int heightL = 0;
      int heightR = 0;
      if (node -> left && !node -> left -> isLeaf())
      {
        heightL = node -> left -> height;
      }
      if (node -> right && !node -> right -> isLeaf())
      {
        heightR = node -> right -> height;
      }
      if (heightL > heightR)
      {
        node -> height = 1 + heightL;
      }
      else
      {
        node -> height = 1 + heightR;
      }
    }
  }

  template <class Key, class Value, class Compare>
  int BSTree<Key, Value, Compare>::getBalance(Node<Key, Value>* node)
  {
    if (node && !(node -> isLeaf()))
    {
      int heightL = 0;
      int heightR = 0;
      if (node -> left && !node -> left -> isLeaf())
      {
        heightL = node -> left -> height;
      }
      if (node -> right && !node -> right -> isLeaf())
      {
        heightR = node -> right -> height;
      }
      return heightL - heightR;
    }
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator
  BSTree<Key, Value, Compare>::rotateLeft(const_iterator it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* rotChild = rotParent -> right;
    if (!rotChild || rotChild -> isLeaf())
    {
      return it;
    }
    rotChild -> parent = rotParent -> parent;
    rotChild -> left = rotParent;
    rotParent -> parent = rotChild;
    rotParent -> right = rotChild -> left;
    if (rotChild -> left && !rotChild -> left -> isLeaf())
    {
      rotChild -> left -> parent = rotParent;
    }
    updateHeight(rotParent);
    updateHeight(rotChild);
    if (rotChild -> parent == fakeRoot)
    {
      fakeRoot -> right = rotChild;
    }
    else if (rotChild -> parent -> left == rotParent)
    {
      rotChild -> parent -> left = rotChild;
    }
    else
    {
      rotChild -> parent -> right = rotChild;
    }
    return const_iterator(rotChild);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator
  BSTree<Key, Value, Compare>::rotateRight(const_iterator it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* rotChild = rotParent -> left;
    if (!rotChild || rotChild -> isLeaf())
    {
      return it;
    }
    rotChild -> parent = rotParent -> parent;
    rotChild -> right = rotParent;
    rotParent -> parent = rotChild;
    rotParent -> left = rotChild -> right;
    if (rotChild -> right && !rotChild -> right -> isLeaf())
    {
      rotChild -> right -> parent = rotParent;
    }
    updateHeight(rotParent);
    updateHeight(rotChild);
    if (rotChild -> parent == fakeRoot)
    {
      fakeRoot -> right = rotChild;
    }
    else if (rotChild -> parent -> left == rotParent)
    {
      rotChild -> parent -> left = rotChild;
    }
    else
    {
      rotChild -> parent -> right = rotChild;
    }
    return const_iterator(rotChild);
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator
  BSTree<Key, Value, Compare>::rotateLargeLeft(const_iterator it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    const_iterator newLeft = rotateLeft(const_iterator(rotParent -> left));
    rotParent -> left = newLeft.getNode();
    if (rotParent -> left && !rotParent -> left -> isLeaf())
    {
      rotParent -> left -> parent = rotParent;
    }
    return rotateRight(const_iterator(rotParent));
  }

  template <class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator
  BSTree<Key, Value, Compare>::rotateLargeRight(const_iterator it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    const_iterator newRight = rotateRight(const_iterator(rotParent -> right));
    rotParent -> right = newRight.getNode();
    if (rotParent -> right && !rotParent -> right -> isLeaf())
    {
      rotParent -> right -> parent = rotParent;
    }
    return rotateLeft(const_iterator(rotParent));
  }
}

#endif
