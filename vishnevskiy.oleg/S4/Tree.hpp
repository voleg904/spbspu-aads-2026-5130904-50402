#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>

namespace vishnevskiy
{
  template <class Key, class Value, class Compare>
  class BSTree;

  template <class Key, class Value>
  struct Node;

  template <class Key, class Value>
  class BSIterator
  {
    private:
      Node<Key, Value>* curr;
    public:
      BSIterator(const Node<Key, Value>* node);
      void next();
      void prev();
      void fallLeft();
      void fallRight();
      Value* getVal();
      Key* getKey();
      Node<Key, Value>* getNode();
      bool isEnd() const;
  };

  template <class Key, class Value>
  class BSConstIterator
  {
    private:
      Node<Key, Value>* curr;
    public:
      BSConstIterator(const Node<Key, Value>* node);
      void next();
      void prev();
      void fallLeft();
      void fallRight();
      const Value* getVal() const;
      const Key* getKey() const;
      const Node<Key, Value>* getNode() const;
      bool isEnd() const;
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
      Node(const Key* k, const Value* v, Node* p);
      ~Node();
      bool isLeaf() const;
      template <class K, class V, class C>
      friend class BSTree;
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
      void updateHeight(Node<Key, Value>* node);
      size_t calcBalance(Node<Key, Value>* node);
      Node<Key, Value>* copy(Node<Key, Value>* node,Node<Key, Value>* parent);
    public:
      BSTree();
      ~BSTree();
      BSTree(const BSTree& other);
      BSTree& operator=(const BSTree& other);
      bool has(const Key& k) const;
      void push(const Key& k, const Value& v);
      size_t getBalance(Node<Key, Value>* node);
      Value get(const Key& k);
      Value drop(const Key& k);

      using const_iterator = BSConstIterator<Key, Value>;
      const_iterator rotateLeft(const_iterator it);
      const_iterator rotateRight(const_iterator it);
      const_iterator rotateLargeLeft(const_iterator it);
      const_iterator rotateLargeRight(const_iterator it);
      size_t height(const_iterator it) const;
      size_t height() const;
  };
}

#endif
