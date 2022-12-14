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

struct node {
  vector<node> children;
  optional<int> value;

  node() = default;
  node(int v) : value(v) {}
  node(vector<node> c) : children(std::move(c)) {}

  bool has_value() const { return value.has_value();
  }

  vector<node> as_children() const {
    vector<node> out;
    if (has_value())
      out.emplace_back(value.value());
    return out;
  }

  node as_list() const {
    if (has_value())
      return node(as_children());
    return *this;
  }
};


ostream& operator<<(ostream& out, const node& n) {
  if (n.has_value())
    return out << n.value.value();
  out << "[";
  if (n.children.size())
    out << n.children[0];
  for (int i = 1; i < n.children.size(); ++i)
    out << "," << n.children[i];
  return out << "]";
}

const int IN_ORDER = -1;
const int UNSURE = 0;
const int OUT_ORDER = 1;

int compare(const node& a, const node& b) {
  if (a.has_value() != b.has_value()) {
    return compare(a.as_list(), b.as_list());
  }
  if (a.has_value() && b.has_value()) {
    if (a.value.value() < b.value.value()) {
      return IN_ORDER;
    }
    if (a.value.value() == b.value.value()) {
      return UNSURE;
    }
    return OUT_ORDER;
  }
  for (int i = 0;; ++i) {
    if (i == a.children.size())
      return i == b.children.size() ? UNSURE : IN_ORDER;
    if (i == b.children.size())
      return OUT_ORDER;
    auto res = compare(a.children[i], b.children[i]);
    if (res != UNSURE)
      return res;
  }
  return -2;
}

bool operator<(node& a, node& b) {
  return compare(a, b) != OUT_ORDER;
}

bool operator==(const node& a, const node& b) {
  if (a.children.size() != b.children.size())
    return false;
  if (a.children.empty())
    return a.value == b.value;
  for (int i = 0; i < a.children.size(); ++i) {
    if (!(a.children[i] == b.children[i]))
      return false;
  }
  return true;
}

node parse(string s) {
  node out;
  if (s.length() == 0)
    return out;
  if (s[0] != '[') {
    out.value = atoi(s.c_str());
    return out;
  }
  s = s.substr(1, s.length() - 2); // []

  while (s.length()) {
    int depth = 0;
    int pos = 0;
    for (; pos < s.length() && (depth != 0 || s[pos] != ','); ++pos) {
      if (s[pos] == '[')
        ++depth;
      if (s[pos] == ']')
        --depth;
    }
    string token = s.substr(0, pos);
    if (pos == s.length())
      s = "";
    else
      s = s.substr(pos + 1);
    if (token.length() != 0) {
      out.children.push_back(parse(token));
    }
  }
  return out;
}

void first() {
  string a, b;
  int total = 0;
  int index = 0;
  while (_cin >> a >> b) {
    auto nodeA = parse(a);
    auto nodeB = parse(b);
    //cerr << a << ":" << nodeA << endl << b << ":" << nodeB << endl;
    ++index;
    if (OUT_ORDER != compare(nodeA,nodeB)) {
      total += index;
    }
  }
  cout << total << endl;
}

void second() {
  vector<node> nodes;
  node start = parse("[[2]]");
  node end = parse("[[6]]");
  nodes.push_back(start);
  nodes.push_back(end);
  string s;
  while (_cin >> s)
    nodes.push_back(parse(s));
  sort(nodes.begin(), nodes.end());
  auto iter1 = find(nodes.begin(), nodes.end(), start);
  auto iter2 = find(nodes.begin(), nodes.end(), end);
  cout << (distance(nodes.begin(), iter1) + 1) *
              (distance(nodes.begin(), iter2) + 1)
       << endl;
}


int main()
{
  //first();
  second();
  return 0;
}