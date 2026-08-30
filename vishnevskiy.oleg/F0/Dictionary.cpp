#include "Dictionary.hpp"
#include <iostream>
#include <cstring>

namespace vishnevskiy
{
  using stringHash_t = size_t(*)(const std::string&);
  using stringEq_t = bool(*)(const std::string&, const std::string&);

  size_t stringHasher(const std::string& s)
  {
    boost::hash2::siphash_64 hash;
    hash.update(s.data(), s.size());
    return hash.result();
  }

  bool stringEq(const std::string& a, const std::string& b)
  {
    return a == b;
  }

  DictionaryManager::DictionaryManager(size_t capacity, stringHash_t hash, stringEq_t eq):
    dictionaries(capacity, hash, eq), hashFunc(hash), eqFunc(eq)
  {}

  DictionaryManager::~DictionaryManager()
  {
    vishnevskiy::tableIt<std::string, dict_t*, stringHash_t, stringEq_t> it(&dictionaries);
    while (it.hasNext())
    {
      delete it.val();
      it.next();
    }
  }
}
