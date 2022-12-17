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

ifstream _cin("input.txt");

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

template<typename Node, typename Cost>
struct Edge {
  Node start;
  Node dest;
  Cost cost;

  bool operator<(const Edge& that) const {
    return cost < that.cost;
  }
};

template<typename Node, typename Cost>
struct Graph {
  using EdgeType = Edge<Node, Cost>;
  virtual ~Graph() {}

  virtual set<Node> nodes() const = 0;
  virtual int forEachEdge(Node start, function<bool(EdgeType)>) const = 0;
  virtual optional<EdgeType> getEdge(Node start, Node dest) const = 0;
};

template<typename Node, typename Cost>
vector<Edge<Node, Cost>> shortestPath(const Graph<Node, Cost>& g, Node start, Node end) {
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

template<typename Node, typename Cost>
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

template<typename Node, typename Cost>
struct DirectedGraph : public Graph<Node, Cost> {
  set<Node> nodes_;
  multimap<Node, Edge<Node, Cost>> edges_;

  virtual ~DirectedGraph() {}

  virtual set<Node> nodes() const override{ 
    return nodes_;
  }

  virtual int forEachEdge(Node start, function<bool(Edge<Node, Cost>)> func) const {
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

  virtual void addNode(Node n) {
    nodes_.insert(n);
  }

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

template<typename Node, typename Cost>
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

template<typename Node, typename Cost>
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
          if (auto oldc = out.cost[s][e];
              oldc < 0 || oldc > cost) {
            out.next[s][e] = out.next[s][k];
            out.cost[s][e] = cost;
          }
        }
      }
    }
  }

  return out;
}

namespace slow {
using DG = DirectedGraph<string, int>;

DG connections;
map<string, int> values;
map<pair<string, string>, int> distances;

void solve(int& best,
           int time,
           vector<string>& unvisited,
           int score,
           string location) {
  best = max(best, score);
  if (time <= 1)
    return;
  for (auto& node : unvisited) {
    auto iter = distances.find({location, node});
    if (node != "" && time > iter->second) {
      string t = "";
      swap(t, node);
      int newt = time - (iter->second + 1);
      solve(best, newt, unvisited, score + (values[t] * newt), t);
      swap(t, node);
    }
  }
}

void solve2(int& best,
            int time,
            vector<string>& unvisited,
            int score,
            string locationy,
            string locatione,
            int ny,
            int ne) {
  best = max(best, score);
  while (ny && ne) {
    --time;
    --ny;
    --ne;
  }

  if (time <= 1)
    return;

  bool ele = (ne == 0);

  for (auto& node : unvisited) {
    auto iter = distances.find({(ele ? locatione : locationy), node});
    if (node != "" && time > (iter->second + 1)) {
      string t = "";
      swap(t, node);
      int next_score = score;
      if (ele) {
        ne = iter->second + 1;
        next_score += max(0, (time - ne) * values[t]);
        solve2(best, time, unvisited, next_score, locationy, t, ny, ne);
      } else {
        ny = iter->second + 1;
        next_score += max(0, (time - ny) * values[t]);
        solve2(best, time, unvisited, next_score, t, locatione, ny, ne);
      }
      swap(t, node);
    }
  }
}

void first() {
  string line;
  while (getline(_cin, line)) {
    string node, d;
    stringstream ss(line);
    ss >> d >> node >> d >> d >> line >> d >> d >> d >> d;
    int v;
    sscanf_s(line.c_str(), "rate=%i;", &v);
    values[node] = v;
    char buf[2];
    ss.read(buf, 1);  // ' '
    getline(ss, d);
    auto dests = split(string(d), ", ");
    for (auto& dest : dests) {
      connections.addEdge(node, dest, 1);
    }
  };
  int best = 0;
  vector<string> unvisited;
  for (auto& iter : values) {
    if (iter.second > 0) {
      unvisited.push_back(iter.first);
    }
  }
  sort(unvisited.begin(), unvisited.end(),
       [&](const string& a, const string& b) { return values[a] > values[b]; });

  distances = AllShortestPaths(connections).cost;

  solve(best, 30, unvisited, 0, "AA");
  cout << best << endl;
}

void second() {
  string line;
  while (getline(_cin, line)) {
    string node, d;
    stringstream ss(line);
    ss >> d >> node >> d >> d >> line >> d >> d >> d >> d;
    int v;
    sscanf_s(line.c_str(), "rate=%i;", &v);
    values[node] = v;
    char buf[2];
    ss.read(buf, 1);  // ' '
    getline(ss, d);
    auto dests = split(string(d), ", ");
    for (auto& dest : dests) {
      connections.addEdge(node, dest, 1);
    }
  };
  int best = 0;
  vector<string> unvisited;
  for (auto& iter : values) {
    if (iter.second > 0) {
      unvisited.push_back(iter.first);
    }
  }
  sort(unvisited.begin(), unvisited.end(),
       [&](const string& a, const string& b) { return values[a] > values[b]; });

  distances = AllShortestPaths(connections).cost;

  solve2(best, 26, unvisited, 0, "AA", "AA", 0, 0);
  cout << best << endl;
}

}  // namespace slow

