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

vector<string> split(const string& large, const string& marker) {
  vector<string> out;
  size_t iter = 0;
  size_t pos = 0;
  while ((iter = large.find(marker, pos)) != string::npos) {
    out.emplace_back(large.begin(), large.begin() + iter);
    pos = iter + marker.length();
  }
  out.push_back(large.substr(pos));
  return out;
}

typedef pair<int, int> point;

vector<pair<point, point>> input;

point minp, maxp;

point operator-(const point& a, const point& b) {
  return {a.first - b.first, a.second - b.second};
}

point operator+(const point& a, const point& b) {
  return {a.first + b.first, a.second + b.second};
}

point dir(const point& a, const point& b) {
  point out = b - a;
  if (out.first != 0)
    out.first /= abs(out.first);
  if (out.second != 0)
    out.second /= abs(out.second);
  return out;
}

point adjust(const point& i) {
  return i - minp;
}

vector<string> grid;

void print_grid() {
  for (auto& s : grid)
    cerr << s << endl;
}

void fill_grid() {
  grid.clear();
  auto dim = adjust(maxp);
  for (auto i = 0; i <= dim.second; ++i) {
    grid.emplace_back(dim.first + 1, '.');
  }
  for (auto& line : input) {
    auto start = adjust(line.first);
    auto end = adjust(line.second);
    auto step = dir(start, end);
    grid[end.second][end.first] = '#';
    do {
      grid[start.second][start.first] = '#';
      start = start + step;
    } while (start != end);
  }
}

bool can_drop(point location) {
  if (location.first < 0 || location.first > grid[0].length() ||
      location.second >= grid.size())
    return true;
  return grid[location.second][location.first] == '.';
}

bool drop_sand(point location) {
  if (location.first < 0 || location.first > grid[0].length() ||
      location.second >= grid.size())
    return true;
  if (can_drop(location + point{0, 1}))
    return drop_sand(location + point{0, 1});
  if (can_drop(location + point{-1, 1}))
    return drop_sand(location + point{-1, 1});
  if (can_drop(location + point{1, 1}))
    return drop_sand(location + point{1, 1});
  grid[location.second][location.first] = 'O';
  return false;
}

bool can_drop2(point location) {
  if (location.second == grid.size())
    return false;
  if (location.first < 0 || location.first > grid[0].length())
    return true;
  return grid[location.second][location.first] == '.';
}

bool drop_sand2(point location) {
  if (can_drop2(location + point{0, 1}))
    return drop_sand2(location + point{0, 1});
  if (can_drop2(location + point{-1, 1}))
    return drop_sand2(location + point{-1, 1});
  if (can_drop2(location + point{1, 1}))
    return drop_sand2(location + point{1, 1});
  grid[location.second][location.first] = 'O';
  return false;
}



void parse() {
  string s;
  minp = {500, 0};
  maxp = {500, 0};
  while (getline(_cin, s)) {
    stringstream ss(s);
    char c;
    string t;
    point start, end;
    if (s == "")
      continue;
    ss >> start.first >> c >> start.second;
    minp.first = min(minp.first, start.first);
    minp.second = min(minp.second, start.second);
    maxp.first = max(maxp.first, start.first);
    maxp.second = max(maxp.second, start.second);
    
    while (ss >> t >> end.first >> c >> end.second) {
      input.emplace_back(start, end);
      start = end;
      //if (start.first != end.first && start.second != end.second)
      //  cerr << "oh no, way too complicated" << endl;
      minp.first = min(minp.first, end.first);
      minp.second = min(minp.second, end.second);
      maxp.first = max(maxp.first, end.first);
      maxp.second = max(maxp.second, end.second);
    }
  }
}

void first() {
  parse();
  fill_grid();
  //print_grid();
  int out = 0;
  while (!drop_sand(adjust({500, 0}))) {
    ++out;
    // print_grid();
    // cerr << endl << endl;
  }
  cout << out << endl;
}

void second() {
  parse();
  auto height = adjust(maxp).second + 2;
  minp.first = min(minp.first, 500 - height);
  maxp.first = max(maxp.first, 500 + height);
  fill_grid();
  grid.emplace_back(adjust(maxp).first, '.');
  // print_grid();
  int out = 0;
  while (can_drop2(adjust({500, 0}))) {
    drop_sand2(adjust({500, 0}));
    ++out;
    //print_grid();
    //cerr << endl << endl;
  }
  cout << out << endl;
}

void what_second_should_have_been_if_i_wasnt_a_god_damned_idiot_that_wasted_5_minutes_duplicating_code_and_5_minutes_debugging_forgetting_to_put_2s_at_the_end_of_calls_only_to_realize_i_needed_to_adjust_x_limits_to_500_plusminus_height_first() {
  parse();
  auto height = adjust(maxp).second + 2;
  minp.first = min(minp.first, 500 - height);
  maxp.first = max(maxp.first, 500 + height);
  fill_grid();
  grid.emplace_back(adjust(maxp).first, '.');
  grid.emplace_back(adjust(maxp).first, '#');
  // print_grid();
  int out = 0;
  while (can_drop(adjust({500, 0}))) {
    drop_sand(adjust({500, 0}));
    ++out;
    // print_grid();
    // cerr << endl << endl;
  }
  cout << out << endl;
}


int main()
{
  //first();
  second();
  return 0;
}