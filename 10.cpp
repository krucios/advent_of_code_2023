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

using namespace std;

struct Coord {
    int x;
    int y;
};

int main() {
    ifstream fin("inputs/10.in");

    string line;

    int ans_1 = 0;
    int ans_2 = 0;

    vector<string> map;
    vector<vector<int>> dist;

    Coord start;

    while (getline(fin, line)) {
        map.push_back(line);
        dist.push_back({});
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == 'S') {
                start.x = map.size() - 1;
                start.y = i;
            }
            dist.back().push_back(line[i] == 'S' ? 0 : INT32_MAX);
        }
    }

    queue<Coord> bfs;
    bfs.push(start);

    auto check_and_add = [&](Coord c, Coord n) {
        if (dist[n.x][n.y] > (dist[c.x][c.y] + 1)) {
            dist[n.x][n.y] = dist[c.x][c.y] + 1;
            bfs.push(n);
        }
    };
    auto check_and_add_top = [&](Coord c) {
        Coord n = c;

        n.x = c.x - 1;
        if (c.x > 0) {
            check_and_add(c, n);
        }
    };
    auto check_and_add_bot = [&](Coord c) {
        Coord n = c;

        n.x = c.x + 1;
        if (c.x < (map.size() - 1)) {
            check_and_add(c, n);
        }
    };
    auto check_and_add_left = [&](Coord c) {
        Coord n = c;

        n.y = c.y - 1;
        if (c.y > 0) {
            check_and_add(c, n);
        }
    };
    auto check_and_add_right = [&](Coord c) {
        Coord n = c;

        n.y = c.y + 1;
        if (c.y < (map[c.x].size() - 1)) {
            check_and_add(c, n);
        }
    };

    while (bfs.size()) {
        Coord c = bfs.front();
        bfs.pop();

        Coord n = c;

        switch (map[c.x][c.y]) {
            case '|':
                check_and_add_top(c);
                check_and_add_bot(c);
                break;
            case '-':
                check_and_add_left(c);
                check_and_add_right(c);
                break;
            case 'L':
                check_and_add_top(c);
                check_and_add_right(c);
                break;
            case 'J':
                check_and_add_top(c);
                check_and_add_left(c);
                break;
            case '7':
                check_and_add_bot(c);
                check_and_add_left(c);
                break;
            case 'F':
                check_and_add_bot(c);
                check_and_add_right(c);
                break;
            case 'S':
                n.x = c.x - 1;
                if (c.x > 0) {
                    char t = map[n.x][n.y];
                    if (t == '|' || t == '7' || t == 'F') {
                        check_and_add_top(c);
                    }
                }

                n.x = c.x + 1;
                if (c.x < (map.size() - 1)) {
                    char t = map[n.x][n.y];
                    if (t == '|' || t == 'J' || t == 'L') {
                        check_and_add_bot(c);
                    }
                }

                n.x = c.x;
                n.y = c.y - 1;
                if (c.y > 0) {
                    char t = map[n.x][n.y];
                    if (t == '-' || t == 'F' || t == 'L') {
                        check_and_add_left(c);
                    }
                }

                n.y = c.y + 1;
                if (c.y < (map[c.x].size() - 1)) {
                    char t = map[n.x][n.y];
                    if (t == '-' || t == '7' || t == 'J') {
                        check_and_add_right(c);
                    }
                }
                break;

            default: assert(false);
        }
    }

    ans_1 = dist[start.x][start.y];
    for (int i = 0; i < dist.size(); i++) {
        for (int j = 0; j < dist[i].size(); j++) {
            if (dist[i][j] != INT32_MAX) {
                ans_1 = max(ans_1, dist[i][j]);
            }
        }
    }

    cout << ans_1 << endl;

    bool top_valid;
    bool bot_valid;
    bool left_valid;
    bool right_valid;

    if (start.x > 0) {
        top_valid = (map[start.x - 1][start.y] == '|')
                 || (map[start.x - 1][start.y] == 'F')
                 || (map[start.x - 1][start.y] == '7');
    }
    if (start.x < map.size() - 2) {
        bot_valid = (map[start.x + 1][start.y] == '|')
                 || (map[start.x + 1][start.y] == 'L')
                 || (map[start.x + 1][start.y] == 'J');
    }
    if (start.y > 0) {
        left_valid = (map[start.x][start.y - 1] == '-')
                  || (map[start.x][start.y - 1] == 'F')
                  || (map[start.x][start.y - 1] == 'L');
    }
    if (start.y < map[start.x].length() - 2) {
        right_valid = (map[start.x][start.y + 1] == '-')
                   || (map[start.x][start.y + 1] == 'J')
                   || (map[start.x][start.y + 1] == '7');
    }

    if (top_valid && bot_valid) {
        map[start.x][start.y] = '|';
    }
    if (top_valid && left_valid) {
        map[start.x][start.y] = 'J';
    }
    if (top_valid && right_valid) {
        map[start.x][start.y] = 'L';
    }
    if (bot_valid && left_valid) {
        map[start.x][start.y] = '7';
    }
    if (bot_valid && right_valid) {
        map[start.x][start.y] = 'F';
    }
    if (left_valid && right_valid) {
        map[start.x][start.y] = '-';
    }

    char prev_corner;
    for (int i = 0; i < map.size(); i++) {
        int border_crossed = 0;
        for (int j = 0; j < map[i].length(); j++) {
            if (dist[i][j] == INT32_MAX) {
                ans_2 += border_crossed % 2;
            } else {
                if (map[i][j] == '|') {
                    border_crossed++;
                } else if (map[i][j] == 'F' || map[i][j] == 'L') {
                    prev_corner = map[i][j];
                } else if (map[i][j] == 'J') {
                    assert(prev_corner == 'F' || prev_corner == 'L');
                    border_crossed += prev_corner == 'F' ? 1 : 2;
                } else if (map[i][j] == '7') {
                    assert(prev_corner == 'F' || prev_corner == 'L');
                    border_crossed += prev_corner == 'L' ? 1 : 2;
                }
            }
        }
    }
    cout << ans_2 << endl;

    return 0;
}