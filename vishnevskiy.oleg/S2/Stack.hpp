#ifndef STACK_HPP
#define STACK_HPP
#include "ListImpl.hpp"

namespace vishnevskiy
{
  template <class T>
  class Stack
  {
    public:
      void push(T rhs);
      T drop();
    private:
      LIter<T> fake;
  };
}

#endif