using DG = DirectedGraph<int, int>;

DG connections;
vector<int> values;
vector<vector<int>> distances;
map<string, int> lookup;

void solve2_faster(int& best,
            int time,
            vector<int>& unvisited,
            int score,
            int locationy,
            int locatione,
            int ny,
            int ne) {
  best = max(best, score);
  while (ny && ne) {
    --time;
    --ny;
    --ne;
  }

  if (time <= 1)
    return;

  { 
    int t = time - ny - ne + 1;
    int over_score = score;
    for (auto& node : unvisited) {
      if (node != -1) {
        over_score += values[node] * (t--);
      }
      if (t <= 0)
        break;
    }
    if (over_score < best)
      return;
  }

  bool ele = (ne == 0);

  for (auto& node : unvisited) {
    if (node == -1)
      continue;
    auto dist = distances[ele ? locatione : locationy][node];
    if (dist >= 0 && time > (dist + 1)) {
      int t = -1;
      swap(t, node);
      int next_score = score;
      if (ele) {
        ne = dist + 1;
        next_score += max(0, (time - ne) * values[t]);
        solve2_faster(best, time, unvisited, next_score, locationy, t, ny, ne);
      } else {
        ny = dist + 1;
        next_score += max(0, (time - ny) * values[t]);
        solve2_faster(best, time, unvisited, next_score, t, locatione, ny, ne);
      }
      swap(t, node);
    }
  }
}

int last_index = 0;
int get_index(string s) {
  if (auto iter = lookup.find(s); iter != lookup.end())
    return iter->second;
  return lookup[s] = last_index++;
}

void second() {
  string line;
  map<int, int> tvalues;
  while (getline(_cin, line)) {
    string node, d;
    int inode;
    stringstream ss(line);
    ss >> d >> node >> d >> d >> line >> d >> d >> d >> d;
    int v;
    sscanf_s(line.c_str(), "rate=%i;", &v);
    tvalues[inode = get_index(node)] = v;
    char buf[2];
    ss.read(buf, 1);  // ' '
    getline(ss, d);
    auto dests = split(string(d), ", ");
    for (auto& dest : dests) {
      connections.addEdge(inode, get_index(dest), 1);
    }
  };
  
  values.resize(last_index);
  for (auto& iter : tvalues)
    values[iter.first] = iter.second;

  int best = 0;
  vector<int> unvisited;
  for (auto i = 0; i < values.size(); ++i) {
    if (values[i] > 0) {
      unvisited.push_back(i);
    }
  }
  sort(unvisited.begin(), unvisited.end(),
       [&](const int& a, const int& b) { return values[a] > values[b]; });

  distances = AllShortestPathsCompact(connections).cost;

  solve2_faster(best, 26, unvisited, 0, get_index("AA"), get_index("AA"), 0, 0);
  cout << best << endl;
}


int main()
{
  auto start = chrono::high_resolution_clock::now();
  //slow::first();
  //slow::second();
  second();
  cout << "Time elapsed: "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - start)
              .count()
       << "ms" << endl;
}