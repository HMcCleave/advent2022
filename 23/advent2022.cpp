#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

deque<string> grid;
vector<pair<int, int>> elves;

tuple<int, int, int, int> bounds() {
  tuple<int, int, int, int> out;
  auto& [x1, y1, x2, y2] = out;
  x1 = x2 = elves[0].first;
  y1 = y2 = elves[0].second;

  for (auto& e : elves) {
    x1 = min(x1, e.first);
    x2 = max(x2, e.first);
    y1 = min(y1, e.second);
    y2 = max(y2, e.second);
  }

  return out;
}

void grow() {
  for (auto& s : grid) {
    s = "." + s + ".";
  }
  grid.push_back(string(grid.front().length(), '.'));
  grid.push_front(string(grid.front().length(), '.'));
  for (auto& [x, y] : elves) {
    ++x;
    ++y;
  }
}


void parse() {
  string s;
  while (getline(_cin, s)) {
    for (int i = 0; i < s.length(); ++i) {
      if (s[i] == '#') {
        elves.push_back(make_pair(i, grid.size()));
      }
    }
    grid.push_back(s);
  }
  auto [x1, y1, x2, y2] = bounds();
  if (min(x1, y1) == 0 || x2 == grid[0].length() - 1 || y2 == grid.size())
    grow();
}

bool should_move(pair<int, int> pos) {
  for (int x = pos.first - 1; x <= pos.first + 1; ++x) {
    for (int y = pos.second - 1; y <= pos.second + 1; ++y) {
      if (((x != pos.first) || (y != pos.second)) && grid[y][x] == '#')
        return true;
    }
  }
  return false;
}

bool can_move_north(pair<int, int> pos) {
  return grid[pos.second - 1][pos.first - 1] != '#' &&
         grid[pos.second - 1][pos.first] != '#' &&
         grid[pos.second - 1][pos.first + 1] != '#';
}

bool can_move_south(pair<int, int> pos) {
  return grid[pos.second + 1][pos.first - 1] != '#' &&
         grid[pos.second + 1][pos.first] != '#' &&
         grid[pos.second + 1][pos.first + 1] != '#';
}

bool can_move_west(pair<int, int> pos) {
  return grid[pos.second - 1][pos.first - 1] != '#' &&
         grid[pos.second][pos.first - 1] != '#' &&
         grid[pos.second + 1][pos.first - 1] != '#';
}

bool can_move_east(pair<int, int> pos) {
  return grid[pos.second - 1][pos.first + 1] != '#' &&
         grid[pos.second][pos.first + 1] != '#' &&
         grid[pos.second + 1][pos.first + 1] != '#';
}

bool cycle(int cycleI) {
  vector<pair<int, int>> nextelves;
  nextelves.reserve(elves.size());
  vector<bool> emoved(elves.size(), false);

  array<pair<decltype(&can_move_north), pair<int, int>>, 4> moves{
      make_pair(can_move_north, make_pair(0, -1)),
      make_pair(can_move_south, make_pair(0, 1)),
      make_pair(can_move_west, make_pair(-1, 0)),
      make_pair(can_move_east, make_pair(1, 0))
  };

  set<int> reset;

  for (auto& e : elves) {
    bool moved = false;
    if (should_move(e)) {
      for (int i = 0; i < 4; ++i) {
        auto& [functor, dir] = moves[(i + cycleI) % 4];
        if (functor(e)) {
          auto dest = make_pair(e.first + dir.first, e.second + dir.second);
          for (int o = 0; o < nextelves.size();++o) {
            if (nextelves[o] == dest) {
              reset.insert(o);
              reset.insert(nextelves.size());
            }
          }
          emoved[nextelves.size()] = true;
          nextelves.push_back(dest);
          moved = true;
          break;
        }
      }
    } 
    if (!moved) {
      nextelves.push_back(e);
    }
  }

  for (auto& i : reset) {
    nextelves[i] = elves[i];
    emoved[i] = false;
  }

  for (auto& e : elves) {
    grid[e.second][e.first] = '.';
  }
  nextelves.swap(elves);
  for (auto& e : elves) {
    grid[e.second][e.first] = '#';
  }

  auto [x1, y1, x2, y2] = bounds();
  if (min(x1, y1) == 0 || x2 == grid[0].length() - 1 || y2 == grid.size() - 1)
    grow();

  return any_of(emoved.begin(), emoved.end(), [](auto x) { return x; });
  //for (auto& s : grid)
  //  cout << s << endl;
  //cout << endl << endl;
}



void first() {
  parse();

  for (int i = 0; i < 10; ++i)
    cycle(i);

  auto [x1, y1, x2, y2] = bounds();

  cout << ((1 + x2 - x1) * (1 + y2 - y1) - (int)elves.size()) << endl;
}
void second() {
  parse();

  int i;
  for (i = 0; cycle(i); ++i)
    ;

  cout << (i + 1) << endl;
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