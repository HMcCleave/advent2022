#include <any>
#include <bitset>
#include <chrono>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <variant>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <execution>
#include <cfenv>
#include <cmath>
#include <complex>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <streambuf>
#include <regex>
#include <stack>
#include <chrono>
#include <fstream>

using namespace std;

namespace {  // helpers

string joinv(const vector<string>& parts, const string& inter) {
  if (parts.empty())
    return "";
  string out = parts.front();
  for (auto iter = parts.begin() + 1; iter != parts.end(); ++iter)
    out = out + inter + *iter;
  return out;
}

string to_string(const string& s) {
  return s;
}

template <typename... Args>
string join(const string& inter, Args... args) {
  vector<string> asvec{to_string(args)...};
  return joinv(asvec, inter);
}

vector<string> split(const string& large, const string& marker) {
  vector<string> out;
  size_t iter = 0;
  size_t pos = 0;
  while ((iter = large.find(marker, pos)) != string::npos) {
    out.emplace_back(large.begin() + pos, large.begin() + iter);
    pos = iter + marker.length();
  }
  out.push_back(large.substr(pos));
  return out;
}

template <typename Node, typename Cost>
struct Edge {
  Node start;
  Node dest;
  Cost cost;

  bool operator<(const Edge& that) const { return cost < that.cost; }
};

template <typename Node, typename Cost>
struct Graph {
  using EdgeType = Edge<Node, Cost>;
  virtual ~Graph() {}

  virtual set<Node> nodes() const = 0;
  virtual int forEachEdge(Node start, function<bool(EdgeType)>) const = 0;
  virtual optional<EdgeType> getEdge(Node start, Node dest) const = 0;
};

template <typename Node, typename Cost>
vector<Edge<Node, Cost>> shortestPath(const Graph<Node, Cost>& g,
                                      Node start,
                                      Node end) {
  set<Node> found;
  priority_queue<Edge> edges;
  map<Node, Node> pred;

  found.insert(start);
  g.forEachEdge(start, [&edges](Edge<Node, Cost> e) {
    edges.push(e);
    return false;
  });

  while (!found.count(end) && edges.size()) {
    auto& e = edges.top();
    if (found.count(e.dest) == 0) {
      found.insert(e.dest);
      pred[e.dest] = e.source;
    }
    edges.pop();
  }
  if (found.count(end)) {
    vector<Edge<Node, Cost>> out;
    for (Node n = end; n != start; n = pred[n])
      out.push_back(g.getEdge(pred[n], n));
    reverse(out.begin(), out.end());
    return out;
  }
  return {};
}

template <typename Node, typename Cost>
ostream& operator<<(ostream& out, const Edge<Node, Cost>& e) {
  return out << e.start << " -> " << e.dest << " = " << e.cost << endl;
}

template <typename Node, typename Cost>
ostream& operator<<(ostream& out, const Graph<Node, Cost>& g) {
  for (auto& node : g.nodes()) {
    g.forEachEdge(node, [&](auto e) {
      out << e;
      return false;
    });
  }
  return out;
}

template <typename Node, typename Cost>
struct DirectedGraph : public Graph<Node, Cost> {
  set<Node> nodes_;
  multimap<Node, Edge<Node, Cost>> edges_;

  virtual ~DirectedGraph() {}

  virtual set<Node> nodes() const override { return nodes_; }

  virtual int forEachEdge(Node start,
                          function<bool(Edge<Node, Cost>)> func) const {
    return count_if(edges_.lower_bound(start), edges_.upper_bound(start),
                    [&](auto p) { return func(p.second); });
  }

  virtual optional<Edge<Node, Cost>> getEdge(Node start, Node dest) const {
    optional<Edge<Node, Cost>> out{};
    forEachEdge(start, [&](Edge<Node, Cost> e) {
      if (e.dest == dest) {
        out = e;
        return true;
      }
      return false;
    });
    return out;
  }

  virtual void addNode(Node n) { nodes_.insert(n); }

  virtual void addEdge(Node start, Node end, Cost cost) {
    addNode(start);
    addNode(end);
    using vt = typename decltype(edges_)::value_type;
    Edge<Node, Cost> e{start, end, cost};
    edges_.insert(vt{start, e});
  }
};

template <typename Node, typename Cost>
struct UnDirectedGraph : public DirectedGraph<Node, Cost> {
  virtual void addEdge(Node start, Node end, Cost cost) override {
    DirectedGraph<Node, Cost>::addEdge(start, end, cost);
    DirectedGraph<Node, Cost>::addEdge(end, start, cost);
  }
};

template <typename Node, typename Cost>
struct FW_Preds {
  map<pair<Node, Node>, Node> next;
  map<pair<Node, Node>, Cost> cost;

  vector<Node> path(Node start, Node end) {
    vector<Node> out;
    while (start != end) {
      out.push_back(start);
      start = next({start, end});
    }
    out.push_back(end);
    return out;
  }
};

template <typename Cost>
struct FW_PredsCompact {
  vector<vector<int>> next;
  vector<vector<Cost>> cost;

