#ifndef LISTTOOLS_HPP
#define LISTTOOLS_HPP
#include <cstddef>
#include <string>

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
  class NamedList
  {
    private:
      std::string name;
      NamedList<T>* next;
      List<T>* data;
  };

  template <class T>
  class NamedLIter
  {
    friend class NamedList<T>;
    private:
      NamedList<T>* curr;
    public:
      NamedLIter();
      NamedLIter(NamedList<T>* h);
      List<T>* value();
      void operator++();
      std::string getName();
      void setData(NamedList<T>* h);
      bool hasNext();
      void end();
      void insert(List<T>* d, std::string name);
      void clear(NamedLIter* head);
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
      T* value();
      void operator++();
      void set(List<T>* h);
      bool hasNext();
      void end();
      void insert(T& d);
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
      void operator++();
      T* value();
      bool hasNext();
  };
}

#endif
