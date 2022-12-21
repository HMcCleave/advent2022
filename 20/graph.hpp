#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <optional>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace details {
  template <typename CostType,
            CostType kInvalid,
            bool multiple_edges>
  struct DenseGraphBase;

  template <typename CostType, CostType kInvalid>
  struct DenseGraphBase<CostType, kInvalid, false> {
    std::vector<std::vector<CostType>> edges_;
  };

  template <typename CostType, CostType kInvalid>
  struct DenseGraphBase<CostType, kInvalid, true> {
    std::vector<std::vector<std::multiset<CostType>>> edges_;
  };

  template<typename CostType, CostType kInvalid, bool multiple_edges>
  struct DenseGraph
      : public DenseGraphBase<CostType, kInvalid, multiple_edges> {

    void reserve(size_t capacity) { 
      this->edges_.reserve(capacity);
      for (auto& v : this->edges_) {
        v.reserve(capacity);
      }
    }

    void resize(size_t newsize) {
      this->edges_.resize(newsize);
      for (auto& e : this->edges_) {
        if constexpr (multiple_edges) {
          e.resize(newsize);
        } else {
          e.resize(newsize, kInvalid);
        }
      }
    }

    std::optional<CostType> getShortestEdge(int start, int end) const {
      if constexpr (multiple_edges) {
        return this->edges_[start][end].size() ? this->edges_[start][end].begin()
                                         : std::nullopt;
      } else {
        return this->edges_[start][end] != kInvalid ? this->edges_[start][end]
                                              : std::nullopt;
      }
    }

    std::multiset<CostType> getEdges(int start, int end) const {
      if constexpr (multiple_edges) {
        return this->edges_[start][end];
      } else {
        std::multiset<CostType> out;
        if (this->edges_[start][end] != kInvalid)
          out.insert(this->edges_[start][end]);
        return out;
      }
    }

    bool removeEdge(int start, int end, CostType t) {
      if constexpr (multiple_edges) {
        if (this->edges_[start][end].count(t) > 0) {
          this->edges_[start][end].remove(t);
          return true;
        }
      } else {
        if (this->edges_[start][end] == t) {
          this->edges_[start][end] = kInvalid;
        }
      }
      return false;
    }

    void clearEdges(int start, int end) {
      if constexpr (multiple_edges) {
        this->edges_[start][end] = {};
      } else {
        this->edges_[start][end] = kInvalid;
      }
    }

    void addEdge(int start, int end, CostType cost) {
      if constexpr (multiple_edges) {
        this->edges_[start][end].insert(cost);
      } else {
        this->edges_[start][end] = cost;
      }
    }

    void forEachEdge(int start, std::function<void(int, CostType)> visitor) const {
      if constexpr (!multiple_edges) {
        for (int dest = 0; dest < this->edges_.size(); ++dest) {
          for (const auto& cost : this->edges_[start][dest])
            visitor(dest, cost);
        }
      } else {
        return forEachShortestEdge(start, visitor);
      }
    }

    void forEachShortestEdge(int start, std::function<void(int, CostType)> visitor) const {
      for (int dest = 0; dest < (int)this->edges_.size(); ++dest) {
        if constexpr (multiple_edges) {
          if (this->edges_[start][dest].size() != 0)
            visitor(dest, this->edges_[start][dest].begin());
        } else {
          if (const auto& value = this->edges_[start][dest]; value != kInvalid)
            visitor(dest, value);
        }
      }
    }
  };

};


template <typename NodeType,
          typename CostType,
          bool directed = false,
          bool multiple_edges = false,
          CostType kInvalidCost = (CostType)-1>
class Graph {
  std::unordered_map<NodeType, int> node_lookup_;
  std::vector<NodeType> nodes_;
  details::DenseGraph<CostType, kInvalidCost, multiple_edges> graph_;

  struct AllShortestPathData {
    std::vector<std::vector<int>> next_;
    std::vector<std::vector<CostType>> cost_;
  };

  std::optional<AllShortestPathData> path_data_;

 public:
  Graph() {}
  Graph(int cap) { reserve(cap); }
  Graph(std::vector<NodeType> nodes) : nodes_(std::move(nodes)) {
    reserve(nodes_.size());
    for (size_t i = 0; i < nodes_.size(); ++i)
      node_lookup_[nodes_[i]] = i;
  }