  vector<int> path(int start, int end) {
    vector<int> out;
    while (start != end) {
      out.push_back(start);
      start = next[start][end];
    }
    out.push_back(end);
    return out;
  }
};

template <typename Node, typename Cost>
FW_Preds<Node, Cost> AllShortestPaths(const Graph<Node, Cost>& g) {
  auto nodes = g.nodes();
  FW_Preds<Node, Cost> out;

  for (const auto& s : nodes) {
    out.cost[{s, s}] = (Cost)0;
    out.next[{s, s}] = s;

    g.forEachEdge(s, [&](auto e) {
      out.cost[{e.start, e.dest}] = e.cost;
      out.next[{e.start, e.dest}] = e.dest;
      return false;
    });
  }
  for (const auto& k : nodes) {
    for (const auto& s : nodes) {
      if (k == s)
        continue;
      for (const auto& e : nodes) {
        if (e == s || k == e)
          continue;
        auto c_sk = out.cost.find({s, k});
        auto c_ke = out.cost.find({k, e});
        auto cend = out.cost.end();
        if (c_sk != cend && c_ke != cend) {
          auto cost = c_sk->second + c_ke->second;
          if (auto iter = out.cost.find({s, e});
              iter == out.cost.end() || (iter->second > cost)) {
            out.next[{s, e}] = out.next[{s, k}];
            out.cost[{s, e}] = cost;
          }
        }
      }
    }
  }

  return out;
}

template <typename Cost>
FW_PredsCompact<Cost> AllShortestPathsCompact(const Graph<int, Cost>& g) {
  auto nodes = g.nodes();
  FW_PredsCompact<Cost> out;
  auto count = g.nodes().size();
  out.next.resize(count, vector<int>(count, -1));
  out.cost.resize(count, vector<Cost>(count, (Cost)-1));

  for (const auto& s : nodes) {
    out.cost[s][s] = (Cost)0;
    out.next[s][s] = s;

    g.forEachEdge(s, [&](auto e) {
      out.cost[e.start][e.dest] = e.cost;
      out.next[e.start][e.dest] = e.dest;
      return false;
    });
  }
  for (const auto& k : nodes) {
    for (const auto& s : nodes) {
      if (k == s)
        continue;
      for (const auto& e : nodes) {
        if (e == s || k == e)
          continue;
        auto c_sk = out.cost[s][k];
        auto c_ke = out.cost[k][e];
        if (c_sk >= 0 && c_ke >= 0) {
          auto cost = c_sk + c_ke;
          if (auto oldc = out.cost[s][e]; oldc < 0 || oldc > cost) {
            out.next[s][e] = out.next[s][k];
            out.cost[s][e] = cost;
          }
        }
      }
    }
  }

  return out;
}

}  // helpers

ifstream _cin("sample.txt");

vector<vector<string>> tiles{
    vector<string>{"..@@@@."},
    vector<string>{"...@...", "..@@@..", "...@..."},
    vector<string>{"....@..", "....@..", "..@@@.."},
    vector<string>{"..@....", "..@....", "..@....", "..@...."},
    vector<string>{"..@@...", "..@@..."}
};

ostream& operator<<(ostream& out, vector<string>& lines) {
  for (auto& l : lines)
    out << l << endl;
  return out;
}

ostream& operator<<(ostream& out, deque<string>& lines) {
  for (auto& l : lines)
    out << l << endl;
  return out;
}

deque<string> tower;

bool can_drop(vector<string>& tile, int inset) {
  for (auto& line : tile) {
    for (int i = 0; i < 7; ++i) {
      if (inset == tower.size() || tower[inset][i] == '#' && line[i] == '@')
        return false;
    }
    ++inset;
  }
  return true;
}

void shift(vector<string>& lines, bool dir) {
  for (auto& l : lines) {
    if ((dir ? l.front() : l.back()) == '@')
      return;
  }

  for (auto& l : lines) {
    if (dir) {
      l = l.substr(1) + ".";
    } else {
      l.pop_back();
      l = "." + l;
    }
  }
}

map<string, pair<int64_t, int64_t>> known;

string encode(int c, int m) {
  stringstream ss;
  ss << c << m;
  for (auto& l : tower) {
    ss << l;
  }
  return ss.str();
}

void solve(int64_t limit) {
  string commands;
  _cin >> commands;
  int c = 0;
  {
    int row = -4;
    auto tile = tiles[0];
    for (; row < 0; ++row) {
      shift(tile, commands[c++ % commands.length()] == '<');
    }
    tower.push_back(tile.front());
    for (auto i = 0; i < 7; ++i)
      if (tower[0][i] == '@')
        tower[0][i] = '#';
  }
  int64_t lost_rows = 0;
  for (int64_t i = 1; i < limit; ++i) {
    auto tile = tiles[i % 5];
    for (int o = 0; o < tile.size() + 3; ++o)
      tower.push_front(".......");
    int row = 0;
    do {
      auto com = commands[c++ % commands.length()] == '<';
      shift(tile, com);
      if (!can_drop(tile, row))
        shift(tile, !com);
      ++row;
      // cout << tile << row << endl;
    } while (can_drop(tile, row));

    --row;
    for (auto& line : tile) {
      for (auto o = 0; o < 7; ++o)
        if (line[o] == '@')
          tower[row][o] = '#';
      ++row;
    }

    while (tower.front() == ".......")
      tower.pop_front();

    if (c >= commands.size()) {
      int blocked = 0;
      for (int64_t r=0; r < tower.size(); ++r) {
        for (int i = 0; i < 7; ++i) {
          blocked |= (tower[r][i] == '#') << i;
        }
        if (blocked == (1 << 7) - 1) {
          lost_rows += tower.size() - (r + 1);
          tower.erase(tower.begin() + r + 1, tower.end());
          break;
        }
      }
      c = c % commands.size();
      auto enc = encode(c, i % 5);
      if (known.count(enc)) {
        auto diff = i - known[enc].first;
        auto reps = (limit - i) / diff;
        i += diff * reps;
        lost_rows += (lost_rows - known[enc].second) * reps;
      } else {
        known[enc] = {i, lost_rows};
      }
    }
  }
  cout << lost_rows + (int64_t)tower.size() << endl;
}

void first() {
  solve(2022);
}
void second() {
  solve(1000000000000);
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  //first();
  second();
  cout << "Time elapsed: "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - start)
              .count()
       << "ms" << endl;
}