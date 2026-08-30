#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "HashTableImpl.hpp"
#include "ListImpl.hpp"

namespace vishnevskiy
{
  using stringHash_t = size_t(*)(const std::string&);
  using stringEq_t = bool(*)(const std::string&, const std::string&);

  using dict_t = HashTable<std::string, List<std::string>, stringHash_t, stringEq_t>;

  class DictionaryManager
  {
    private:
      HashTable<std::string, dict_t*, stringHash_t, stringEq_t> dictionaries;
      stringHash_t hashFunc;
      stringEq_t eqFunc;


  };
}

#endif