  void addNode(NodeType v) {
    if (node_lookup_.find(v) == node_lookup_.end()) {
      node_lookup_[v] = nodes_.size();
      nodes_.push_back(v);
      resize(nodes_.size());
    }
  }

  void reserve(int size) {
    graph_.reserve(size);
  }

  void resize(int size) {
    graph_.resize(size);
    invalidate();
  }

  void invalidate() {
    path_data_ = std::nullopt;
  }

  void addEdge(NodeType start, NodeType end, CostType cost) {
    addNode(start);
    addNode(end);
    auto istart = node_lookup_[start];
    auto iend = node_lookup_[end];
    graph_.addEdge(istart, iend, cost);
    if constexpr (!directed) {
      graph_.addEdge(iend, istart, cost);
    }
    invalidate();
  }

  void computeAllShortestPaths() {
    AllShortestPathData data;

    const int node_count{(int)nodes_.size()};

    data.next_.resize(node_count);
    for (auto& v : data.next_)
      v.resize(node_count, -1);

    data.cost_.resize(node_count);
    for (auto& c : data.cost_)
      c.resize(node_count, kInvalidCost);

    for (int i = 0; i < node_count; ++i) {
      data.next_[i][i] = i;
      data.cost_[i][i] = (CostType)0;
      graph_.forEachShortestEdge(i, [&](int dest, CostType cost) {
        data.next_[i][dest] = dest;
        data.cost_[i][dest] = cost;
      });
    }

    for (int k = 0; k < node_count; ++k) {
      for (int s = 0; s < node_count; ++s) {
        if (data.next_[s][k] != -1) {
          for (int e = 0; e < node_count; ++e) {
            if (data.next_[k][e] != -1 &&
                (data.next_[s][e] == -1 || data.cost_[s][e] > data.cost_[s][k] + data.cost_[k][e])) {
              data.next_[s][e] = data.next_[s][k];
              data.cost_[s][e] = data.cost_[s][k] + data.cost_[k][e];
            }
          }
        }
      }
    }

    path_data_ = std::move(data);
  }

  std::tuple<std::vector<NodeType>, CostType> fastestPath(NodeType start,
                                                          NodeType end) {
    auto istart = node_lookup_[start];
    auto iend = node_lookup_[end];
    if (path_data_.has_value()) {
      auto& data = path_data_.value();
      std::tuple<std::vector<NodeType>, CostType> out;
      auto& [path, cost] = out;
      cost = data.cost_[istart][iend];
      path.push_back(nodes_[istart]);
      while (istart != iend) {
        istart = data.next_[istart][iend];
        path.push_back(nodes_[istart]);
      }
      return out;
    }
    return computeFastestPath(istart, iend);
  }

private:
  std::tuple<std::vector<NodeType>, CostType> computeFastestPath(int istart,
                                                                int iend) {
    struct Edge {
      int start_, end_;
      CostType cost_;
      bool operator<(const Edge& that) const { return cost_ < that.cost_; }
    };
    std::vector<bool> found(nodes_.size(), false);
    std::vector<Edge> pred(nodes_.size());
    std::priority_queue<Edge> edges;
    graph_.forEachShortestEdge(istart, [&](int dest, CostType cost) {
      edges.push({istart, dest, cost});
    });
    pred[istart] = {istart, istart, 0};

    while (!found[iend] && edges.size()) {
      auto edge = edges.top();
      edges.pop();
      if (!found[edge.end_]) {
        found[edge.end_] = true;
        pred[edge.end_] = edge;
        graph_.forEachShortestEdge(edge.end_, [&](int dest, CostType cost) {
          edges.push({edge.end_, dest, cost});
        });
      }
    }

    if (found[iend]) {
      std::tuple<std::vector<NodeType>, CostType> out;
      auto& [path, cost] = out;

      path.push_back(nodes_[iend]);
      while (iend != istart) {
        auto edge = pred[iend];
        cost += edge.cost_;
        iend = edge.start_;
        path.push_back(nodes_[iend]);
      }
      std::reverse(path.begin(), path.end());

      return out;
    }

    return {{}, kInvalidCost};
  }
};

#endif // GRAPH_HPP