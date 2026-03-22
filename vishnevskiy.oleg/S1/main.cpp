#include "ListTools.hpp"
#include <iostream>
#include <string>

template <class T>
T read(std::istream& s)
{
  T val << s;
  return val;
}

template <class T>
bool isNumber(T var)
{
  return std::is_arithmetic<T>::value;
}

int main()
{
  size_t seqSize = 0;
  size_t currSize = 0;
  List<List<T>*>* lhead = nullptr;
  auto lIt = begin(lhead);
  List<T>* first = nullptr;
  while (std::cin)
  {
    auto val = read(std::cin);
    if (isNumber(val))
    {
      currSize++;
    }
    else
    {
      currSize = 1;
      lIt = begin(lhead);
    }
    if (currSize > seqSize)
    {
      List<T>* embed = new List<T>* {val, nullptr};
      insert(lIt, embed);
    }
  }
  lIt = begin(lhead);
  while (lIt)
  {
    if (lIt.value)
    {
      while (lIt.value)
      {
        std::cout << lIt.value.value << "\n";
      }
    }
  }
}
