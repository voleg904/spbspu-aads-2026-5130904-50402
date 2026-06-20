#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

namespace vishnevskiy
{
  template <class Key, class Value>
  class BSIterator
  {
    BSTree<Key, Value, Compare>* curr;
  };

  template <class Key, class Value>
  class BSConstIterator
  {
    BSTree<Key, Value, Compare>* curr;
  };

  template <class Key, class Value, class Compare>
  class BSTree
  {
    private:
      BSTree<Key, Value, Compare>* parent;
      BSTree<Key, Value, Compare>* left;
      BSTree<Key, Value, Compare>* right;
      Value* val;
    public:
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
