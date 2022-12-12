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

vector<vector<int>> monkey_inv;

struct op {
  bool mul;
  bool ref;
  int64_t count;

  int64_t operator()(int64_t in) {
    auto oper = count;
    if (ref)
      oper = in;
    if (mul)
      return oper * in;
    return oper + in;
  }

};
ostream& operator<<(ostream& out, op o) {
  return out << o.mul << ' ' << o.count << endl;
}

struct test {
  int mod;
  int tv;
  int fv;

  int operator()(int64_t v) {
    return (v % mod == 0 ? tv : fv);
  }
};

struct monkey {
  vector<int64_t> inv_;
  op op_;
  test test_;
  int64_t ins_;
};

bool read_monkey(monkey& in) {
  string s;

  in.inv_.clear();
  in.ins_ = 0;
  // monkey X:
  if (!getline(_cin, s))
    return false;

  _cin >> s >> s; // starting items:
  getline(_cin, s);
  auto v = split(s, ",");
  for (auto& str : v) {
    in.inv_.push_back(atoi(str.c_str()));
  }
  {
    string oper;
    getline(_cin, s);
    stringstream ss(s);
    ss >> s >> s >> s >> s >> s >> oper;
    in.op_.mul = (s == "*");
    in.op_.ref = (oper == "old");
    in.op_.count = (in.op_.ref ? 0 : atoi(oper.c_str()));
  }
  _cin >> s >> s >> s >> in.test_.mod >> s >> s >> s >> s >> s >> in.test_.tv >>
      s >> s >> s >> s >> s >> in.test_.fv;
  getline(_cin, s);
  getline(_cin, s);
  return true;
}

void do_round(vector<monkey>& monkeys) {
  for (auto& m : monkeys) {
    for (auto& i : m.inv_) {
      auto worry = m.op_(i) / 3;
      monkeys[m.test_(worry)].inv_.push_back(worry);
      ++m.ins_;
    }
    m.inv_.clear();
  }
}

void do_round2(vector<monkey>& monkeys, int64_t worry_mod) {
  for (auto& m : monkeys) {
    for (auto& i : m.inv_) {
      auto worry = m.op_(i) % worry_mod;
      monkeys[m.test_(worry)].inv_.push_back(worry);
      ++m.ins_;
    }
    m.inv_.clear();
  }
}

void first() {
  monkey m;
  vector<monkey> ms;
  while (read_monkey(m)) {
    ms.push_back(m);
  }



  for (int i = 0; i < 20; ++i)
    do_round(ms);

  vector<uint64_t> counts;

  for (auto& mo : ms)
    counts.push_back(mo.ins_);

  sort(counts.begin(), counts.end());

  cout << *counts.rbegin() * *(counts.rbegin() + 1);
}

void second() {
  monkey m;
  vector<monkey> ms;
  int64_t worry_mod = 1;
  while (read_monkey(m)) {
    ms.push_back(m);
    worry_mod *= m.test_.mod;
  }

  for (int i = 0; i < 10000; ++i)
    do_round2(ms, worry_mod);

  vector<uint64_t> counts;

  for (auto& mo : ms)
    counts.push_back(mo.ins_);

  sort(counts.begin(), counts.end());

  cout << *counts.rbegin() * *(counts.rbegin() + 1);
}


int main()
{
  //first();
  second();
  return 0;
}