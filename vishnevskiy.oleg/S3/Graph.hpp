#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "HashTableImpl.hpp"
#include "ListImpl.hpp"
#include <string>
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
    vishnevskiy::HashTable<vertex, vishnevskiy::List<int>*, size_t(*)(const vertex&), bool(*)(const vertex&, const vertex&)> vertexes;
    graph();
    graph(const std::string& n);
    graph(const std::string& n, const size_t& c);
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
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::graph(const std::string& n):
    name(n),
    vertexCount(0),
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::graph(const std::string& n, const size_t& c):
    name(n),
    vertexCount(c),
    vertexes(10, vertexHasher, vertexEq)
  {}

  graph::~graph()
  {
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
