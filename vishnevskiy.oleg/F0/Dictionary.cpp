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

  std::string DictionaryManager::makeKey(const std::string& word, const std::string& part)
  {
    return word + "|" + part;
  }

  std::pair<std::string, std::string> DictionaryManager::splitKey(const std::string& key)
  {
    size_t part = key.find('|');
    return std::make_pair(key.substr(0, part), key.substr(part + 1));
  }

  bool DictionaryManager::exists(const std::string& dictName) const
  {
    return dictionaries.has(dictName);
  }

  bool DictionaryManager::isValid(const std::string& part)
  {
    static const std::string validParts[] = {"noun", "verb", "adjective", "adverb", "pronoun", "preposition", "conjunction", "interjection"};
    static const size_t numParts = 8;
    for (size_t i = 0; i < numParts; ++i)
    {
      if (part == validParts[i])
      {
        return true;
      }
    }
    return false;
  }

  bool DictionaryManager::isInList(const List<std::string>& list, const std::string& translation) const
  {
    LCIter<std::string> it(&list);
    while (it.hasNext())
    {
      if (*(it.value()) == translation)
        return true;
      ++it;
    }
    return false;
  }

  void DictionaryManager::addToList(List<std::string>& list, const std::string& translation)
  {
    if (!isInList(list, translation))
    {
      LIter<std::string> it(&list);
      it.end();
      it.insert(translation);
    }
  }

  void DictionaryManager::copyList(List<std::string>& dest, const List<std::string>& src)
  {
    dest.val = src.val;
    dest.next = nullptr;
    if (src.next)
    {
      LCIter<std::string> it(&src);
      LIter<std::string> destIt(&dest);
      while (it.hasNext())
      {
        ++it;
        destIt.insert(*(it.value()));
        ++destIt;
      }
    }
  }

  void DictionaryManager::printList(const List<std::string>& list, std::ostream& out)
  {
    LCIter<std::string> it(&list);
    bool first = true;
    while (it.hasNext())
    {
      if (!first)
      {
        out << ", ";
      }
      out << *(it.value());
      first = false;
      ++it;
    }
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
        std::string part = parts[2];

        if (!isValid(part))
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

        std::string key = makeKey(word, part);
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
    if (!exists(dict1) || !exists(dict2))
    {
      throw std::runtime_error("Dictionaries do not exist");
    }
    if (exists(result))
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
          LCIter<std::string> itNew(&newList);
          while (itNew.hasNext())
          {
            addToList(existing, *(itNew.value()));
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

  void DictionaryManager::find(const std::string& dictName, const std::string& part, const std::string& word)
  {
    if (!exists(dictName))
    {
      throw std::runtime_error("Dictionary does not exist");
    }
    dict_t* dict = dictionaries.at(dictName);

    if (part == "all")
    {
      bool found = false;
      List<std::string> allTranslations;
      bool firstTrans = true;

      vishnevskiy::tableIt<std::string, List<std::string>, stringHash_t, stringEq_t> it(dict);
      while (it.hasNext())
      {
        std::string key = it.key();
        std::pair<std::string, std::string> wordPos = splitKey(key);
        if (wordPos.first == word)
        {
          found = true;
          List<std::string> current = it.val();
          LCIter<std::string> itCurrent(&current);
          while (itCurrent.hasNext())
          {
            if (firstTrans)
            {
              allTranslations.val = *(itCurrent.value());
              allTranslations.next = nullptr;
              firstTrans = false;
            }
            else
            {
              addToList(allTranslations, *(itCurrent.value()));
            }
            ++itCurrent;
          }
        }
        it.next();
      }

      if (found)
      {
        std::cout << word << " ";
        printList(allTranslations, std::cout);
        std::cout << "\n";
      }
      else
      {
        throw std::runtime_error("Word not found");
      }
    }
    else
    {
      if (!isValid(part))
      {
        throw std::runtime_error("Invalid part of speech");
      }
      std::string key = makeKey(word, part);
      if (dict->has(key))
      {
        List<std::string> translations = dict->at(key);
        std::cout << word << " " << part << " ";
        printList(translations, std::cout);
        std::cout << "\n";
      }
      else
      {
        throw std::runtime_error("Word not found");
      }
    }
  }

  void DictionaryManager::save(const std::string& filename, const std::string& dictName)
  {
    if (!dictExists(dictName))
    {
      throw std::runtime_error("Dictionary does not exist");
    }
    std::ofstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Can't create file");
    }
    dict_t* dict = dictionaries.at(dictName);

    vishnevskiy::tableIt<std::string, List<std::string>, stringHash_t, stringEq_t> it(dict);
    while (it.hasNext())
    {
      std::string key = it.key();
      std::pair<std::string, std::string> wordPart = splitKey(key);
      List<std::string> translations = it.val();

      file << "(" << wordPart.first << "|";

      LCIter<std::string> itTrans(&translations);
      bool first = true;
      while (itTrans.hasNext())
      {
        if (!first)
        {
          file << ",";
        }
        file << *(itTrans.value());
        first = false;
        ++itTrans;
      }

      file << "|" << wordPart.second << ")\n";

      it.next();
    }
    file.close();
  }

}
