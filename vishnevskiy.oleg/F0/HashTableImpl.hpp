#ifndef HASHTABLEIMPL_HPP
#define HASHTABLEIMPL_HPP

#include <boost/hash2/siphash.hpp>
#include "HashTable.hpp"
#include <cstddef>
#include <stdexcept>

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
    size_t currPsl = 0;
    for (size_t i = 0; i < cap; ++i)
    {
      if (tombstone[ind] && psl[ind] == 0)
      {
        return cap+1;
      }
      if (!tombstone[ind] && eq(keys[ind], key))
      {
        return ind;
      }
      if (psl[ind] < currPsl)
      {
        return cap+1;
      }
      ind = probe(ind);
      ++currPsl;
    }
    return cap+1;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::findFree(const Key& key, size_t& resPsl)
  {
    size_t ind = getIndex(key);
    size_t currPsl = 0;
    size_t firstTombstone = cap;
    bool hasTombstone = false;

    for (size_t i = 0; i < cap; ++i)
    {
      if (tombstone[ind] && psl[ind] == 0)
      {
        if (hasTombstone)
        {
          resPsl = psl[firstTombstone];
          return firstTombstone;
        }
        resPsl = currPsl;
        return ind;
      }
      if (tombstone[ind] && !hasTombstone)
      {
        firstTombstone = ind;
        hasTombstone = true;
      }
      if (!tombstone[ind] && eq(keys[ind], key))
      {
        resPsl = psl[ind];
        return ind;
      }
      if (!tombstone[ind] && psl[ind] < currPsl)
      {
        if (hasTombstone)
        {
          resPsl = psl[firstTombstone];
          return firstTombstone;
        }
        resPsl = currPsl;
        return ind;
      }
      ind = probe(ind);
      ++currPsl;
    }
    if (hasTombstone)
    {
      resPsl = psl[firstTombstone];
      return firstTombstone;
    }
    return cap+1;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::add(const Key& k, const Value& v)
  {
    size_t currPsl = 0;
    size_t curr = findFree(k, currPsl);

    if (curr > cap)
    {
      throw std::runtime_error("Table is full");
    }
    if (!tombstone[curr] == 1 && eq(keys[curr], k))
    {
      values[curr] = v;
      return;
    }
    Key currKey = k;
    Value currVal = v;

    for (size_t i = 0; i < cap; ++i)
    {
      if (tombstone[curr] || psl[curr] < currPsl)
      {
        if (tombstone[curr])
        {
          keys[curr] = currKey;
          values[curr] = currVal;
          tombstone[curr] = false;
          psl[curr] = currPsl;
          ++size;
          return;
        }

        Key tmpKey = currKey;
        currKey = keys[curr];
        keys[curr] = tmpKey;

        Value tmpVal = currVal;
        currVal = values[curr];
        values[curr] = tmpVal;

        size_t tmpPsl = currPsl;
        currPsl = psl[curr];
        psl[curr] = tmpPsl;
      }
      curr = probe(curr);
      ++currPsl;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  Value HashTable<Key, Value, Hash, Equal>::drop(Key k)
  {
    size_t curr = findByKey(k);

    if (curr <= cap && !tombstone[curr])
    {
      Value result = values[curr];
      tombstone[curr] = true;
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
    bool* tombCopy = tombstone;
    size_t* pslCopy = psl;
    size_t capCopy = cap;

    keys = nullptr;
    values = nullptr;
    tombstone = nullptr;
    psl = nullptr;
    size = 0;
    cap = slots;
    createEls(cap);

    try
    {
      for (size_t i = 0; i < capCopy; ++i)
      {
        if (!tombCopy[i])
        {
          add(keysCopy[i], valuesCopy[i]);
        }
      }
    }
    catch (...)
    {
      delete[] keysCopy;
      delete[] valuesCopy;
      delete[] tombCopy;
      delete[] pslCopy;
      throw;
    }

    delete[] keysCopy;
    delete[] valuesCopy;
    delete[] tombCopy;
    delete[] pslCopy;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::createEls(size_t capacity)
  {
    Key* keyPtr = nullptr;
    Value* valPtr = nullptr;
    bool* tombPtr = nullptr;
    size_t* pslPtr = nullptr;

    try
    {
      keyPtr = new Key[capacity];
      valPtr = new Value[capacity];
      tombPtr = new bool[capacity];
      pslPtr = new size_t[capacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] keyPtr;
      delete[] valPtr;
      delete[] tombPtr;
      delete[] pslPtr;
      throw e;
    }

    for (size_t i = 0; i < capacity; ++i)
    {
      tombPtr[i] = true;
      pslPtr[i] = 0;
    }
    keys = keyPtr;
    values = valPtr;
    tombstone = tombPtr;
    psl = pslPtr;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(size_t capacity, Hash hash_f, Equal eq_f):
    keys(nullptr),
    values(nullptr),
    tombstone(nullptr),
    psl(nullptr),
    hash(hash_f),
    eq(eq_f),
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
    delete[] tombstone;
    delete[] psl;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable& other):
    keys(nullptr),
    values(nullptr),
    flags(nullptr),
    tombstone(nullptr),
    psl(nullptr),
    hash(other.hash),
    eq(other.eq),
    size(other.size),
    cap(other.cap)
  {
    createEls(cap);

    try
    {
      for (size_t i = 0; i < cap; ++i)
      {
        tombstone[i] = other.tombstone[i];
        psl[i] = other.psl[i];
        if (!tombstone[i])
        {
          keys[i] = other.keys[i];
          values[i] = other.values[i];
        }
      }
    }
    catch (...)
    {
      delete[] keys;
      delete[] values;
      delete[] tombstone;
      delete[] psl;
      throw;
    }
  }

    try
    {
      Key* newKeys = nullptr;
      Value* newValues = nullptr;
      bool* newTomb = nullptr;
      size_t* newPsl = nullptr;
      try
      {
        newKeys = new Key[other.cap];
        newValues = new Value[other.cap];
        newTomb = new bool[other.cap];
        newPsl = new size_t[other.cap];
        for (size_t i = 0; i < other.cap; ++i)
        {
          newTomb[i] = true;
          newPsl[i] = 0;
        }
        for (size_t i = 0; i < other.cap; ++i)
        {
          newTomb[i] = other.tombstone[i];
          newPsl[i] = other.psl[i];
          if (!newTomb[i])
          {
            newKeys[i] = other.keys[i];
            newValues[i] = other.values[i];
          }
        }
      }
      catch (...)
      {
        delete[] newKeys;
        delete[] newValues;
        delete[] newTomb;
        delete[] newPsl;
        throw;
      }

      delete[] keys;
      delete[] values;
      delete[] tombstone;
      delete[] psl;
      keys = newKeys;
      values = newValues;
      tombstone = newTomb;
      psl = newPsl;
      cap = other.cap;
      size = other.size;
      hash = other.hash;
      eq = other.eq;
    }
    catch (...)
    {
      delete[] keys;
      delete[] values;
      delete[] flags;
      throw;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>&
  HashTable<Key, Value, Hash, Equal>::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      Key* newKeys = nullptr;
      Value* newValues = nullptr;
      size_t* newFlags = nullptr;
      try
      {
        newKeys = new Key[other.cap];
        newValues = new Value[other.cap];
        newFlags = new size_t[other.cap];
        for (size_t i = 0; i < other.cap; ++i)
        {
          newFlags[i] = 0;
        }
        for (size_t i = 0; i < other.cap; ++i)
        {
          newFlags[i] = other.flags[i];
          if (newFlags[i] == 1)
          {
            newKeys[i] = other.keys[i];
            newValues[i] = other.values[i];
          }
        }
      }
      catch (...)
      {
        delete[] newKeys;
        delete[] newValues;
        delete[] newFlags;
        throw;
      }

      delete[] keys;
      delete[] values;
      delete[] flags;
      keys = newKeys;
      values = newValues;
      flags = newFlags;
      cap = other.cap;
      size = other.size;
      hash = other.hash;
      eq = other.eq;
    }
    return *this;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getSize() const
  {
    return size;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getCapacity() const
  {
    return cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  Value& HashTable<Key, Value, Hash, Equal>::at(const Key& k)
  {
    size_t curr = findByKey(k);

    if (curr <= cap && !tombstone[curr])
    {
      return values[curr];
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  const Value& HashTable<Key, Value, Hash, Equal>::at(const Key& k) const
  {
    size_t curr = findByKey(k);

    if (curr <= cap && !tombstone[curr])
    {
      return values[curr];
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>::tableIt():
    table(nullptr),
    curr(0)
  {}

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>::tableIt(const HashTable<Key, Value, Hash, Equal>* InitialTable):
    table(InitialTable),
    curr(0)
  {
    while (curr < table->getCapacity() && table->tombstone[curr])
    {
      curr++;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  void tableIt<Key, Value, Hash, Equal>::next()
  {
    if (table && curr < table->cap)
    {
      curr++;
      while (curr < table->cap && table->tombstone[curr])
      {
        curr++;
      }
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  bool tableIt<Key, Value, Hash, Equal>::hasNext()
  {
    if (table && curr >= table->cap)
    {
      return false;
    }
    return true;
  }

  template <class Key, class Value, class Hash, class Equal>
  Value& tableIt<Key, Value, Hash, Equal>::val()
  {
    return table->values[curr];
  }

  template <class Key, class Value, class Hash, class Equal>
  Key& tableIt<Key, Value, Hash, Equal>::key()
  {
    return table->keys[curr];
  }
}

#endif
