#ifndef STACKIMPL_HPP
#define STACKIMPL_HPP
#include "Stack.hpp"

namespace Tools
{
  template <class T>
  Stack<T>::Stack():
    fake(new LIter<T>(new List<T>))
  {}

  template <class T>
  Stack<T>::~Stack()
  {
    fake -> clear(fake);
    delete fake;
  }

  template <class T>
  void Stack<T>::push(T rhs)
  {
    fake -> insert(rhs);
  }

  template <class T>
  T Stack<T>::drop()
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
