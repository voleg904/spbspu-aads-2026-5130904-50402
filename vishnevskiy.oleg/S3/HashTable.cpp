#include "NoteTools.hpp"

namespace vishnevskiy
{
  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getIndex(const Key& key)
  {
    return hash(key) % cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::probe(size_t ind)
  {
    return (ind + 1) % cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::findByKey(const Key& key)
  {
    size_t ind = getIndex(key);
    for (size_t i = 0; i < cap; ++i)
    {
      if (flags[ind] == 0)
      {
        return cap+1;
      }
      if (flags[ind] == 1 && eq(keys[ind], key))
      {
        return ind;
      }
      ind = probe(ind);
    }
    return cap+1;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::findFree(const Key& key)
  {
    size_t ind = getIndex(key);
    size_t firstTombstone = cap;
    bool hasTombstone = false;
    
    for (size_t i = 0; i < cap; ++i)
    {
      size_t f = flags[ind];
      if (f == 0)
      {
        if (hasTombstone)
        {
          return firstTombstone;
        }
        return ind;
      }
      else if (f == 2 && !hasTombstone)
      {
        firstTombstone = ind;
        hasTombstone = true;
      }
      else if (f == 1 && eq(keys[ind], key))
      {
        return ind;
      }
      ind = probe(ind);
    }
    if (hasTombstone)
    {
      return firstTombstone;
    }
    return cap+1;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::add(Key k, Value v)
  {
    
  }

  template <class Key, class Value, class Hash, class Equal>
  Value HashTable<Key, Value, Hash, Equal>::drop(Key k)
  {
    
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::has(Key k)
  {
    
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::rehash(size_t slots)
  {
    
  }
}
