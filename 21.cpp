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

struct Coord {
    int x;
    int y;
    long d;
};

static constexpr array<int, 4> dx = {-1, 0, 1, 0};
static constexpr array<int, 4> dy = {0, 1, 0, -1};

int distance(const Coord& a, const Coord& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int main(int argc, char* argv[]) {
    ifstream fin("inputs/21.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    vector<string> m;
    vector<vector<bool>> possible;
    vector<vector<int>> dist;

    Coord start;
    start.d = 0;

    while (getline(fin, line)) {
        m.push_back(line);
        possible.push_back({});
        dist.push_back({});

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == 'S') {
                start.x = m.size() - 1;
                start.y = i;
            }
            possible.back().push_back(false);
            dist.back().push_back(0);
        }
    }

    queue<Coord> bfs;

    bfs.push(start);

    int steps = stoi(argv[1]);

    possible[start.x][start.y] = true;
    while (!bfs.empty()) {
        Coord c = bfs.front();
        bfs.pop();

        if (c.d >= steps) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            Coord n;
            n.x = c.x + dx[i];
            n.y = c.y + dy[i];
            n.d = c.d + 1;

            if (m[n.x][n.y] == '.' && !possible[n.x][n.y]) {
                possible[n.x][n.y] = true;
                dist[n.x][n.y] = n.d;
                bfs.push(n);
            }
        }
    }

    for (int i = 0; i < possible.size(); i++) {
        for (int j = 0; j < possible[i].size(); j++) {
            Coord c;
            c.x = i;
            c.y = j;

            ans_1 += possible[i][j] && (distance(start, c) % 2);
            ans_2 += possible[i][j] && (distance(start, c) % 2 == 0);
        }
    }

    for (int i = 0; i < possible.size(); i++) {
        if ((i % (m.size() / 5)) == 0) {
            for (int j = 0; j < 132 * 5 + 1; j++) {
                cout << "-";
            }
        }

        for (int j = 0; j < possible[i].size(); j++) {
            if ((j % (m[0].length() / 5)) == 0) {
                cout << "|";
            }
            cout << ((m[i][j] == '.') ? (possible[i][j] ? 'O' : '.') : m[i][j]);
        }
        cout << "|" << endl;
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
