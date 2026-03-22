#ifndef LISTTOOLS_HPP
#define LISTTOOLS_HPP
namespace ListTools
{
  template <class T>
  class List
  {
    private:
      T val;
      List<T>* next;
  };

  template <class T>
  class LIter
  {
    friend class List<T>;
    List<T>* curr;
    LIter<T> begin(List<T>* h);
    T value(LIter<T> it);
    LIter<T> next(LIter<T> it);
    bool hasNext(LIter<T> it);
    LIter<T> end(LIter<T> it);
    LIter<T>* insert(LIter<T>* node, T& d);
    void clear(LIter<T>* head);
  };

  template <class T>
  LIter<T> LIter::begin(List<T>* h)
  {
    return {h};
  }

  template <class T>
  T LIter::value(LIter<T> it)
  {
    return (it -> curr.val);
  }

  template <class T>
  LIter<T> LIter::next(LIter<T> it)
  {
    it.curr = it.curr -> next;
    return it;
  }

  template <class T>
  bool LIter::hasNext(LIter<T> it)
  {
    return it.curr;
  }

  template <class T>
  LIter<T> LIter::end(LIter<T> it)
  {
    while (it.curr -> next)
    {
      it.curr = it.curr -> next;
    }
    return it;
  }

  template <class T>
  LIter<T>* LIter::insert(LIter<T> node, T& d)
  {
    List<T>* s = new List<T>{d};
    node -> next = s;
    return s;
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
  class LCIter
  {
    friend class List<T>;
    const List<T>* curr;
    LCIter<T> begin(const List<T>* h);
    LCIter<T> next(LCIter<T> it);
    bool hasNext(LCIter<T> it);
  };

  template <class T>
  LCIter<T> LCIter::begin(const List<T>* h)
  {
    return {h};
  }

  template <class T>
  LCIter<T> LCIter::next(LCIter<T> it)
  {
    it.curr = it.curr -> next;
    return it;
  }

  template <class T>
  bool LCIter::hasNext(LCIter<T> it)
  {
    return it.curr;
  }
}  
#endif
