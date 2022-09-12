/*******************************************************************************
 * IBM Confidential
 *
 *   OCO Source Materials
 *   5737-A56
 *   © Copyright IBM Corp. 2017
 *
 *   The source code for this program is not published or other-wise divested
 *   of its trade secrets, irrespective of what has been deposited with the
 *   U.S. Copyright Office.
 ******************************************************************************/

#ifndef SRC_HELAYERS_GRAPH_H
#define SRC_HELAYERS_GRAPH_H

#include "SaveableBasic.h"
#include "boost/graph/adjacency_list.hpp"

namespace helayers {

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
    AdjacencyList;

///@brief A class to represent a directed graph with basic operations on it,
/// where nodes are represented as subsequent integers.
///
class Graph : public SaveableBasic
{
  AdjacencyList graph;

public:
  /// Constructor
  Graph() {}

  /// The default destructor
  ~Graph() = default;

  ///@brief Default copy constructor
  ///
  ///@param src Source object
  Graph(const Graph& src) = default;

  ///@brief Default operator=
  ///
  ///@param src Source object
  Graph& operator=(const Graph& src) = default;

  std::streamoff save(std::ostream& stream) const override;

  std::streamoff load(std::istream& stream) override;

  ///@brief Adds new node to the graph. The index of it will be the next
  /// available integer.
  ///
  void addNode();

  ///@brief Adds new directed edge to the graph.
  ///
  ///@param src The source node.
  ///@param dst The destination node.
  void addEdge(int src, int dst);

  ///@brief Removes an existing node from the graph along with all the edges
  /// connected to it. this operation changes the indexes of nodes in the graph
  /// to keep them subsequent.
  ///
  ///@param node The node to remove.
  void removeNode(int node);

  ///@brief Removes an existing edge from the graph.
  ///
  ///@param src The source node.
  ///@param dst The destination node.
  void removeEdge(int src, int dst);

  ///@brief Returns the number of nodes in the graph.
  ///
  int getNumNodes() const;

  ///@brief Returns the number of edges in the graph.
  ///
  int getNumEdges() const;

  ///@brief Repositions an existing source node in the graph such that it is
  /// connected to a given target node.
  /// All the edges currently entering the source node will now be connected to
  /// all the so-far-neighbours of the source node instead, and all the edges
  /// currently entering the target node will now be connected to the source
  /// node instead. An edge will be added between the source and target nodes.
  ///
  ///@param sourceNode The source node to reposition.
  ///@param targetNode The target node to reposition before.
  void repositionBefore(int sourceNode, int targetNode);

  ///@brief Repositions an existing source node in the graph such that a given
  /// target node is connected to it.
  /// All the edges currently leaving the source node will now be connected to
  /// all the so-far-neighbours of the source node instead, and all the edges
  /// currently leaving the target node will now be connected to the source
  /// node instead. An edge will be added between the target and source nodes.
  ///
  ///@param sourceNode The source node to reposition.
  ///@param targetNode The target node to reposition after.
  void repositionAfter(int sourceNode, int targetNode);

  ///@brief Places an existing source node in the graph, that has no neighbours,
  /// such that it is connected to a given target node.
  /// All the edges currently entering the target node will now be connected to
  /// the source node instead. An edge will be added between the source and
  /// target nodes.
  ///
  ///@param sourceNode The source node to palce.
  ///@param targetNode The target node to palce before.
  void placeBefore(int sourceNode, int targetNode);

  ///@brief Places an existing source node in the graph, that has no neighbours,
  /// such that a given target node is connected to it.
  /// All the edges  currently leaving the target node will now be connected to
  /// the source node instead. An edge will be added between the target and
  /// source nodes.
  ///
  ///@param sourceNode The source node to palce.
  ///@param targetNode The target node to palce after.
  void placeAfter(int sourceNode, int targetNode);

  ///@brief Removes all the edges entering and leaving an existing node in the
  /// graph, and adds new edges between the current neighbours of the node such
  /// that the node is "bypassed".
  ///
  ///@param node The given node to bypass.
  void bypassNode(int node);

  ///@brief Returns the inbound nodes of a given node in the graph.
  ///
  ///@param node The given node.
  std::set<int> getInboundNodes(int node) const;

  ///@brief Returns the outbound nodes of a given node in the graph.
  ///
  ///@param node The given node.
  std::set<int> getOutboundNodes(int node) const;

  ///@brief Returns, with respect to a given first node, the index of a given
  /// second node that is an inbound node of the first node, in the set of all
  /// inbound nodes of the first node.
  ///
  ///@param node The first node.
  ///@param inboundNode The second node, must be an inbound node of the first
  /// node.
  int getInboundNodeIndex(int node, int inboundNode) const;

  ///@brief Returns a topological order of the nodes in the graph.
  ///
  std::vector<int> getTopologicalOrder() const;

  ///@brief Returns indication to whether all the nodes in the graph are
  /// reachable from a given node, by a directed path of any length.
  ///
  ///@param node The given node to check.
  bool doAllNodesReachableFrom(int node) const;

  ///@brief Returns indication to whether all the nodes in the graph reach a
  /// given node, by a directed path of any length.
  ///
  ///@param node The given node to check.
  bool doAllNodesReach(int node) const;

  ///@brief Returns all the nodes in the graph reachable from a given node.
  ///
  ///@param node The given node to check.
  ///@param reverse Whether to iterate the graph in reverse, such that the
  /// direction of every directed edge if flipped.
  std::set<int> getReachableNodes(int node, bool reverse = false) const;
};
} // namespace helayers

#endif /* SRC_HELAYERS_GRAPH_H */