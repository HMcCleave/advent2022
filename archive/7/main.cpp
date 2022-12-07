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
struct dir;

dir* find_dir(string path);

map<string, unique_ptr<dir>> global_dir;

struct dir {
  string path;
  set<string> subs;
  map<string, uint64_t> files;

  uint64_t size() { 
    uint64_t out = 0;
    for (auto& d : subs)
      out += find_dir(d)->size();
    for (auto& f : files)
      out += f.second;
    return out;
  }
};

dir* find_dir(string path) {
  if (global_dir.find(path) == global_dir.end()) {
    global_dir[path] = make_unique<dir>();
    global_dir[path]->path = path;
  }
  return global_dir[path].get();
}



void first() {
  stack<dir*> path;
  path.push(find_dir("/"));

  string line;
  while (getline(cin, line)) {
    if (line.find("$ ls") == 0) {
      continue;
    } else if (line.find("dir ") == 0) {
      string name = string(line.c_str() + 4);
      path.top()->subs.insert(path.top()->path + "/" + name);
    } else if (line.find("$ cd ..") == 0) {
      path.pop();
    } else if (line.find("$ cd") == 0) {
      string name = string(line.c_str() + 5);
      path.push(find_dir(path.top()->path + "/" + name));
    } else if (line[0] >= '1' && line[0] <= '9') {
      stringstream ss(line);
      string name;
      uint64_t size;
      ss >> size >> name;
      path.top()->files[name] = size;
    }
  }

  uint64_t out = 0;
  for (auto& dir : global_dir) {
    if (dir.second->size() <= 100000) {
      out += dir.second->size();
    }
  }
  cout << out << endl;
}

void second() {
  stack<dir*> path;
  path.push(find_dir("/"));

  string line;
  while (getline(cin, line)) {
    if (line.find("$ ls") == 0) {
      continue;
    } else if (line.find("dir ") == 0) {
      string name = string(line.c_str() + 4);
      path.top()->subs.insert(path.top()->path + "/" + name);
    } else if (line.find("$ cd ..") == 0) {
      path.pop();
    } else if (line.find("$ cd") == 0) {
      string name = string(line.c_str() + 5);
      path.push(find_dir(path.top()->path + "/" + name));
    } else if (line[0] >= '1' && line[0] <= '9') {
      stringstream ss(line);
      string name;
      uint64_t size;
      ss >> size >> name;
      path.top()->files[name] = size;
    }
  }

  string best = "/";
  uint64_t min = 30000000 - (70000000 - find_dir("/")->size());
  uint64_t cost = find_dir("/")->size();
  ;
  for (auto& dir : global_dir) {
    uint64_t cand = dir.second->size();
    if (cand >= min && cand < cost) {
      cost = cand;
      best = dir.first;
    }
  }
  cout << cost << endl;
}

int main() {
#ifdef SECOND
  second();
#else
  first();
#endif
}