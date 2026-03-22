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
    private:
      List<T>* curr;
    public:
      LIter();
      LIter(List<T>* h);
      T* operator->();
      void operator++();
      bool hasNext();
      void end();
      void insert(LIter<T>* node, T& d);
      void clear(LIter* head);
  };

  
  template <class T>
  class LCIter
  {
    friend class List<T>;
    private:
      const List<T>* curr;
    public:
      LCIter();
      LCIter(const List<T>* h);
      LCIter& operator++();
      T* operator->();
      bool hasNext();
  };
}

#endif
