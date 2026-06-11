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
  };

  struct graph
  {
    std::string name;
    vishnevskiy::HashTable<vertex, vishnevskiy::List<int>, size_t(*)(const vertex&), bool(*)(const vertex&, const vertex&)> vertexes;
    graph():
      vertexes(10, vertexHasher, vertexEq)
    {}
    graph(const std::string& n):
      name(n),
      vertexes(10, vertexHasher, vertexEq)
    {}
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

  size_t graphHasher(const std::string& name)
  {
    boost::hash2::siphash_64 hash;
    hash.update(name.data(), name.size());
    return hash.result();
  }

  bool graphEq(const graph& graph1, const graph& graph2)
  {
    if ((graph1.name != graph2.name) || (graph1.vertexes.getSize() != graph2.vertexes.getSize()))
    {
      return false;
    }
    return true;
  }
}

#endif
