#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "ListImpl.hpp"

namespace Tools
{
  template <class T>
  class Queue
  {
    public:
      void push(T rhs);
      T drop();
      Queue();
      ~Queue();
    private:
      LIter<T>* fake;
      LIter<T>* last;
  };
}

#endif
