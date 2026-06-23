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
}
