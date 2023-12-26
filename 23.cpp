#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <utility>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <regex>
#include <map>
#include <set>
#include <bit>

using namespace std;

static constexpr array<int, 4> dx = {-1,  0,  1,  0};
static constexpr array<int, 4> dy = { 0,  1,  0, -1};

struct Coord {
    int x;
    int y;
};

typedef vector<vector<bool>> visited_map_t;
typedef vector<vector<int>> distance_map_t;

distance_map_t d_1;
distance_map_t d_2;

void dfs_1(Coord p, int dist, visited_map_t visited, const vector<string>& m) {
    d_1[p.x][p.y] = max(dist, d_1[p.x][p.y]);

    for (int i = 0; i < 4; i++) {
        Coord n;
        n.x = p.x + dx[i];
        n.y = p.y + dy[i];

        if (m[p.x][p.y] == '^' && i != 0) {
            continue;
        }

        if (m[p.x][p.y] == '>' && i != 1) {
            continue;
        }

        if (m[p.x][p.y] == 'v' && i != 2) {
            continue;
        }

        if (m[p.x][p.y] == '<' && i != 3) {
            continue;
        }

        if (n.x < 0 || n.x >= visited.size() || n.y < 0 || n.y >= visited[0].size()) {
            continue;
        }

        if (m[n.x][n.y] != '#' && !visited[n.x][n.y] && d_1[n.x][n.y] < (dist + 1)) {
            visited[n.x][n.y] = true;

            assert(abs(n.x - p.x) + abs(n.y - p.y) == 1);

            dfs_1(n, dist + 1, visited, m);

            visited[n.x][n.y] = false;
        }
    }
}

void dfs_2(Coord p, int dist, visited_map_t& visited, const vector<string>& m) {
    d_2[p.x][p.y] = max(dist, d_2[p.x][p.y]);

    for (int i = 0; i < 4; i++) {
        Coord n;
        n.x = p.x + dx[i];
        n.y = p.y + dy[i];

        if (n.x < 0 || n.x >= visited.size() || n.y < 0 || n.y >= visited[0].size()) {
            continue;
        }

        if (m[n.x][n.y] != '#' && !visited[n.x][n.y]) {
            visited[n.x][n.y] = true;
            dfs_2(n, dist + 1, visited, m);
            visited[n.x][n.y] = false;
        }
    }
}

int main() {
    ifstream fin("inputs/23.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    vector<string> m;
    visited_map_t visited;

    while (getline(fin, line)) {
        m.push_back(line);
        d_1.push_back({});
        d_2.push_back({});
        visited.push_back({});

        for (int i = 0; i < line.length(); i++) {
            d_1.back().push_back(0);
            d_2.back().push_back(0);
            visited.back().push_back(false);
        }
    }

    Coord start;
    start.x = 0;
    start.y = 1;
    visited[start.x][start.y] = true;

    dfs_1(start, 0, visited, m);
    ans_1 = d_1.back()[d_1[0].size() - 2];
    cout << ans_1 << endl;

    dfs_2(start, 0, visited, m);
    ans_2 = d_2.back()[d_2[0].size() - 2];
    cout << ans_2 << endl;

    return 0;
}
