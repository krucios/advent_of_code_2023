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
    long x;
    long y;
};

long shoelace(const vector<Coord>& v) {
    long l_sum = 0;
    long r_sum = 0;

    for (int i = 0; i < v.size(); i++) {
        int j = (i + 1) % v.size();

        l_sum += v[i].x * v[j].y;
        r_sum += v[j].x * v[i].y;
    }

    assert(!(abs(l_sum - r_sum) & 1));
    return abs(l_sum - r_sum) / 2;
}

int main() {
    ifstream fin("inputs/18.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    vector<Coord> v_1;
    vector<Coord> v_2;

    Coord vertex_1;
    vertex_1.x = 0;
    vertex_1.y = 0;

    Coord vertex_2;
    vertex_2.x = 0;
    vertex_2.y = 0;

    long perimeter_1 = 0;
    long perimeter_2 = 0;

    while (getline(fin, line)) {
        stringstream ss(line);

        string token;
        char d;
        int l;

        ss >> d >> l;

        perimeter_1 += l;

        if (d == 'U') {
            vertex_1.x -= l;
        } else if (d == 'D') {
            vertex_1.x += l;
        } else if (d == 'L') {
            vertex_1.y -= l;
        } else if (d == 'R') {
            vertex_1.y += l;
        } else {
            assert(false);
        }
        v_1.push_back(vertex_1);

        ss.ignore(3);

        ss >> token;
        token.pop_back();

        d = token.back();
        token.pop_back();

        l = stol(token, 0, 16);

        perimeter_2 += l;

        if (d == '3') {
            vertex_2.x -= l;
        } else if (d == '1') {
            vertex_2.x += l;
        } else if (d == '2') {
            vertex_2.y -= l;
        } else if (d == '0') {
            vertex_2.y += l;
        } else {
            assert(false);
        }
        v_2.push_back(vertex_2);
    }

    ans_1 = shoelace(v_1) + perimeter_1 / 2 + 1;
    ans_2 = shoelace(v_2) + perimeter_2 / 2 + 1;

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
