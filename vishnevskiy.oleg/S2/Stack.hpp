#ifndef STACK_HPP
#define STACK_HPP
#include "ListImpl.hpp"

namespace Tools
{
  template <class T>
  class Stack
  {
    public:
      void push(T rhs);
      T drop();
      Stack();
      ~Stack();
    private:
      LIter<T>* fake;
  };
}

#endif
