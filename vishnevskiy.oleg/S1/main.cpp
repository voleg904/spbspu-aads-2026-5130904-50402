#include "ListImpl.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

bool isNumber(std::string& str)
{
  for (char ch : str) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

bool hasDepth(ListTools::LIter<int> em, size_t depth)
{
  size_t d = 0;
  while (em.curr)
  {
    if (depth == d)
    {
      return true;
    }
    d++;
    ++em;
  }
  return false;
}

void printNames(ListTools::NamedLIter<int> lt)
{
  while (lt.hasNext())
  {
    std::cout << lt.getName() << " ";
    ++lt;
  }
  std::cout << lt.getName() << " ";
}

bool printEmbed(ListTools::LIter<int> em, size_t depth, int& sm)
{
  for (size_t i = 0; i < depth; i++)
  {
    if (!em.hasNext())
    {
      return false;
    }
    ++em;
  }
  if (em.curr)
  {
    std::cout << em.value() << " ";
    sm += em.value();
    return true;
  }
  return false;
}

void printSeq(ListTools::NamedLIter<int> lt, size_t depth, int* sums, size_t index)
{
  bool f = false;
  int sm = 0;
  ListTools::NamedLIter<int> currIt = lt;
  while (currIt.curr)
  {
    if (currIt.value())
    {
      ListTools::LIter<int> temp(currIt.value());
      if (hasDepth(temp, depth))
      {
        f = printEmbed(temp, depth, sm);
      }
    }
    ++currIt;
  }
  if (f)
  {
    std::cout << "\n";
    sums[index] = sm;
    printSeq(lt, depth+1, sums, index+1);
  }
}

int main()
{
  ListTools::NamedList<int>* lhead = nullptr;
  ListTools::NamedLIter<int> lIt(lhead);
  ListTools::LIter<int> embedIt(nullptr);
  std::string data;
  size_t lSize = 0;
  size_t cSize = 0;
  while (std::cin >> data)
  {
    if (isNumber(data))
    {
      int number = std::stoi(data);
      if (!lIt.value())
      {
        ListTools::List<int>* embed = new ListTools::List<int>{number, nullptr};
        embedIt.set(embed);
        lIt.setData(embedIt.curr);
      }
      else
      {
        //try
        embedIt.insert(number);
        ++embedIt;
      }
      cSize++;
      if (cSize > lSize)
      {
        lSize = cSize;
      }
    }
    else
    {
      cSize = 0;
      if (!lhead)
      {
        //try
        lhead = new ListTools::NamedList<int>{data, nullptr, nullptr};
        lIt.setCurr(lhead);
      }
      else
      {
        //try
        lIt.insert(nullptr, data);
        ++lIt;
      }
    }
  }

  //try
  int* sums = new int[lSize];
  lIt.setCurr(lhead);
  printNames(lIt);
  std::cout << "\n";
  printSeq(lIt, 0, sums, 0);
  for (size_t i = 0; i < lSize; i++)
  {
    std::cout << sums[i] << " ";
  }
  std::cout << "\n";
  delete[] sums;
}
