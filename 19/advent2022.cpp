#include "all_helpers.hpp"

using namespace std;

ifstream _cin("sample.txt");


struct blueprint {
  int ore_robot_cost;
  int clay_robot_cost;
  pair<int, int> obs_robot_cost;
  pair<int, int> geode_robot_cost;
};


vector<pair<int, char>> bestpath;

void dfs(const blueprint& bp,
         int& best,
         int score,
         int ore,
         int clay,
         int obs,
         int time,
         int num_ore,
         int num_clay,
         int num_obs,
         vector<pair<int, char>>& path) {
  if (best > score + (time * time / 2))
    return; // todo tune for faster exits

  if (score > best) {
    best = score;
    //bestpath = path;
  }

  if (time <= 1)
    return;

  if (num_obs) {  // build a geode robot next
    int delay =
        max(0,
            max((bp.geode_robot_cost.second - obs + (num_obs - 1)) / num_obs,
                (bp.geode_robot_cost.first - ore + (num_ore - 1)) / num_ore)) +
        1;
    //path.push_back({time - delay, 'G'});
    if (delay < time)
      dfs(bp, best, score + time - delay,
          ore + (delay * num_ore) - bp.geode_robot_cost.first,
          clay + (delay * num_clay),
          obs + (delay * num_obs) - bp.geode_robot_cost.second, time - delay,
          num_ore, num_clay, num_obs, path);
    //path.pop_back();
  }

  if (num_clay) {  // build a obsidian robot next
    int delay =
        max(0,
            max((bp.obs_robot_cost.second - clay + (num_clay - 1)) / num_clay,
                (bp.obs_robot_cost.first - ore + (num_ore - 1)) / num_ore)) +
        1;
    //path.push_back({time - delay, 'N'});
    dfs(bp, best, score, ore + (delay * num_ore) - bp.obs_robot_cost.first,
          clay + (delay * num_clay) - bp.obs_robot_cost.second,
          obs + (delay * num_obs), time - delay, num_ore, num_clay, num_obs + 1,
          path);
    //path.pop_back();
  }

  { // build a ore robot next
    int delay = max(0, (bp.ore_robot_cost - ore + (num_ore - 1)) / num_ore) + 1;
    //path.push_back({time - delay, 'O'});
    dfs(bp, best, score, ore + (delay * num_ore) - bp.ore_robot_cost,
          clay + (delay * num_clay), obs + (delay * num_obs), time - delay,
          num_ore + 1, num_clay, num_obs, path);
    //path.pop_back();
  }

  { // build a clay robot next
    int delay =
        max(0, (bp.clay_robot_cost - ore + (num_ore - 1)) / num_ore) + 1;
    //path.push_back({time - delay, 'C'});
    dfs(bp, best, score, ore + (delay * num_ore) - bp.clay_robot_cost,
          clay + (delay * num_clay), obs + (delay * num_obs),
          time - delay, num_ore, num_clay + 1, num_obs,
          path);
    //path.pop_back();
  } 
}

int score(blueprint b, int n = 24) {
  int best = 0;
  vector<pair<int, char>> t;
  dfs(b, best, 0, 0, 0, 0, n, 1, 0, 0, t);
  /*cout << best << endl;
  for (auto& iter : bestpath)
    cout << 24 - iter.first << "-" << iter.second << ' ';
  cout << endl;*/
  return best;
}

bool read(blueprint& bp) {
  string line;
  if (!getline(_cin, line))
    return false;
  int bpid;
  auto count = sscanf_s(line.c_str(),
           "Blueprint %i: Each ore robot costs %i ore. Each clay robot costs %i "
           "ore. Each obsidian robot costs %i ore and %i clay. Each geode robot "
           "costs %i ore and %i obsidian.", &bpid, &bp.ore_robot_cost, &bp.clay_robot_cost, &bp.obs_robot_cost.first, &bp.obs_robot_cost.second, &bp.geode_robot_cost.first, &bp.geode_robot_cost.second);
  return count == 7;
}

void first() {
  int out = 0;
  blueprint bp;
  int index = 0;
  while (read(bp)) {
    out += ++index * score(bp);
    /* cout << bp.ore_robot_cost << ' ' << bp.clay_robot_cost << ' '
         << bp.obs_robot_cost.first << ',' << bp.obs_robot_cost.second << ' '
         << bp.geode_robot_cost.first << ',' << bp.geode_robot_cost.second
         << endl;*/
  }
  cout << out << endl;
}

void second() {
  int out = 1;
  blueprint bp;
  int index = 0;
  while (read(bp)) {
    out *= score(bp, 32);
    /* cout << bp.ore_robot_cost << ' ' << bp.clay_robot_cost << ' '
         << bp.obs_robot_cost.first << ',' << bp.obs_robot_cost.second << ' '
         << bp.geode_robot_cost.first << ',' << bp.geode_robot_cost.second
         << endl;*/
    if (++index == 3)
      break;
  }
  cout << out << endl;
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