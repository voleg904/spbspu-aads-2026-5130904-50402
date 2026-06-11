#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "HashTableImpl.hpp"
#include "Graph.hpp"
#include "ListImpl.hpp"

namespace vishnevskiy
{
  using graph_t = vishnevskiy::HashTable<std::string, vishnevskiy::graph, size_t(*)(const std::string&), bool(*)(const vishnevskiy::graph&, const vishnevskiy::graph&)>;

  void load(const std::string& filename, graph_t& graphtable)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::string graphName;
    size_t edgeCount = 0;

    while (std::getline(file, line))
    {
      std::istringstream iss(line);

      if (edgeCount == 0)
      {
        iss >> graphName >> edgeCount;
        graphtable.add(graphName, vishnevskiy::graph(graphName));
      }
      else
      {
        std::string from, to;
        size_t weight;
        iss >> from >> to >> weight;
        vishnevskiy::graph& g = graphtable.at(graphName);
        vishnevskiy::vertex v{to, from};
        if (g.vertexes.has(v))
        {
          vishnevskiy::List<int>& weights = g.vertexes.at(v);
          vishnevskiy::LIter<int> it(&weights);
          it.end();
          it.insert(weight);
        }
        else
        {
          vishnevskiy::List<int> weights;
          weights.val = weight;
          weights.next = nullptr;
          g.vertexes.add(v, weights);
        }
        
        edgeCount--;
      }
    }
  }

  void graphs(std::ostream& o, std::istream&, graph_t& graphtable)
  {
    vishnevskiy::tableIt<std::string, vishnevskiy::graph, size_t(*)(const std::string&), bool(*)(const vishnevskiy::graph&, const vishnevskiy::graph&)> it(&graphtable);
    while (it.hasNext())
    {
      o << it.key() << "\n";
      it.next();
    }
  }

  void vertexes(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name;
    i >> name;
    if (graphtable.has(name))
    {
      vishnevskiy::graph g = graphtable.at(name);
      vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it(&g.vertexes);
      while (it.hasNext())
      {
        o << it.key().in << "\n";
        o << it.key().in << "\n";
        it.next();
      }
    }
    else
    {
      throw std::logic_error("Cannot find name!");
    }
  }

  void outbound(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    
  }

  void inbound(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  void bind(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  void cut(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  void create(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  void merge(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  void extract(std::ostream& o, std::istream& i, graph_t& graphtable)
  {

  }

  int main(int argc, char* argv[])
  {
    if (argc != 2)
    {
      return 1;
    }

    std::string filename = argv[1];

    using cm_t = void(*)(std::ostream&, std::istream&, graph_t&);
    using table_t = vishnevskiy::HashTable<std::string, cm_t, size_t(*)(const std::string&), bool(*)(const std::string&, const std::string&)>;
    graph_t graphtable(32, vishnevskiy::graphHasher, vishnevskiy::graphEq);

    try
    {
      load(filename, graphtable);
    }
    catch (const std::runtime_error&)
    {
      std::cerr << "Bad file!\n";
      return 1;
    }

    table_t cmds(9, vishnevskiy::graphHasher, vishnevskiy::graphEq);

    cmds.add("graphs", graphs);
    cmds.add("vertexes", vertexes);
    cmds.add("outbound", outbound);
    cmds.add("inbound", inbound);
    cmds.add("bind", bind);
    cmds.add("cut", cut);
    cmds.add("create", create);
    cmds.add("merge", merge);
    cmds.add("extract", extract);

    std::string cmd;
    while (std::cin >> cmd)
    {
      try
      {
        cmds.at(cmd)(std::cout, std::cin, graphtable);
      }
      catch (const std::out_of_range&)
      {
        std::cout << "<INVALID COMMAND>\n";
        auto toignore = std::numeric_limits<std::streamsize>::max();
        std::cin.ignore(toignore, '\n');
      }
      catch (const std::logic_error& e) {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    if (!std::cin.eof())
    {
      std::cerr << "Bad input!\n";
      return 1;
    }
    return 0;
  }
}
