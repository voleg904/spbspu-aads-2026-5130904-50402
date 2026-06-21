#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <stdexcept>

namespace vishnevskiy
{
  template <class Key, class Value, class Compare>
  class BSTree;

  template <class Key, class Value>
  class BSIterator
  {
    private:
      Node<Key, Value>* curr;
    public:
      void next();
      void prev();
      Value getVal();
      Node<Key, Value>* getNode() const;
  };

  template <class Key, class Value>
  class BSConstIterator
  {
    private:
      BSTree<Key, Value, Compare>* curr;
    public:
      void next();
      void prev();
      const Value getVal() const;
      const Node<Key, Value>* getNode() const;
  };

  template <class Key, class Value>
  struct Node
  {
    private:
      Key* key;
      Value* val;
      Node* parent;
      Node* left;
      Node* right;
      size_t height;
    public:
      Node();
      Node(const Key& k, const Value& v, Node* p);
      ~Node();
      friend class BSTree<Key, Value, Compare>;
  };

  template <class Key, class Value, class Compare>
  class BSTree
  {
    private:
      Node<Key, Value>* fakeRoot;
      Node<Key, Value>* fakeLeaf;
      Compare cmp;
      size_t nodes;
      Node<Key, Value>* findByKey(const Key& k) const;
    public:
      BSTree();
      ~BSTree();
      BSTree(const BSTree& other);
      BSTree& operator=(const BSTree& other);
      bool has(const Key& k) const;
      void push(Key k, Value v);
      Value get(Key k);
      Value drop(Key k);

      using const_iterator = BSTConstIterator< Key, Value >;
      const_iterator rotateLeft(const_iterator it);
      const_iterator rotateRight(const_iterator it);
      const_iterator rotateLargeLeft(const_iterator it);
      const_iterator rotateLargeRight(const_iterator it);
      size_t height(const_iterator it);
      size_t height();
  };
}

#endif
