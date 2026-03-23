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
      T* operator->();
      void operator++();
      std::string getName();
      void setData(NamedList<T>* h);
      bool hasNext();
      void end();
      void insert(NamedLIter<T>* node, T& d);
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
      T* operator->();
      void operator++();
      void set(List<T>* h);
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
      void operator++();
      void operator->();
      bool hasNext();
  };
}

#endif
