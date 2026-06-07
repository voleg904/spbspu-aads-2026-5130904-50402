#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <boost/hash2/siphash.hpp>

namespace vishnevskiy
{
  template <class Key, class Value, class Hash, class Equal>
  class HashTable
  {
    private:
      Key* keys;
      Value* values;
      size_t* flags;
      Hash hash;
      Equal eq;
      size_t size;
      size_t cap;

      size_t getIndex(const Key& key);
      size_t probe(size_t ind);
      size_t findByKey(const Key& key);
      size_t findFree(const Key& key);

    public:
      HashTable(size_t capacity);
      ~HashTable();
      void add(Key& k, Value& v);
      Value drop(Key k);
      bool has(Key k);
      void rehash(size_t slots);
  };
}

#endif
