#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

using Blizzard = tuple<int, int, int>;

int start_index;
int end_index;

vector<vector<bool>> grid;
vector<Blizzard> blizzards;

void step() {
  vector<vector<bool>> nextgrid(grid.size(),
                                vector<bool>(grid[0].size(), false));

  nextgrid[0][start_index] = grid[0][start_index] || grid[1][start_index];
  for (int y = 1; y < grid.size() - 1; ++y) {
    for (int x = 1; x < grid[y].size() - 1; ++x) {
      nextgrid[y][x] = grid[y][x] || grid[y - 1][x] || grid[y + 1][x] ||
                       grid[y][x + 1] || grid[y][x - 1];
    }
  }
  nextgrid[grid.size() - 1][end_index] = grid[grid.size() - 2][end_index] || grid[grid.size() - 1][end_index];

  for (auto& [x, y, type] : blizzards) {
    switch (type) {
    case 0:// east
        if (++x == grid[y].size() - 1)
          x = 1;
      break;
    case 1: // south
      if (++y == grid.size() - 1)
        y = 1; // ? end_index
      break;
    case 2: // west
      if (--x == 0)
        x = grid[y].size() - 2;
      break;
    case 3:
      if (--y == 0)
        y = grid.size() - 2;
      break;
    }
    nextgrid[y][x] = false;
  }

  grid.swap(nextgrid);
}

void print_grid() {
  vector<string> cgrid(grid.size(), string(grid[0].size(), '#'));
  for (int y = 0; y < grid.size(); ++y)
    for (int x = 0; x < grid[y].size(); ++x)
      if (grid[y][x])
        cgrid[y][x] = 'E';

  for (auto& [x, y, z] : blizzards)
    cgrid[y][x] = 'B';

  for (auto& s : cgrid)
    cout << s << endl;
  cout << endl;
}

const string blizzstr = ">v<^";

void first() {
  string s;
  getline(_cin, s);

  start_index = s.find('.');
  grid.emplace_back(s.length(), false);
  grid.back()[start_index] = true;

  while (getline(_cin, s)) {
    for (int i = 1; i < s.length(); ++i) {
      if (auto bt = blizzstr.find(s[i]); bt != string::npos) {
          blizzards.push_back(make_tuple(i, (int)grid.size(), (int)bt));
        }
    }
    grid.emplace_back(s.length(), false);
  }
  end_index = s.find('.');

  int cycle = 0;

  while (!grid[grid.size() - 1][end_index]) {
    step();
    ++cycle;
    //print_grid();
  }

  cout << cycle << endl;
}
void second() {
  string s;
  getline(_cin, s);

  start_index = s.find('.');
  grid.emplace_back(s.length(), false);
  grid.back()[start_index] = true;

  while (getline(_cin, s)) {
    for (int i = 1; i < s.length(); ++i) {
      if (auto bt = blizzstr.find(s[i]); bt != string::npos) {
        blizzards.push_back(make_tuple(i, (int)grid.size(), (int)bt));
      }
    }
    grid.emplace_back(s.length(), false);
  }
  end_index = s.find('.');

  int cycle = 0;

  while (!grid[grid.size() - 1][end_index]) {
    step();
    ++cycle;
    // print_grid();
  }
  for (auto& row : grid)
    for (int i = 0; i < row.size(); ++i)
      row[i] = false;

  grid[grid.size() - 1][end_index] = true;
  while (!grid[0][start_index]) {
    step();
    ++cycle;
  }

  for (auto& row : grid)
    for (int i = 0; i < row.size(); ++i)
      row[i] = false;
  grid[0][start_index] = true;
  while (!grid[grid.size() - 1][end_index]) {
    step();
    ++cycle;
    // print_grid();
  }

  cout << cycle << endl;
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