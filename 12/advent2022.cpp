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

vector<string> split(string large, const string& marker) {
  vector<string> out;
  size_t iter = 0;
  while ((iter = large.find(marker)) != string::npos) {
    out.emplace_back(large.begin(), large.begin() + iter);
    large = large.substr(iter + marker.length());
  }
  out.push_back(large);
  return out;
}

vector<string> grid;
vector<vector<int>> state, new_state;;
pair<int, int> dest;

void read_grid() {
  string s;
  grid.clear();
  while (getline(_cin, s)) {
    grid.push_back(s);
  }
}

bool can_move(char a, char b) {
  if (a == 'S')
    a = 'a';
  if (b == 'S')
    b = 'a';
  if (a == 'E')
    a = 'z';
  if (b == 'E')
    b = 'z';
  return (int)b - (int)a <= 1;
}

int test(int x, int y, int x2, int y2) {
  if (x2 < 0 || x2 >= grid[y].length() || y2 < 0 || y2 >= grid.size())
    return INT_MAX;
  if (!state[y2][x2])
    return INT_MAX;
  return can_move(grid[y2][x2], grid[y][x]) ? state[y2][x2] + 1 : INT_MAX;
}

void update_tile(int x, int y) {
  if (state[y][x])
    return;
  int cand = min(min(test(x, y, x - 1, y), test(x, y, x + 1, y)),
                 min(test(x, y, x, y - 1), test(x, y, x, y + 1)));

  new_state[y][x] = cand == INT_MAX ? state[y][x] : cand;
}

void do_turn() {
  for (int y = 0; y < grid.size(); ++y)
    for (int x = 0; x < grid[y].length(); ++x)
      update_tile(x, y);
}

void first() {
  read_grid();
  state.clear();
  for (int y = 0; y < grid.size(); ++y) {
    const auto& rin = grid[y];
    auto& row = state.emplace_back();
    for (int x = 0; x < rin.length(); ++x) {
      row.push_back(rin[x] == 'S' ? 1 : 0);
      if (rin[x] == 'E') {
        dest = {x, y};
      }
    }
  }
  new_state = state;
  while(!state[dest.second][dest.first]) {
    do_turn();
    state.swap(new_state);
  }
  cout << state[dest.second][dest.first] - 1 << endl;
}

void second() {
  read_grid();
  state.clear();
  for (int y = 0; y < grid.size(); ++y) {
    const auto& rin = grid[y];
    auto& row = state.emplace_back();
    for (int x = 0; x < rin.length(); ++x) {
      row.push_back(rin[x] == 'S' || rin[x] == 'a' ? 1 : 0);
      if (rin[x] == 'E') {
        dest = {x, y};
      }
    }
  }
  new_state = state;
  while (!state[dest.second][dest.first]) {
    do_turn();
    state.swap(new_state);
  }
  cout << state[dest.second][dest.first] - 1 << endl;
}


int main()
{
  //first();
  second();
  return 0;
}