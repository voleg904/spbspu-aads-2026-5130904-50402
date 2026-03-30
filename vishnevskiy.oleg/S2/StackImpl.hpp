#ifndef STACKIMPL_HPP
#define STACKIMPL_HPP
#include "Stack.hpp"

namespace vishnevskiy
{
  template <class T>
  Stack<T>::Stack():
    fake(new List<T>)
  {}

  template <class T>
  void Stack<T>::push(T rhs)
  {
    fake.insert(rhs);
  }

  template <class T>
  T drop()
  {
    LIter<T>* temp = fake;
    ++temp;
    T tempVal = temp.value();
    delete temp->curr;
    delete temp;
    return tempVal;
  }
}

#endif
