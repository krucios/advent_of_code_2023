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

static constexpr int UP = 0;
static constexpr int DOWN = 1;
static constexpr int LEFT = 2;
static constexpr int RIGHT = 3;

struct Coord {
    int x;
    int y;
    int dir;
    int steps;
    int cur_dist;
};

int opposite_dir(int dir) {
    switch (dir) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
        default:
            assert(false);
    }
    return 0;
}

int left_dir(int d) {
    switch (d) {
        case UP: return LEFT;
        case DOWN: return RIGHT;
        case LEFT: return DOWN;
        case RIGHT: return UP;
        default: assert(false);
    }
    return 0;
}

int right_dir(int d) {
    switch (d) {
        case UP: return RIGHT;
        case DOWN: return LEFT;
        case LEFT: return UP;
        case RIGHT: return DOWN;
        default: assert(false);
    }
    return 0;
}

bool is_in_map(const Coord& c, const vector<vector<int>>& m) {
    return (0 <= c.x) && (c.x < m.size()) && (0 <= c.y) && (c.y < m[0].size());
}

Coord next(const Coord& c, size_t dir, const vector<vector<int>> m) {
    Coord n;
    n.steps = dir == c.dir ? c.steps + 1 : 1;
    n.dir = dir;
    n.x = c.x;
    n.y = c.y;

    switch (dir) {
        case UP:
            n.x--;
            break;

        case DOWN:
            n.x++;
            break;

        case LEFT:
            n.y--;
            break;

        case RIGHT:
            n.y++;
            break;

        default:
            assert(false);
    }

    if (is_in_map(n, m)) {
        n.cur_dist = c.cur_dist + m[n.x][n.y];
    }
    return n;
}

typedef vector<vector<array<array<int, 4>, 3>>> dist_map_1_t;
typedef vector<vector<array<array<int, 4>, 10>>> dist_map_2_t;

int solve_part_1(const vector<vector<int>>& m) {
    int ans = INT32_MAX;
    dist_map_1_t d;

    for (int i = 0; i < m.size(); i++) {
        d.push_back({});
        for (int j = 0; j < m[0].size(); j++) {
            d.back().push_back({});
            for (int step = 0; step < 3; step++) {
                for (int dir = 0; dir < 4; dir++) {
                    d.back().back()[step][dir] = INT32_MAX;
                }
            }
        }
    }

    Coord start;
    start.x = 0;
    start.y = 0;
    start.dir = RIGHT;
    start.steps = 1;
    start.cur_dist = 0;

    deque<Coord> bfs;
    bfs.push_back(start);
    while (!bfs.empty()) {
        Coord c = bfs.front();
        bfs.pop_front();

        if (!is_in_map(c, m)) {
            continue;
        }

        if (d[c.x][c.y][c.steps - 1][c.dir] <= c.cur_dist) {
            continue;
        }
        d[c.x][c.y][c.steps - 1][c.dir] = c.cur_dist;

        for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
            if ((dir != opposite_dir(c.dir))
             && (c.steps < 3 || dir != c.dir)) {
                Coord n = next(c, dir, m);
                bfs.push_back(n);
                erase_if(bfs, [&](const auto& e) {
                    return e.x == n.x && e.y == n.y && e.dir == n.dir && e.steps >= n.steps && e.cur_dist > n.cur_dist;
                });
            }
        }
    }

    for (const auto& r : d.back().back()) {
        for (const auto& c : r) {
            ans = min(ans, c);
        }
    }

    return ans;
}

int solve_part_2(const vector<vector<int>>& m) {
    int ans = INT32_MAX;
    dist_map_2_t d;

    for (int i = 0; i < m.size(); i++) {
        d.push_back({});
        for (int j = 0; j < m[0].size(); j++) {
            d.back().push_back({});
            for (int step = 0; step < 10; step++) {
                for (int dir = 0; dir < 4; dir++) {
                    d.back().back()[step][dir] = INT32_MAX;
                }
            }
        }
    }

    Coord start;
    start.x = 0;
    start.y = 0;
    start.dir = RIGHT;
    start.steps = 1;
    start.cur_dist = 0;

    deque<Coord> bfs;
    bfs.push_back(start);
    while (!bfs.empty()) {
        auto it = min_element(bfs.begin(), bfs.end(), [](const auto& a, const auto& b) { return a.cur_dist < b.cur_dist; });
        Coord c = *it;
        bfs.erase(it);

        if ((c.x == (m.size() - 1)) && (c.y == (m[0].size() - 1))) {
            return c.cur_dist;
        }

        d[c.x][c.y][c.steps - 1][c.dir] = c.cur_dist;
        for (auto dir : {UP, DOWN, LEFT, RIGHT}) {
            if ((dir != opposite_dir(c.dir))
             && (c.steps < 10 || dir != c.dir)
             && (c.steps >= 4 || dir != left_dir(c.dir))
             && (c.steps >= 4 || dir != right_dir(c.dir))) {
                Coord n = next(c, dir, m);

                if (c.steps == 1) {
                    for (int i = 0; i < 2; i++) {
                        n = next(n, dir, m);
                    }
                }

                if (is_in_map(n, m) && d[n.x][n.y][n.steps - 1][n.dir] > n.cur_dist) {
                    bfs.push_back(n);
                }
            }
        }
    }

    for (int step = 3; step < 10; step++) {
        for (const auto& c : d.back().back()[step]) {
            ans = min(ans, c);
        }
    }

    return ans;
}

int main() {
    ifstream fin("inputs/17.in");

    string line;

    int ans_1 = 0;
    size_t ans_2 = 0;

    vector<vector<int>> m;

    while (getline(fin, line)) {
        m.push_back({});
        for (auto c : line) {
            m.back().push_back(c - '0');
        }
    }

    // ans_1 = solve_part_1(m);
    cout << ans_1 << endl;

    ans_2 = solve_part_2(m);
    cout << ans_2 << endl;

    return 0;
}
