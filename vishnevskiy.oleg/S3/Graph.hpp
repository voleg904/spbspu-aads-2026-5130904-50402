#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTableImpl.hpp"
#include "ListImpl.hpp"
#include <string>
#include <iostream>
#include <cstddef>
#include <boost/hash2/siphash.hpp>

namespace vishnevskiy
{
  struct vertex
  {
    std::string out;
    std::string in;
    vertex() = default;
    vertex(const std::string& o, const std::string& i):
      out(o),
      in(i)
    {}
    vertex(const vertex& other) = default;
    vertex& operator=(const vertex& other) = default;
  };

  struct graph
  {
    std::string name;
    size_t vertexCount;
    size_t pointCount;
    vishnevskiy::List<std::string>* points;
    vishnevskiy::HashTable<vertex, vishnevskiy::List<int>*, size_t(*)(const vertex&), bool(*)(const vertex&, const vertex&)> vertexes;
    graph();
    graph(const std::string& n);
    graph(const std::string& n, const size_t& c);
    graph(const graph& other);
    graph& operator=(const graph& other);
    ~graph();
  };

  size_t vertexHasher(const vertex& v)
  {
    boost::hash2::siphash_64 hash;
    hash.update(v.out.data(), v.out.size());
    hash.update(v.in.data(), v.in.size());
    return hash.result();
  }

  bool vertexEq(const vertex& first, const vertex& second)
  {
    return first.out == second.out && first.in == second.in;
  }

  graph::graph():
    vertexCount(0),
    pointCount(0),
    points(nullptr),
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::graph(const std::string& n):
    name(n),
    vertexCount(0),
    pointCount(0),
    points(nullptr),
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::graph(const std::string& n, const size_t& c):
    name(n),
    vertexCount(c),
    pointCount(0),
    points(nullptr),
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::graph(const graph& other):
    name(other.name),
    vertexCount(other.vertexCount),
    pointCount(other.pointCount),
    points(nullptr),
    vertexes(other.vertexes)
  {
    if (other.points)
    {
      points = new vishnevskiy::List<std::string>();
      vishnevskiy::LIter<std::string> itE(points);
      vishnevskiy::LIter<std::string> itH(other.points);
      while (itH.hasNext())
      {
        itE.insert(itH.value());
        ++itH;
      }
      itE.insert(itH.value());
    }
  }

  graph& graph::operator=(const graph& other)
  {
    if (this != &other)
    {
      name = other.name;
      vertexCount = other.vertexCount;
      pointCount = other.pointCount;

      if (points)
      {
        vishnevskiy::LIter<std::string> it(points);
        it.clear(&it);
        delete points;
        points = nullptr;
      }

      if (other.points)
      {
        points = new vishnevskiy::List<std::string>();
        points->val = other.points->val;
        points->next = nullptr;

        vishnevskiy::List<std::string>* h = other.points->next;
        vishnevskiy::List<std::string>* e = points;

        while (h)
        {
          e->next = new vishnevskiy::List<std::string>();
          e = e->next;
          e->val = h->val;
          e->next = nullptr;
          h = h->next;
        }
      }

      vertexes = other.vertexes;
    }
    return *this;
  }

  graph::~graph()
  {
    if (points)
    {
      vishnevskiy::List<std::string>* current = points;
      while (current)
      {
        vishnevskiy::List<std::string>* next = current->next;
        delete current;
        current = next;
      }
      points = nullptr;
    }
    using vHt = size_t(*)(const vishnevskiy::vertex&);
    using vEqt = bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&);
    vishnevskiy::tableIt<vertex, vishnevskiy::List<int>*, vHt, vEqt> it(&vertexes);
    while (it.hasNext())
    {
      delete it.val();
      it.next();
    }
  }

  size_t graphHasher(const std::string& name)
  {
    boost::hash2::siphash_64 hash;
    hash.update(name.data(), name.size());
    return hash.result();
  }
}

#endif
