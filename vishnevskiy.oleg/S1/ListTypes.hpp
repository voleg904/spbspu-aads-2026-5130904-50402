#ifndef LISTTYPES_HPP
#define LISTTYPES_HPP
#include "ListTools.hpp"
#include <string>
#include <cstddef>

namespace ListTypes
{
  template <class T>
  class NamedList
  {
    std::string name;
    List<T>* data;
  }
}
#endif
