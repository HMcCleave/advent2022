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

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

int beatenby(int in) {
  switch (in) {
    case ROCK:
      return PAPER;
    case PAPER:
      return SCISSORS;
    default:
      return ROCK;
  }
}

int score(int opp, int me) {
  if (opp == me) {
    return 3 + me;
  }
  if (opp == beatenby(me)) {
    return me;
  }
  return 6 + me;
}

void first() {
  char opp, me;
  int total = 0;
  while (cin >> opp >> me) {
    total += score(1 + (opp - 'A'), 1 + (me - 'X'));
  }
  cout << total << endl;
}

void second() {
  char opp, res;
  int total = 0;
  while (cin >> opp >> res) {
    int iopp = 1 + (opp - 'A');
    int me = iopp;
    if (res == 'X') {
      me = beatenby(beatenby(iopp));
    } else if (res == 'Z') {
      me = beatenby(iopp);
    }
    total += score(iopp, me);
  }
  cout << total;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}