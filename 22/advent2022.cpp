#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

struct point {
  int x, y;
};

point operator+(point a, point b) {
  return {a.x + b.x, a.y + b.y};
}

ostream& operator<<(ostream& out, point p) {
  return out << p.x << ',' << p.y;
}

array<point, 4> step{
  point{1, 0},
  point{0, 1},
  point{-1, 0},
  point{0, -1}
};

vector<string> cmap;
string legend;


void parse() {
  int x = 0;
  while (getline(_cin, legend) && legend != "") {
    cmap.push_back(legend);
    x = max(x, (int)legend.length());
  }

  for (auto& s : cmap) {
    while (s.length() < x)
      s = s + ' ';
  }
  getline(_cin, legend);
}

point next(point location, point dir) {
  do{
    location = location + dir;
    if (location.y == -1)
      location.y += cmap.size();
    if (location.y == cmap.size())
      location.y = 0;
    if (location.x == -1)
      location.x += cmap[location.y].length();
    if (location.x == cmap[location.y].length())
      location.x = 0;
  } while (cmap[location.y][location.x] == ' '); 
  return location;
}


void first() {
  parse();
  int dir = 0;
  auto start = next(point{(int)cmap[0].length() - 1, 0}, step[dir]);

  int dist;
  char rot;

  stringstream ss(legend);
  while (ss >> dist) {
    for (int i = 0; i < dist; ++i) {
      auto n = next(start, step[dir]);
      if (cmap[n.y][n.x] != '#')
        start = n;
    }
    //cout << start << endl;
    if (ss >> rot)
      dir = (dir + ((rot == 'R') ? 1 : 3)) % 4;
  }

  cout << (1000 * (1 + start.y)) + (4 * (1 + start.x)) + dir << endl;
}

struct FaceTile {
  bool walkable;
  int gx, gy;
};

struct Face {
  vector<vector<FaceTile>> tiles_;
  struct Boundary {
    int next;
    bool flipx;
    bool flipy;
    bool swap;
    int o;
  };
  array<Boundary, 4> edges;
};
array<Face, 6> cube;

typedef tuple<int, int, int, int> cubeloc;
int dim;

cubeloc next(cubeloc current) {
  auto& [c, x, y, o] = current;
  x += step[o].x;
  y += step[o].y;
  if (min(x, y) < 0 || max(x, y) == dim) {
    x = max(0, min(dim - 1, x));
    y = max(0, min(dim - 1, y));
    auto& bound = cube[c].edges[o];
    if (bound.flipx)
      x = dim - 1 - x;
    if (bound.flipy)
      y = dim - 1 - y;
    if (bound.swap)
      swap(x, y);
    c = bound.next;
    o = bound.o;
  }
  return current;
}

