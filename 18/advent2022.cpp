#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

void first() {
  map<int, map<int, set<int>>> tiles;
  int x, y, z;
  char c;
  while (_cin >> x >> c >> y >> c >> z) {
    tiles[x][y].insert(z);
  }

  int64_t out = 0;
  for (auto& a : tiles) {
    x = a.first;
    for (auto& b : a.second) {
      y = b.first;
      for (auto& z : b.second) {
        out += 6;
        out -= tiles[x][y].count(z - 1);
        out -= tiles[x][y].count(z + 1);
        out -= tiles[x][y + 1].count(z);
        out -= tiles[x][y - 1].count(z);
        out -= tiles[x + 1][y].count(z);
        out -= tiles[x - 1][y].count(z);
      }
    }
  }

  cout << out << endl;
}

const int CONTAINED = 0;
const int TILE = 1;
const int OUTSIDE = 2;

void flag(vector<vector<vector<int>>>& grid, set<tuple<int, int, int>>& todo, int x, int y, int z) {
  if (x < 0 || x >= grid.size() || y < 0 || y >= grid[x].size() || z < 0 ||
      z >= grid[x][y].size())
    return;

  if (grid[x][y][z] == CONTAINED) {
    grid[x][y][z] = OUTSIDE;
    todo.insert(make_tuple(x, y, z));
  }
}

void mark_outside(vector<vector<vector<int>>>& grid) {

  set<tuple<int, int, int>> todo;

  flag(grid, todo, 0, 0, 0);
  while (!todo.empty()) {
    auto iter = todo.begin();
    auto [x, y, z] = *iter;
    todo.erase(iter);
    flag(grid, todo, x, y, z + 1);
    flag(grid, todo, x, y, z - 1);
    flag(grid, todo, x, y - 1, z);
    flag(grid, todo, x, y + 1, z);
    flag(grid, todo, x + 1, y, z);
    flag(grid, todo, x - 1, y, z);
  }
}

int64_t countedges(vector<vector<vector<int>>>& grid) {
  auto isnt_outside = [&](int px, int py, int pz) {
    if (px < 0 || py < 0 || pz < 0 || px >= grid.size() ||
        py >= grid[px].size() || pz >= grid[px][py].size())
      return 0;
    return (grid[px][py][pz] != OUTSIDE ? 1 : 0);
  };

  int64_t out = 0;
  for (int x = 0; x < grid.size(); ++x) {
    for (int y = 0; y < grid[x].size(); ++y) {
      for (int z = 0; z < grid[x][y].size(); ++z) {
        if (grid[x][y][z] == TILE) {
          out += 6;
          out -= isnt_outside(x - 1, y, z);
          out -= isnt_outside(x + 1, y, z);
          out -= isnt_outside(x, y - 1, z);
          out -= isnt_outside(x, y + 1, z);
          out -= isnt_outside(x, y, z - 1);
          out -= isnt_outside(x, y, z + 1);
        }
      }
    }
  }
  return out;
}

void second() {
  int x, y, z;
  int mx, my, mz;
  int Mx, My, Mz;
  char c;
  vector<tuple<int, int, int>> input;
  while (_cin >> x >> c >> y >> c >> z)
    input.push_back(make_tuple(x, y, z));

  {
    auto& [tx, ty, tz] = input[0];
    mx = Mx = tx;
    my = My = ty;
    mz = Mz = tz;
  }

  for (auto& [tx, ty, tz] : input) {
    mx = min(mx, tx);
    my = min(my, ty);
    mz = min(mz, tz);
    Mx = max(Mx, tx);
    My = max(My, ty);
    Mz = max(Mz, tz);
  }

  vector<vector<vector<int>>> state(
      Mx - mx + 2,
      vector<vector<int>>(My - my + 2, vector<int>(Mz - mz + 2, CONTAINED)));

    for (auto& [tx, ty, tz] : input) {
      state[tx - mx + 1][ty - my + 1][tz - mz + 1] = 1;
    }

    mark_outside(state);

    cout << countedges(state) << endl;
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