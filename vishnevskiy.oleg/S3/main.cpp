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
    std::string name, vertex;
    i >> name >> vertex;

    if (graphtable.has(name))
    {
      vishnevskiy::graph g = graphtable.at(name);
      vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it(&g.vertexes);
      while (it.hasNext())
      {
        if (it.key().in == vertex)
        {
          o << it.key().out;
          vishnevskiy::List<int>& weights = it.val();
          vishnevskiy::LIter<int> Wit(&weights);
          while (Wit.hasNext())
          {
            o << " " << Wit.value();
            ++Wit;
          }
          o << " " << Wit.value() << "\n";
        }
        it.next();
      }
    }
    else
    {
      throw std::logic_error("Cannot find name!");
    }
  }

  void inbound(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name, vertex;
    i >> name >> vertex;

    if (graphtable.has(name))
    {
      vishnevskiy::graph g = graphtable.at(name);
      vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it(&g.vertexes);
      while (it.hasNext())
      {
        if (it.key().out == vertex)
        {
          o << it.key().in;
          vishnevskiy::List<int>& weights = it.val();
          vishnevskiy::LIter<int> Wit(&weights);
          while (Wit.hasNext())
          {
            o << " " << Wit.value();
            ++Wit;
          }
          o << " " << Wit.value() << "\n";
        }
        it.next();
      }
    }
    else
    {
      throw std::logic_error("Cannot find name!");
    }
  }

  void bind(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name, from, to;
    int weight;
    i >> name >> from >> to >> weight;

    if (graphtable.has(name))
    {
      vishnevskiy::graph& g = graphtable.at(name);
      vishnevskiy::vertex v{to, from};
  
      if (g.vertexes.has(v))
      {
        vishnevskiy::LIter<int> it(&g.vertexes.at(v));
        it.end();
        it.insert(weight);
      }
      else
      {
        vishnevskiy::List<int> w;
        w.val = weight;
        w.next = nullptr;
        g.vertexes.add(v, w);
      }
    }
    else
    {
      throw std::logic_error("Cannot find name!");
    }
  }

  void cut(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name, from, to;
    int weight;
    i >> name >> from >> to >> weight;

    if (!graphtable.has(name))
    {
      throw std::logic_error("Cannot find name!");
    }

    vishnevskiy::graph& g = graphtable.at(name);
    vishnevskiy::vertex v{to, from};

    if (!g.vertexes.has(v))
    {
      throw std::logic_error("Cannot find vertex!");
    }

    vishnevskiy::List<int>& wlist = g.vertexes.at(v);
    vishnevskiy::LIter<int> prev(&wlist);
    vishnevskiy::LIter<int> curr(&wlist);
    bool f = false;
    while (curr.hasNext() && !f)
    {
      if (curr.value() == weight)
      {
        f = true;
      }
      prev = curr;
      ++curr;
    }

    if (!f)
    {
      throw std::logic_error("Cannot find weight!");
    }

    if (curr.curr == &wlist)
    {
      vishnevskiy::List<int>* next = wlist.next;
      if (next)
      {
        wlist.val = next->val;
        wlist.next = next->next;
        delete next;
      }
      else
      {
        g.vertexes.drop(v);
      }
    }
    else
    {
      prev.curr->next = curr.curr->next;
      delete curr.curr;
    }
  }

  void create(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name;
    i >> name;

    if (graphtable.has(name))
    {
      throw std::logic_error("Graph exists!");
    }
    graphtable.add(name, vishnevskiy::graph(name));
  }

  void merge(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string name1, name2, newname;
    i >> name1 >> name2 >> newname;

    if (!graphtable.has(name1) || !graphtable.has(name2))
    {
      throw std::logic_error("Cannot find name!");
    }

    if (graphtable.has(newname))
    {
      throw std::logic_error("Graph exists!");
    }

    vishnevskiy::graph& g1 = graphtable.at(name1);
    vishnevskiy::graph& g2 = graphtable.at(name2);
    vishnevskiy::graph newGraph;
    newGraph.name = newname;

    vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it1(&g1.vertexes);
    while (it1.hasNext())
    {
      newGraph.vertexes.add(it1.key(), it1.val());
      it1.next();
    }

    vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it2(&g2.vertexes);
    while (it2.hasNext())
    {
      if (newGraph.vertexes.has(it2.key()))
      {
        vishnevskiy::LIter<int> it(&newGraph.vertexes.at(it2.key()));
        it.end();
        vishnevskiy::LIter<int> wit(&it2.val());
        while (wit.hasNext())
        {
          it.insert(wit.value());
          ++wit;
        }
      }
      else
      {
        newGraph.vertexes.add(it2.key(), it2.val());
      }
      it2.next();
    }

    graphtable.add(newname, newGraph);
  }

  void extract(std::ostream& o, std::istream& i, graph_t& graphtable)
  {
    std::string newname, oldname;
    size_t vertexCount;
    i >> newname >> oldname >> vertexCount;

    if (graphtable.has(newname) || !graphtable.has(oldname))
    {
      throw std::logic_error("Incorrect name!");
    }

    vishnevskiy::graph& oldGraph = graphtable.at(oldname);
    vishnevskiy::graph newGraph;
    newGraph.name = newname;

    std::string* vertexNames = new std::string[vertexCount];
    for (size_t j = 0; j < vertexCount; ++j)
    {
      i >> vertexNames[j];
    }

    vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>, size_t(*)(const vishnevskiy::vertex&), bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&)> it(&oldGraph.vertexes);
    while (it.hasNext())
    {
      std::string from = it.key().in;
      std::string to = it.key().out;

      bool fromFlag = false;
      bool toFlag = false;
      for (size_t j = 0; j < vertexCount; ++j)
      {
        if (vertexNames[j] == from)
        {
          fromFlag = true;
        }
        if (vertexNames[j] == to)
        {
          toFlag = true;
        }
      }

      if (fromFlag && toFlag)
      {
        newGraph.vertexes.add(it.key(), it.val());
      }

      it.next();
    }
    graphtable.add(newname, newGraph);
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
