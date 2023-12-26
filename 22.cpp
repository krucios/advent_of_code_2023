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
  int z;
};

struct Brick {
    int x[2];
    int y[2];
    int z[2];
};

typedef array<Coord, 2> brick_t;

bool has_xy_overlap(const Brick& a, const Brick& b) {
    bool x_overlap = min(a.x[1], b.x[1]) >= max(a.x[0], b.x[0]);
    bool y_overlap = min(a.y[1], b.y[1]) >= max(a.y[0], b.y[0]);
    return x_overlap && y_overlap;
}

bool has_xyz_overlap(const Brick& a, const Brick& b) {
    bool z_overlap = min(a.z[1], b.z[1]) >= max(a.z[0], b.z[0]);
    return has_xy_overlap(a, b) && z_overlap;
}

bool can_fall(const Brick& b, const vector<Brick>& bricks) {
    int h = 1;

    for (int i = 0; i < bricks.size(); i++) {

        if (b.z[0] > bricks[i].z[0] && has_xy_overlap(b, bricks[i])) {
            h = max(h, bricks[i].z[1] + 1);
        }
    }

    return (b.z[0] - h) > 0;
}

bool is_stable(const vector<Brick>& bricks) {
    bool is_stable = true;

    for (int i = 0; i < bricks.size(); i++) {
        is_stable &= !can_fall(bricks[i], bricks);
    }

    return is_stable;
}

int fall(vector<Brick>& bricks) {
    int amount_fallen = 0;

    for (int i = 0; i < bricks.size(); i++) {
        int h = 1;

        for (int j = 0; j < i; j++) {
            if (has_xy_overlap(bricks[i], bricks[j])) {
                h = max(h, bricks[j].z[1] + 1);
            }
        }

        assert(h <= bricks[i].z[0]);
        int fall_distance = bricks[i].z[0] - h;
        bricks[i].z[0] -= fall_distance;
        bricks[i].z[1] -= fall_distance;

        amount_fallen += fall_distance > 0;
    }

    return amount_fallen;
}

int main() {
    ifstream fin("inputs/22.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    vector<Brick> bricks;

    while (getline(fin, line)) {
        Brick b;

        string position_line;
        stringstream positions_ss(line);
        for (int i = 0; i < 2; i++) {
            getline(positions_ss, position_line, '~');
            stringstream coord_ss(position_line);
            string token;

            getline(coord_ss, token, ',');
            b.x[i] = stoi(token);
            getline(coord_ss, token, ',');
            b.y[i] = stoi(token);
            getline(coord_ss, token, ',');
            b.z[i] = stoi(token);
        }

        Brick a;
        a.x[0] = min(b.x[0], b.x[1]);
        a.x[1] = max(b.x[0], b.x[1]);
        a.y[0] = min(b.y[0], b.y[1]);
        a.y[1] = max(b.y[0], b.y[1]);
        a.z[0] = min(b.z[0], b.z[1]);
        a.z[1] = max(b.z[0], b.z[1]);

        bricks.push_back(a);
    }

    sort(bricks.begin(), bricks.end(), [](const auto& a, const auto& b) {
        return a.z[0] < b.z[0];
    });

    fall(bricks);

    for (int i = 0; i < bricks.size(); i++) {
        vector<Brick> new_bricks(bricks);
        new_bricks.erase(new_bricks.begin() + i);

        int amount_fallen = fall(new_bricks);
        ans_1 += (amount_fallen == 0);
        ans_2 += amount_fallen;
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
