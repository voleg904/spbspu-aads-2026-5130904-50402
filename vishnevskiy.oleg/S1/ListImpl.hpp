#ifndef LISTIMPL_HPP
#define LISTIMPL_HPP
#include <cstddef>
#include <string>
#include "ListTools.hpp"

namespace ListTools
{
  template <class T>
  LIter<T>::LIter(): 
    curr(nullptr)
  {}

  template <class T>
  LIter<T>::LIter(List<T>* h): 
    curr(h)
  {}

  template <class T>
  T* LIter<T>::value()
  {
    return (curr->val);
  }

  template <class T>
  void LIter<T>::operator++()
  {
    curr = curr -> next;
  }

  template <class T>
  bool LIter<T>::hasNext()
  {
    return curr -> next != nullptr;
  }

  template <class T>
  void LIter<T>::set(List<T>* h)
  {
    curr = h;
  }

  template <class T>
  void LIter<T>::end()
  {
    while (hasNext())
    {
      this++;
    }
  }

  template <class T>
  void LIter<T>::insert(T& d)
  {
    List<T>* s = new List<T>{d, nullptr};
    curr -> next = s;
    if (curr -> next)
    {
      s -> next = curr -> next;
    }
  }
  
  template <class T>
  void LIter<T>::clear(LIter<T>* head)
  {
    LIter<T>* prev = nullptr;
    while (head -> curr)
    {
      prev = head;
      head.curr = head.curr -> next;
      delete prev;
    }
  }

  template <class T>
  NamedLIter<T>::NamedLIter(): 
    curr(nullptr)
  {}

  template <class T>
  NamedLIter<T>::NamedLIter(NamedList<T>* h): 
    curr(h)
  {}

  template <class T>
  List<T>* NamedLIter<T>::value()
  {
    return curr -> data;
  }

  template <class T>
  std::string NamedLIter<T>::getName()
  {
    return curr -> name;
  }

  template <class T>
  void NamedLIter<T>::operator++()
  {
    curr = curr -> next;
  }

  template <class T>
  bool NamedLIter<T>::hasNext()
  {
    return curr -> next != nullptr;
  }

  template <class T>
  void NamedLIter<T>::setData(NamedList<T>* h)
  {
    curr = h;
  }

  template <class T>
  void NamedLIter<T>::end()
  {
    while (hasNext())
    {
      this++;
    }
  }

  template <class T>
  void NamedLIter<T>::insert(List<T>* d, std::string name)
  {
    NamedList<T>* s = new NamedList<T>{name, d, nullptr};
    curr -> next = s;
    if (curr -> next)
    {
      s -> next = curr -> next;
    }
  }
  
  template <class T>
  void NamedLIter<T>::clear(NamedLIter<T>* head)
  {
    NamedLIter<T>* prev = nullptr;
    while (head -> curr)
    {
      prev = head;
      head.curr = head.curr -> next;
      delete prev;
    }
  }

  template <class T>
  LCIter<T>::LCIter():
    curr(nullptr)
  {}

  template <class T>
  LCIter<T>::LCIter(const List<T>* h):
    curr(h)
  {}

  template <class T>
  void LCIter<T>::operator++()
  {
    curr = curr -> next;
  }

  template <class T>
  T* LCIter<T>::value()
  {
    return (curr->val);
  }

  template <class T>
  bool LCIter<T>::hasNext()
  {
    return curr -> next != nullptr;
  }
}
#endif
