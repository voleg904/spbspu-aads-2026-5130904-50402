#include "Dictionary.hpp"
#include <iostream>
#include <cstring>

namespace vishnevskiy
{
  using stringHash_t = size_t(*)(const std::string&);
  using stringEq_t = bool(*)(const std::string&, const std::string&);
  using dict_t = HashTable<std::string, List<std::string>, stringHash_t, stringEq_t>;

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

  bool DictionaryManager::exists(const std::string& dictName) const
  {
    return dictionaries.has(dictName);
  }

  void DictionaryManager::load(const std::string& filename, const std::string& dictName)
  {
    if (exists(dictName))
    {
      throw std::runtime_error("Dictionary with this name already exists");
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("File does not exist or cannot be opened");
    }
    dict_t* newDict = new dict_t(64, hashFunc, eqFunc);
    std::string line;

    while (std::getline(file, line))
    {
      if (!line.empty())
      {
        if (line[0] != '(' || line[line.length() - 1] != ')')
        {
          delete newDict;
          throw std::runtime_error("Invalid dictionary format");
        }

        std::string content = line.substr(1, line.length() - 2);

        std::string parts[3];
        size_t partCount = 0;
        size_t start = 0;
        size_t end = content.find('|');

        while (end != std::string::npos && partCount < 3)
        {
          parts[partCount++] = content.substr(start, end - start);
          start = end + 1;
          end = content.find('|', start);
        }
        if (partCount < 3)
        {
          parts[partCount++] = content.substr(start);
        }
        if (partCount != 3)
        {
          delete newDict;
          throw std::runtime_error("Invalid dictionary format");
        }

        std::string word = parts[0];
        std::string translationsStr = parts[1];
        std::string pos = parts[2];

        if (!isValidPos(pos))
        {
          delete newDict;
          throw std::runtime_error("Invalid part of speech");
        }

        List<std::string> transList;
        std::stringstream ss(translationsStr);
        std::string translation;
        bool first = true;

        while (std::getline(ss, translation, ','))
        {
          if (!translation.empty())
          {
            if (first)
            {
              transList.val = translation;
              transList.next = nullptr;
              first = false;
            }
            else
            {
              LIter<std::string> it(&transList);
              it.end();
              it.insert(translation);
            }
          }
        }

        if (first)
        {
          delete newDict;
          throw std::runtime_error("Word must have at least one translation");
        }

        std::string key = makeKey(word, pos);
        try
        {
          newDict->add(key, transList);
        }
        catch (...)
        {
          delete newDict;
          throw;
        }
      }
    }
    file.close();

    try
    {
      dictionaries.add(dictName, newDict);
    }
    catch (...)
    {
      delete newDict;
      throw;
    }
  }

  void DictionaryManager::deleteDict(const std::string& dictName)
  {
    if (!exists(dictName))
    {
      throw std::runtime_error("Dictionary does not exist");
    }
    dict_t* dict = dictionaries.at(dictName);
    delete dict;
    dictionaries.drop(dictName);
  }

  void DictionaryManager::merge(const std::string& dict1, const std::string& dict2, const std::string& result)
  {
    if (!dictExists(dict1) || !dictExists(dict2))
    {
      throw std::runtime_error("Dictionaries do not exist");
    }
    if (dictExists(result))
    {
      throw std::runtime_error("Dictionary already exists");
    }
    dict_t* dict1Ptr = dictionaries.at(dict1);
    dict_t* dict2Ptr = dictionaries.at(dict2);
    dict_t* resultDict = new dict_t(dict1Ptr->getCapacity() + dict2Ptr->getCapacity(), hashFunc, eqFunc);

    try
    {
      vishnevskiy::tableIt<std::string, List<std::string>, stringHash_t, stringEq_t> it1(dict1Ptr);
      while (it1.hasNext())
      {
        std::string key = it1.key();
        List<std::string> value;
        copyList(value, it1.val());
        resultDict->add(key, value);
        it1.next();
      }

      vishnevskiy::tableIt<std::string, List<std::string>, stringHash_t, stringEq_t> it2(dict2Ptr);
      while (it2.hasNext())
      {
        std::string key = it2.key();
        if (resultDict->has(key))
        {
          List<std::string> existing = resultDict->at(key);
          List<std::string> newList = it2.val();
          
          LIter<std::string> itNew(const_cast<List<std::string>*>(&newList));
          while (itNew.curr)
          {
            addTranslationToList(existing, itNew.value());
            ++itNew;
          }

          resultDict->add(key, existing);
        }
        else
        {
          List<std::string> value;
          copyList(value, it2.val());
          resultDict->add(key, value);
        }
        it2.next();
      }

      dictionaries.add(result, resultDict);
    }
    catch (...)
    {
      delete resultDict;
      throw;
    }
  }

}