void buildSampleCube() {
  dim = 4;
  for (auto& f : cube) {
    f.tiles_.resize(dim);
    for (auto& row : f.tiles_) {
      row.resize(dim);
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y]);
    char c;
    for (int x = 0; x < dim; ++x) {
      ss >> c;
      cube[0].tiles_[y][x].walkable = (c == '.');
      cube[0].tiles_[y][x].gx = (2 * dim) + x + 1;
      cube[0].tiles_[y][x].gy = y + 1;
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y + dim]);
    char c;
    for (int z = 0; z < 3; ++z) {
      for (int x = 0; x < dim; ++x) {
        ss >> c;
        cube[z + 1].tiles_[y][x].walkable = (c == '.');
        cube[z + 1].tiles_[y][x].gx = (z * dim) + x + 1;
        cube[z + 1].tiles_[y][x].gy = dim + y + 1;
      }
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y + dim + dim]);
    char c;
    for (int z = 0; z < 2; ++z) {
      for (int x = 0; x < dim; ++x) {
        ss >> c;
        cube[z + 4].tiles_[y][x].walkable = (c == '.');
        cube[z + 4].tiles_[y][x].gx = ((z + 2) * dim) + x + 1;
        cube[z + 4].tiles_[y][x].gy = dim + dim + y + 1;
      }
    }
  }

  const auto KEEP_X = false;
  const auto FLIP_X = true;
  const auto KEEP_Y = false;
  const auto FLIP_Y = true;
  const auto SWAP = true;
  const auto NO_SWAP = false;

  const auto RIGHT = 0;
  const auto DOWN = 1;
  const auto LEFT = 2;
  const auto UP = 3;

  //     0
  // 1 2 3
  //     4 5
  cube[0].edges[RIGHT] = {5, KEEP_X, FLIP_Y, NO_SWAP, LEFT};
  cube[0].edges[DOWN] = {3, KEEP_X, FLIP_Y, NO_SWAP, DOWN};
  cube[0].edges[LEFT] = {2, KEEP_X, KEEP_Y, SWAP, DOWN};
  cube[0].edges[UP] = {1, FLIP_X, KEEP_Y, NO_SWAP, DOWN};

  cube[1].edges[RIGHT] = {2, FLIP_X, KEEP_Y, NO_SWAP, RIGHT};
  cube[1].edges[DOWN] = {4, FLIP_X, KEEP_Y, NO_SWAP, UP};
  cube[1].edges[LEFT] = {5, FLIP_X, FLIP_Y, SWAP, UP};
  cube[1].edges[UP] = {0, FLIP_X, KEEP_Y, NO_SWAP, DOWN};

  cube[2].edges[RIGHT] = {3, KEEP_X, KEEP_Y, NO_SWAP, RIGHT};
  cube[2].edges[DOWN] = {4, FLIP_X, FLIP_Y, SWAP, RIGHT};
  cube[2].edges[LEFT] = {1, KEEP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[2].edges[UP] = {0, KEEP_X, KEEP_Y, SWAP, RIGHT};

  cube[3].edges[RIGHT] = {5, FLIP_X, FLIP_Y, SWAP, DOWN};
  cube[3].edges[DOWN] = {4, KEEP_X, KEEP_Y, NO_SWAP, DOWN};
  cube[3].edges[LEFT] = {2, KEEP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[3].edges[UP] = {0, KEEP_X, KEEP_Y, NO_SWAP, UP};

  cube[4].edges[RIGHT] = {5, KEEP_X, KEEP_Y, NO_SWAP, RIGHT};
  cube[4].edges[DOWN] = {1, FLIP_X, KEEP_Y, NO_SWAP, UP};
  cube[4].edges[LEFT] = {2, KEEP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[4].edges[UP] = {3, FLIP_X, KEEP_Y, NO_SWAP, UP};

  cube[5].edges[RIGHT] = {0, KEEP_X, FLIP_Y, NO_SWAP, LEFT};
  cube[5].edges[DOWN] = {1, FLIP_X, FLIP_Y, SWAP, RIGHT};
  cube[5].edges[LEFT] = {4, FLIP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[5].edges[UP] = {3, FLIP_X, FLIP_Y, SWAP, LEFT};

}

void buildInputCube() {
  dim = 50;
  for (auto& f : cube) {
    f.tiles_.resize(dim);
    for (auto& row : f.tiles_) {
      row.resize(dim);
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y]);
    char c;
    for (int x = 0; x < dim; ++x) {
      ss >> c;
      cube[0].tiles_[y][x].walkable = (c == '.');
      cube[0].tiles_[y][x].gx = (dim) + x + 1;
      cube[0].tiles_[y][x].gy = y + 1;
    }
    for (int x = 0; x < dim; ++x) {
      ss >> c;
      cube[1].tiles_[y][x].walkable = (c == '.');
      cube[1].tiles_[y][x].gx = (2 * dim) + x + 1;
      cube[1].tiles_[y][x].gy = y + 1;
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y + dim]);
    char c;
    for (int x = 0; x < dim; ++x) {
      ss >> c;
      cube[2].tiles_[y][x].walkable = (c == '.');
      cube[2].tiles_[y][x].gx = dim + x + 1;
      cube[2].tiles_[y][x].gy = dim + y + 1;
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y + dim + dim]);
    char c;
    for (int z = 0; z < 2; ++z) {
      for (int x = 0; x < dim; ++x) {
        ss >> c;
        cube[z + 3].tiles_[y][x].walkable = (c == '.');
        cube[z + 3].tiles_[y][x].gx = (z * dim) + x + 1;
        cube[z + 3].tiles_[y][x].gy = dim + dim + y + 1;
      }
    }
  }
  for (int y = 0; y < dim; ++y) {
    stringstream ss(cmap[y + dim + dim + dim]);
    char c;
    for (int x = 0; x < dim; ++x) {
      ss >> c;
      cube[5].tiles_[y][x].walkable = (c == '.');
      cube[5].tiles_[y][x].gx = x + 1;
      cube[5].tiles_[y][x].gy = dim + dim + dim + y + 1;
    }
  }

  const auto KEEP_X = false;
  const auto FLIP_X = true;
  const auto KEEP_Y = false;
  const auto FLIP_Y = true;
  const auto SWAP = true;
  const auto NO_SWAP = false;

  const auto RIGHT = 0;
  const auto DOWN = 1;
  const auto LEFT = 2;
  const auto UP = 3;

  //  01
  //  2
  // 34
  // 5
  cube[0].edges[RIGHT] = {1, FLIP_X, KEEP_Y, NO_SWAP, RIGHT};
  cube[0].edges[DOWN] = {2, KEEP_X, FLIP_Y, NO_SWAP, DOWN};
  cube[0].edges[LEFT] = {3, KEEP_X, FLIP_Y, NO_SWAP, RIGHT};
  cube[0].edges[UP] = {5, KEEP_X, KEEP_Y, SWAP, RIGHT};

  cube[1].edges[RIGHT] = {4, KEEP_X, FLIP_Y, NO_SWAP, LEFT};
  cube[1].edges[DOWN] = {2, KEEP_X, KEEP_Y, SWAP, LEFT};
  cube[1].edges[LEFT] = {0, FLIP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[1].edges[UP] = {5, KEEP_X, FLIP_Y, NO_SWAP, UP};

  cube[2].edges[RIGHT] = {1, KEEP_X, KEEP_Y, SWAP, UP};
  cube[2].edges[DOWN] = {4, KEEP_X, FLIP_Y, NO_SWAP, DOWN};
  cube[2].edges[LEFT] = {3, KEEP_X, KEEP_Y, SWAP, DOWN};
  cube[2].edges[UP] = {0, KEEP_X, FLIP_Y, NO_SWAP, UP};

  cube[3].edges[RIGHT] = {4, FLIP_X, KEEP_Y, NO_SWAP, RIGHT};
  cube[3].edges[DOWN] = {5, KEEP_X, FLIP_Y, NO_SWAP, DOWN};
  cube[3].edges[LEFT] = {0, KEEP_X, FLIP_Y, NO_SWAP, RIGHT};
  cube[3].edges[UP] = {2, KEEP_X, KEEP_Y, SWAP, RIGHT};

  cube[4].edges[RIGHT] = {1, KEEP_X, FLIP_Y, NO_SWAP, LEFT};
  cube[4].edges[DOWN] = {5, KEEP_X, KEEP_Y, SWAP, LEFT};
  cube[4].edges[LEFT] = {3, FLIP_X, KEEP_Y, NO_SWAP, LEFT};
  cube[4].edges[UP] = {2, KEEP_X, FLIP_Y, NO_SWAP, UP};

  cube[5].edges[RIGHT] = {4, KEEP_X, KEEP_Y, SWAP, UP};
  cube[5].edges[DOWN] = {1, KEEP_X, FLIP_Y, NO_SWAP, DOWN};
  cube[5].edges[LEFT] = {0, KEEP_X, KEEP_Y, SWAP, DOWN};
  cube[5].edges[UP] = {3, KEEP_X, FLIP_Y, NO_SWAP, UP};
}

void second() {
  parse();
  //buildSampleCube();
  buildInputCube();

  /*for (auto& f : cube) {
    for (int y = 0; y < dim; ++y) {
      for (int x = 0; x < dim; ++x) {
        cout << (f.tiles_[y][x].walkable ? '.' : '#');
      }
      cout << endl;
    }
    cout << endl;
  }*/

  //return;
  auto copy = cmap;
  stringstream ss(legend);
  cubeloc loc{0, 0, 0, 0};
  auto& [c, x, y, o] = loc;
  int dist;
  while (ss >> dist) {
    for (int i = 0; i < dist; ++i) {
      auto n = next(loc);
      auto& [nc, nx, ny, no] = n;
      if (cube[nc].tiles_[ny][nx].walkable)
        loc = n;
      //cmap[cube[c].tiles_[y][x].gy - 1][cube[c].tiles_[y][x].gx - 1] = ">v<^"[o];
    }
    /*for (auto& s : cmap)
      cout << s << endl;
    cmap = copy;*/

    if (char rot; ss >> rot)
      o = (o + ((rot == 'R') ? 1 : 3)) % 4;
  }

  /*for (auto& s : cmap)
    cout << s << endl;*/

  int out = o;
  out += cube[c].tiles_[y][x].gy * 1000;
  out += cube[c].tiles_[y][x].gx * 4;
  cout << out << endl;
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