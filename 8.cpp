#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <utility>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <regex>
#include <map>
#include <set>

using namespace std;

size_t string_to_index(const string& s) {
    size_t res = 0;
    for (int i = 0; i < 3; i++) {
        assert('A' <= s[i] && s[i] <= 'Z');
        size_t c = s[i] - 'A';
        res <<= 8;
        res += c;
    }
    return res;
}

char last_letter(size_t v) {
    return 'A' + (v & 0xff);
}

int main() {
    ifstream fin("inputs/8.in");

    string line;
    string token;
    uint64_t number;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    string dir;

    map<size_t, pair<size_t, size_t>> m;

    getline(fin, dir);
    getline(fin, line); // empty

    while (getline(fin, line)) {
        stringstream ss(line);

        size_t src;
        size_t left;
        size_t right;

        ss >> token;
        src = string_to_index(token);

        ss.ignore(4);
        ss >> token;
        left = string_to_index(token);

        ss >> token;
        right = string_to_index(token);

        if (m.find(src) != m.end()) {
            cout << "Map element exist: " << src << " " << token << endl;
            assert(false);
        }

        m[src] = make_pair(left, right);
    }

    size_t start = string_to_index("AAA");
    size_t finish = string_to_index("ZZZ");

    size_t curr = start;
    size_t dir_idx = 0;

    while (curr != finish) {
        if (dir[dir_idx] == 'L') {
            curr = m[curr].first;
        } else {
            curr = m[curr].second;
        }

        dir_idx = (dir_idx + 1) % dir.length();
        ans_1++;
    }

    cout << ans_1 << endl;

    vector<size_t> ghost;
    vector<long> period;

    for ( const auto& [k, v] : m ) {
        if ((k & 0xff) == 0) {
            ghost.push_back(k);
            period.push_back(0);
        }
    }

    dir_idx = 0;
    size_t count = 0;
    while (any_of(period.begin(), period.end(), [](long v) { return v == 0; })) {
        for (int i = 0; i < ghost.size(); i++) {

            if (dir[dir_idx] == 'L') {
                ghost[i] = m[ghost[i]].first;
            } else {
                ghost[i] = m[ghost[i]].second;
            }
        }

        count++;

        for (int i = 0; i < ghost.size(); i++) {
            if (last_letter(ghost[i]) == 'Z') {
                period[i] = count;
                cout << "[" << i << "]: " << period[i] << endl;
            }
        }

        dir_idx = (dir_idx + 1) % dir.length();
    }

    ans_2 = reduce(period.begin(), period.end(), 1LL, [](long a, long b) { return lcm(a, b); });
    cout << ans_2 << endl;

    return 0;
}