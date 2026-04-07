#ifndef QUEUEIMPL_HPP
#define QUEUEIMPL_HPP
#include "Queue.hpp"

namespace Tools
{
  template <class T>
  Queue<T>::Queue():
    fake(new LIter<T>(new List<T>)),
    last(nullptr)
  {}

  template <class T>
  Queue<T>::~Queue()
  {
    fake -> clear(fake);
    delete fake;
  }

  template <class T>
  void Queue<T>::push(T rhs)
  {
    if (!last)
    {
      fake -> insert(rhs);
      last = fake;
      ++last;
    }
    else
    {
      last -> end();
      last -> insert(rhs);
      ++last;
    }
  }

  template <class T>
  T Queue<T>::drop()
  {
    LIter<T> temp;
    temp.set(fake->curr);
    ++temp;
    if (!temp.curr)
    {
      throw std::runtime_error("No data to drop!");
    }
    T tempVal = temp.value();
    fake -> curr -> next = temp.curr -> next;
    delete temp.curr;
    return tempVal;
  }
}

#endif
