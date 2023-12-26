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

static constexpr size_t UP = 1;
static constexpr size_t DOWN = 2;
static constexpr size_t LEFT = 4;
static constexpr size_t RIGHT = 8;

struct Coord {
    int x;
    int y;
    size_t dir;
};

bool is_in_map(const Coord& c, const vector<string>& m) {
    return (0 <= c.x) && (c.x < m.size()) && (0 <= c.y) && (c.y < m[0].length());
}

Coord next(const Coord& c, size_t dir) {
    Coord n;
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

    return n;
}

size_t energized_count(const Coord& start, const vector<string> m) {
    size_t energized_count = 0;

    vector<vector<size_t>> e;
    for (int i = 0; i < m.size(); i++) {
        e.push_back({});
        for (int j = 0; j < m[0].length(); j++) {
            e.back().push_back(0);
        }
    }

    queue<Coord> beams;
    beams.push(start);
    while (!beams.empty()) {
        Coord b = beams.front();
        beams.pop();

        // If we have been in this place with the same direction
        // then there is a loop which we already processed
        if (e[b.x][b.y] & b.dir) {
            continue;
        }
        e[b.x][b.y] |= b.dir;

        Coord n;
        char c = m[b.x][b.y];
        if (c == '.') {
            n = next(b, b.dir);
            if (is_in_map(n, m)) {
                beams.push(n);
            }
        } else if (c == '\\') {
            size_t new_dir = (b.dir == UP  ) ? LEFT
                           : (b.dir == DOWN) ? RIGHT
                           : (b.dir == LEFT) ? UP
                           : DOWN;

            n = next(b, new_dir);
            if (is_in_map(n, m)) {
                beams.push(n);
            }
        } else if (c == '/') {
            size_t new_dir = (b.dir == UP  ) ? RIGHT
                           : (b.dir == DOWN) ? LEFT
                           : (b.dir == LEFT) ? DOWN
                           : UP;

            n = next(b, new_dir);
            if (is_in_map(n, m)) {
                beams.push(n);
            }
        } else if (c == '|') {
            if (b.dir == UP || b.dir == DOWN) {
                n = next(b, b.dir);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
            } else {
                n = next(b, UP);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
                n = next(b, DOWN);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
            }
        } else if (c == '-') {
            if (b.dir == LEFT || b.dir == RIGHT) {
                n = next(b, b.dir);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
            } else {
                n = next(b, LEFT);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
                n = next(b, RIGHT);
                if (is_in_map(n, m)) {
                    beams.push(n);
                }
            }
        } else {
            assert(false);
        }
    }

    for (const auto& r : e) {
        for (const auto& c : r) {
            energized_count += c != 0;
        }
    }

    return energized_count;
}

int main() {
    ifstream fin("inputs/16.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    vector<string> m;

    while (getline(fin, line)) {
        m.push_back(line);
    }

    Coord start;
    start.x = 0;
    start.y = 0;
    start.dir = RIGHT;
    ans_1 = energized_count(start, m);

    for (int i = 0; i < m.size(); i++) {
        start.x = i;
        start.y = 0;
        start.dir = RIGHT;

        ans_2 = max(energized_count(start, m), ans_2);

        start.y = m[0].length() - 1;
        start.dir = LEFT;
        ans_2 = max(energized_count(start, m), ans_2);
    }

    for (int i = 0; i < m[0].length(); i++) {
        start.x = 0;
        start.y = i;
        start.dir = DOWN;

        ans_2 = max(energized_count(start, m), ans_2);

        start.x = m.size() - 1;
        start.dir = UP;
        ans_2 = max(energized_count(start, m), ans_2);
    }


    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
