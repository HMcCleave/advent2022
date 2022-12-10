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
int x = 1;
int cycle = 0;
int sig = 0;

string grid[6];


void tick() {
  ++cycle;
  switch (cycle) {
    case 20:
    case 60:
    case 100:
    case 140:
    case 180:
    case 220:
      sig += x * cycle;
      break;
    default:
      break;
  };
  if (cycle <= 240) {
    int row = (cycle - 1) / 40;
    int pixel = (cycle - 1) % 40;
    if (row < 6)
      grid[row][pixel] = (x >= (pixel-1) && x <= (pixel + 1)) ? '#' : '.';
  }
}






void first() {
  string cmd;
  while (_cin >> cmd) {
    tick();
    int val;
    if (cmd != "noop") {
      tick();
      _cin >> val;
      x += val;
    }
  }
  cout << sig << endl;
}

void second() {
  for (auto& s : grid)
    s = string(40, '#');
  string cmd;
  while (_cin >> cmd) {
    tick();
    int val;
    if (cmd != "noop") {
      tick();
      _cin >> val;
      x += val;
    }
  }
  for (auto& s : grid)
    cout << s << endl;
}


int main()
{
  string s;
  //first();
  second();
  return 0;
}