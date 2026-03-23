#ifndef LISTIMPL_HPP
#define LISTIMPL_HPP
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
  T* LIter<T>::operator->()
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

  void LIter<T>::set(List<T>* h)
  {
    curr = List<T>* h;
  }

  template <class T>
  void LIter<T>::end()
  {
    while (hasNext())
    {
      this++;
    }
    return *this;
  }

  template <class T>
  void LIter::insert(LIter<T>* node, T& d)
  {
    List<T>* s = new List<T>{d};
    node -> curr -> next = s;
    if (node -> curr -> next)
    {
      s -> next = node -> curr -> next 
    }
  }
  
  template <class T>
  void LIter::clear(LIter<T>* head)
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
  List<T>* NamedLIter<T>::operator->()
  {
    return curr -> data;
  }

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

  void NamedLIter<T>::setData(List<T>* h)
  {
    curr = List<T>* h;
  }

  template <class T>
  void LIter<T>::end()
  {
    while (hasNext())
    {
      this++;
    }
    return *this;
  }

  template <class T>
  void LIter::insert(LIter<T>* node, T& d)
  {
    List<T>* s = new List<T>{d};
    node -> curr -> next = s;
    if (node -> curr -> next)
    {
      s -> next = node -> curr -> next 
    }
  }
  
  template <class T>
  void LIter::clear(LIter<T>* head)
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
  LCIter<T>::LCIter(): 
    curr(nullptr)
  {}

  template <class T>
  LCIter<T>::LCIter(List<T>* h): 
    curr(h)
  {}

  template <class T>
  void LCIter<T>::operator++()
  {
    curr = curr -> next;
  }

  template <class T>
  bool LCIter<T>::hasNext()
  {
    return curr -> next != nullptr;
  }
}
#endif
