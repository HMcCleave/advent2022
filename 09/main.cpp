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

int hx, hy, tx, ty;
int motion = 0;
set<pair<int, int>> visited;

void movestep(int dx, int dy) {
  hx += dx;
  hy += dy;
  if (hx - tx > 1) {
    ++tx;
    if (hy > ty) {
      ++ty;
    } else if (hy < ty) {
      --ty;
    }
  }
  else if (hx - tx < -1) {
    --tx;
    if (hy > ty) {
      ++ty;
    } else if (hy < ty) {
      --ty;
    }
  } else if (hy - ty > 1) {
    ++ty;
    if (hx > tx)
      ++tx;
    else if (hx < tx)
      --tx;
  } else if (hy - ty < -1) {
    --ty;
    if (hx > tx)
      ++tx;
    else if (hx < tx)
      --tx;
  }
  visited.insert({tx, ty});
}

pair<int, int> pos[10];

pair<int, int> operator+(pair<int, int> a, pair<int, int> b) {
  return {a.first + b.first, a.second + b.second};
}

int dist(pair<int, int> a, pair<int, int> b) {
  return max(abs(a.first - b.first),abs(a.second - b.second));
}

int cmp3(int a, int b) {
  if (a == b) return 0;
  return (a < b) ? -1 : 1;
}

ostream& operator<<(ostream& out, pair<int, int> p) {
  out << p.first << ',' << p.second;
  return out;
}

void movestep2(int index, int dx, int dy) {
  pos[index] = pos[index] + make_pair(dx, dy);
  if (index < 9) {
    if (dist(pos[index], pos[index + 1]) > 1) {
      movestep2(index + 1, cmp3(pos[index].first, pos[index+1].first), cmp3(pos[index].second, pos[index+1].second));
    }
  } else {
    visited.insert(pos[9]);
  }
}


void first() {
  char d;
  int n;
  visited.insert({0, 0});
  while (cin >> d >> n) {
    switch (d) {
      case 'R':
        for (int i = 1; i <= n; ++i)
          movestep(0, 1);
        break;
      case 'L':
        for (int i = 1; i <= n; ++i)
          movestep(0, -1);
        break;
      case 'U':
        for (int i = 1; i <= n; ++i)
          movestep(-1, 0);
        break;
      case 'D':
        for (int i = 1; i <= n; ++i)
          movestep(1, 0);
        break;
    };
  }
  cout << visited.size() << endl;
}

void second() {
  for (int i = 0; i < 10; ++i)
    pos[i] = make_pair(0, 0);
  char d;
  int n;
  visited.insert({0, 0});
  while (cin >> d >> n) {
    switch (d) {
      case 'R':
        for (int i = 1; i <= n; ++i)
          movestep2(0,0, 1);
        break;
      case 'L':
        for (int i = 1; i <= n; ++i)
          movestep2(0,0, -1);
        break;
      case 'U':
        for (int i = 1; i <= n; ++i)
          movestep2(0,-1, 0);
        break;
      case 'D':
        for (int i = 1; i <= n; ++i)
          movestep2(0,1, 0);
        break;
    };
  }
  cout << visited.size() << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}