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
    out.emplace_back(large.begin(), large.begin() + iter);
    pos = iter + marker.length();
  }
  out.push_back(large.substr(pos));
  return out;
}

using point = pair<int, int>;

int taxi_dist(const point& a, const point& b) {
  return abs(a.first - b.first) + abs(a.second - b.second);
}

point operator*(const point& a, int scale) {
  return {a.first * scale, a.second * scale};
}

point operator-(const point& a, const point& b) {
  return {a.first - b.first, a.second - b.second};
}
point operator+(const point& a, const point& b) {
  return a - (b * -1);
}


ostream& operator<<(ostream& out, const point& p) {
  return out << "(" << p.first << "," << p.second << ")";
}

vector<pair<point, point>> input;

int minx, maxx;

void parse() {
  string s;
  bool start = true;
  while (getline(_cin, s)) {
    auto& data = input.emplace_back();
    sscanf_s(s.c_str(), "Sensor at x=%i, y=%i: closest beacon is at x=%i, y=%i",
           &data.first.first, &data.first.second, &data.second.first,
           &data.second.second);
    if (start) {
      start = false;
      minx = maxx = data.first.first;
    }
    minx = min(minx, data.first.first - taxi_dist(data.first, data.second));
    maxx = max(maxx, data.first.first + taxi_dist(data.first, data.second));
    //cerr << data.first << ", " << data.second << endl;
  }
  //cerr << minx << ' ' << maxx << endl;
}

void first() {
  parse();
  int total = 0;
  for (auto i = minx; i <= maxx; ++i) {
    const point p = {i, 2000000};
    bool valid = true;
    for (const auto& line : input) {
      if (p != line.second && taxi_dist(p, line.first) <= taxi_dist(line.first, line.second)) {
        valid = false;
        break;
      }
    }
    if (!valid)
      ++total;
  }
  cout << total << endl;
}

point from_iter(const point& source, const int dist, const int iter) {
  point out;

  if (iter < 2 * dist) {
    out.first = iter - dist;
  } else {
    out.first = 3 * dist - iter;
  }

  if (iter <= dist) {
    out.second = iter;
  } else if (iter <= 3 * dist) {
    out.second = 2 * dist - iter;
  } else {
    out.second = iter - (4 * dist);
  }
  return out + source;
}


// I should have thought about not running in debug for this...
// debug: 68 seconds
// release: 1.7 seconds
void second() {
  parse();
  for (const auto& line : input) {
    const auto& source = line.first;
    const auto dist = taxi_dist(line.first, line.second) + 1;
    for (int i = 0; i < 4 * dist; ++i) {
      const auto p = from_iter(source, dist, i);
      if (min(p.first, p.second) >= 0 && max(p.first, p.second) <= 4000000) {
        bool valid = true;
        if (all_of(input.begin(), input.end(), [&p](const auto& s) {
              return taxi_dist(p, s.first) > taxi_dist(s.first, s.second);
            })) {
          cout << (uint64_t)(p.first) * 4000000 + (uint64_t)p.second << endl;
          return;
        }
      }
    }
  }
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