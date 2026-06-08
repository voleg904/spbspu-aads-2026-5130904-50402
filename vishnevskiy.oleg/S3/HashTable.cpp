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
  void HashTable<Key, Value, Hash, Equal>::add(Key& k, Value& v)
  {
    size_t curr = findFree(k);

    if (curr > cap)
    {
      throw std::runtime_error("Table is full");
    }
    if (flags[curr] == 1 && eq(keys[curr], k))
    {
      values[curr] = val;
    }
    else
    {
      keys[curr] = k;
      values[curr] = v;
      flags[curr] = 1;
      size++;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  Value HashTable<Key, Value, Hash, Equal>::drop(Key k)
  {
    size_t curr = findFree(k);

    if (curr <= cap && flags[curr] == 1)
    {
      Value result = values[curr];
      flags[curr] = 2;
      size--;
      return result;
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::has(Key k)
  {
    return findByKey(k) <= cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::rehash(size_t slots)
  {
    if (slots < size)
    {
      throw std::runtime_error("Not enough slots");
    }

    Key* keysCopy = keys;
    Value* valuesCopy = values;
    size_t* flagsCopy = flags;
    size_t capCopy = cap;
    keys = new Key[slots];
    try
    {
      values = new Value[slots];
      flags = new size_t[slots];
    }
    catch (const std::bad_alloc& e)
    {
      throw e;
    }
    size = 0;

    for (size_t i = 0; i < cap; ++i)
    {
      flags[i] = 0;
    }
    for (size_t i = 0; i < capCopy; ++i)
    {
      if (flagsCopy[i] == 1)
      {
        add(keysCopy[i], valuesCopy[i]);
      }
    }
    delete[] keysCopy;
    delete[] valuesCopy;
    delete[] flagsCopy;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::createEls(size_t capacity)
  {
    Key* keyPtr = nullptr;
    Value* = valPtr = nullptr;
    size_t flagPtr = nullptr;

    try
    {
      keyPtr = new Key[capacity];
      valPtr = new Value[capacity];
      flagPtr = new size_t[capacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] keyPtr;
      delete[] valPtr;
      delete[] flagPtr;
      throw e;
    }

    for (size_t i = 0; i < capacity; ++i)
    {
      flagPtr[i] = 0;
    }
    keys = keyPtr;
    values = valPtr;
    flags = flagPtr;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(size_t capacity):
    keys(nullptr),
    values(nullptr),
    flags(nullptr),
    size(0),
    cap(capacity)
  {
    createEls(cap);
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::~HashTable()
  {
    delete[] keys;
    delete[] values;
    delete[] flags;
  }
}
