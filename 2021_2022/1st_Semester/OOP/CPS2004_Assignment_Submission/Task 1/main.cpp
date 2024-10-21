#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>

template <typename T>
class edge final {
 public:
  edge(const T &s, const T &t) : source(std::move(s)), target(std::move(t)) {}
  //Returns the source and target of the edges
  const T &src() const { return source; }

  const T &tgt() const { return target; }

 private:
  const T source; //the edge's source
  const T target; //the edge's target
};

template <typename T>
class vertex {
 public:
  vertex(const T &obj) : obj(std::move(obj)) {}

  bool operator<(const vertex<T> &o) const { return obj < o.obj; }

  void add_vertex(const T &t) { vertices.insert(t); } //function used to add a node to the graph

  const T &value() const { return obj; }

  void print() const { //function used to print out a node
    if (vertices.empty()) {
      std::cout << value() << ": " << std::endl;
      return;
    }
    std::cout << value() << ": ";
    std::cout << '[';
    for (const vertex &v : vertices) {
      std::cout << v.value() << ',';
    }
    std::cout << ']';
    std::cout << std::endl;
  }

  void get_edges(std::list<edge<T>> &edge_list) const { //function used to obtain the edge lisgt
    for (const auto &v : vertices) {
      edge_list.push_back(edge<T>(obj, v));
    }
  }

  bool contains(const T &v) const { return vertices.find(v) != vertices.end(); }

  auto vertex_count() const { return vertices.size(); }

 private:
  T obj;

  std::set<T> vertices;
};

template <typename T>
class dag {
 public:
  dag(const std::list<edge<T>> &edge_list) {
    for (const auto &edge : edge_list) {
      // use std::move to take ownership
      T src = std::move(edge.src());
      T tgt = std::move(edge.tgt());

      // add the left-hand side node of this edge
      auto v = vertex(src);
      // try to find an existing node in the graph for this edge's source
      auto i = vertices.find(v);
      if (i == vertices.end())  // not found (not in graph)
      {
        // add the source node to the graph's set of nodes
        i = vertices.insert(v).first;
      }
      // add the right-hand side node of this edge as a linked node of the
      // left-hand side node of this edge
      ((vertex<T> &)*i).add_vertex(tgt);

      // add the right-hand side node of this edge
      // try to find an existing node in the graph for this edge's target
      i = vertices.find(tgt);
      if (i == vertices.end()) {
        // add the target node to the graph's set of nodes
        vertices.insert(tgt);
      }
    }

    // remove any disconnected vertices
    // remove_disconnected_vertices();
  }

  // void remove_disconnected_vertices()
  // {
  // 	std::list<typename std::set<vertex<T>>::iterator> remove_list;
  // 	for (auto i = vertices.begin(); i != vertices.end(); ++i)
  // 	{
  // 		const auto &v1(*i);
  // 		std::cout << "::: " << v1.value() << std::endl;
  // 		if (v1.vertex_count() > 0)
  // 		{
  // 			continue;
  // 		}
  // 		bool remove = true;
  // 		for (auto j = vertices.begin(); j != vertices.end(); ++j)
  // 		{
  // 			if (j == i)
  // 			{
  // 				continue;
  // 			}
  // 			const auto &v2(*j);
  // 			if (v2.contains(v1.value()))
  // 			{
  // 				remove = false;
  // 				break;
  // 			}
  // 		}
  // 		if (remove)
  // 		{
  // 			std::cout << "@@@@@@@@@@@@ " << v1.value() << " : " <<
  // v1.vertex_count() << std::endl; 			remove_list.push_back(i);
  // 		}
  // 	}
  // 	for (const auto &i : remove_list)
  // 	{
  // 		vertices.erase(i);
  // 	}
  // }

  void print() const {
    for (const auto &v : vertices) {
      v.print();
    }
  }

  std::list<edge<T>> get_edges() const {
    // resultant list of edges
    std::list<edge<T>> res;

    // for each node in the DAG, call it's get_edges function,
    // passing in the result list defined above so that
    // each node adds it's edges to the same list
    for (const auto &v : vertices) {
      v.get_edges(res);
    }

    return res;
  }

  void remove_vertex(const T &t) {
    vertices.erase(t);
    // //remove any disconnected vertices
    // remove_disconnected_vertices();
  }

 private:
  std::set<vertex<T>> vertices;
};

int main(int argc, char *argv[]) {
  //The edge_list used
//   std::list<edge<int>> edge_list;
//   edge_list.push_back(edge<int>(1, 2));
//   edge_list.push_back(edge<int>(2, 4));
//   edge_list.push_back(edge<int>(2, 5));
//   edge_list.push_back(edge<int>(1, 3));
//   edge_list.push_back(edge<int>(4, 7));
//   edge_list.push_back(edge<int>(7, 9));
//   edge_list.push_back(edge<int>(7, 16));
//   edge_list.push_back(edge<int>(16, 32));
   std::list<edge<int>> edge_list;
   edge_list.push_back(edge<int>(3, 8));
   edge_list.push_back(edge<int>(3, 2));
   edge_list.push_back(edge<int>(2, 7));
   edge_list.push_back(edge<int>(7, 22));
   edge_list.push_back(edge<int>(22, 10));
   edge_list.push_back(edge<int>(22, 5));

  dag<int> graph(edge_list);
  std::cout << "===== DAG ================" << std::endl; //print out the full DAG
  graph.print();
  std::cout << "===== Edge List ================" << std::endl; //Print the edge list
  const auto &edges = graph.get_edges();
  for (const auto &e : edges) { //loop through all the edges
    std::cout << e.src() << ": " << e.tgt() << std::endl;
  }
  // std::cout << "===== DAG ================" << std::endl;
  // graph.print();

  std::cout << "======Remove Node 1,2 ====" << std::endl;
  graph.remove_vertex(1); //will remove the node '1'
  graph.remove_vertex(2); //will remove the node '2'
  //graph.remove_vertex(5); //will remove the node '5' - was used during testing
  graph.print();
  std::cout << "===== Edge List ================" << std::endl; //print the edge list again
  for (const auto &e : graph.get_edges()) {
    std::cout << e.src() << ": " << e.tgt() << std::endl;
  }

  return 0;
}