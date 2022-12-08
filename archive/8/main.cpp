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

using namespace std;

struct tile {
  int v;
  bool l, r, u, d;
};

vector<vector<tile>> grid;

struct tile2 {
  int v;
  int l, r, u, d;
};

vector<vector<tile2>> grid2;

void input() {
  string line;
  while ( cin >> line) {
    auto& row = grid.emplace_back();
    stringstream ss(line);
    char c;
    while ( ss >> c) {
      row.push_back({c - '0', true, true, true, true});
    }
  }
  for (auto& row : grid) {
    int maxv = row[0].v;
    for (int i = 1; i < row.size(); ++i) {
      if (row[i].v <= maxv)
        row[i].l = false;
      maxv = max(maxv, row[i].v);
    }
    maxv = row.back().v;
    for (int i = row.size() - 2; i >= 0; --i) {
      if (row[i].v <= maxv)
        row[i].r = false;
      maxv = max(maxv, row[i].v);
    }
  }
  for (int i = 0; i < grid[0].size(); ++i) {
    int maxv = grid[0][i].v;
    for (int c = 1; c < grid.size(); ++c) {
      if (grid[c][i].v <= maxv) {
        grid[c][i].u = false;
      }
      maxv = max(maxv, grid[c][i].v);
    }
    maxv = grid.back()[i].v;
    for (int c = grid.size() - 2; c >= 0; --c) {
      if (grid[c][i].v <= maxv) {
        grid[c][i].d = false;
      }
      maxv = max(maxv, grid[c][i].v);
    }
  }
}

void input2() {
  string line;
  while (cin >> line) {
    auto& row = grid2.emplace_back();
    stringstream ss(line);
    char c;
    while (ss >> c) {
      row.push_back({c - '0', 0, 0, 0, 0});
    }
  }
  for (int x = 1; x < grid2.size() - 1; ++x) {
    for (int y = 1; y < grid2[x].size() - 1; ++y) {
      auto& tile = grid2[x][y];
      for (tile.l = 1; tile.l < x && grid2[x - tile.l][y].v < tile.v; ++tile.l)
        ;
      for (tile.r = 1; tile.r + x < grid2.size() -1 && grid2[x + tile.r][y].v < tile.v; ++tile.r)
        ;
      for (tile.u = 1; tile.u < y && grid2[x][y - tile.u].v < tile.v; ++tile.u)
        ;
      for (tile.d = 1;
           ((tile.d + y) < grid2[x].size() -1) && grid2[x][y + tile.d].v < tile.v;
           ++tile.d)
        ;
    }
  }
}




void first() {
  input();
  int total = 0;
  for (auto& row : grid) {
    for (auto& tile : row) {
      if (tile.l || tile.r || tile.u || tile.d)
        ++total;
    }
  }
  cout << total << endl;
}

void second() {
  input2();
  int out = 0;
  for (auto& row : grid2)
    for (auto& tile : row) {
      out = max(out, tile.l * tile.r * tile.d * tile.u);
    }
  cout << out << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}