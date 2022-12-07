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

using namespace std;

void first() {
  int total = 0;
  char d;
  int s1, s2, e1, e2;
  while (cin >> s1 >> d >> e1 >> d >> s2 >> d >> e2) {
    if ((s1 <= s2 && e1 >= e2) || (s2 <= s1 && e2 >= e1)) {
      ++total;
    }
  }
  cout << total << endl;
}

bool contains(int a, int b, int c) {
  return !(c < a || c > b);
}

bool overlap(int a, int b, int c, int d) {
  return contains(a, b, c) || contains(a, b, d) || contains(c, d, a) ||
         contains(c, d, b);
}

void second() {
  int total = 0;
  char d;
  int s1, s2, e1, e2;
  while (cin >> s1 >> d >> e1 >> d >> s2 >> d >> e2) {
    if (overlap(s1, e1, s2, e2)) {
      ++total;
    }
  }
  cout << total << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}