#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

map<char, int64_t> tv{{'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};


int64_t convert(string s) {
  int64_t out = 0;

  for (auto& c : s) {
    out *= 5;
    out += tv[c];
  }

  return out;
}

string convert(int64_t n) {
  string out = "";

  do {
    switch (n % 5) {
      case 0:
      case 1:
      case 2:
        out = (char)('0' + (n%5)) + out;
        break;
      case 3:
        out = '=' + out;
        n += 5;
        break;
      case 4:
        out = '-' + out;
        n += 5;
        break;
    };
    n /= 5;
  } while (n);

  return out;
}

void first() {
  int64_t out = 0;
  string s;

  while (_cin >> s)
    out += convert(s);

  cout << convert(out) << endl;
}

void second() {
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  first();
  //second();
  cout << "Time elapsed: "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - start)
              .count()
       << "ms" << endl;
}