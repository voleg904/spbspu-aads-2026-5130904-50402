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

  public:
    DictionaryManager(size_t capacity, stringHash_t hash, stringEq_t eq);
    ~DictionaryManager();

    void load(const std::string& filename, const std::string& dictName);
    void deleteDict(const std::string& dictName);
    void merge(const std::string& dict1, const std::string& dict2, const std::string& result);
    void find(const std::string& dictName, const std::string& pos, const std::string& word);
    void save(const std::string& filename, const std::string& dictName);
    void addWord(const std::string& dictName, const std::string& word, const std::string& pos, const std::string& translations);
    void addTranslation(const std::string& dictName, const std::string& word, const std::string& translation);
    void deleteWord(const std::string& dictName, const std::string& word);
    void deleteTranslation(const std::string& dictName, const std::string& word, const std::string& translation);
    void changePos(const std::string& dictName, const std::string& word, const std::string& newPos);
    void findTranslation(const std::string& dictName, const std::string& translation);
  };
}

#endif
