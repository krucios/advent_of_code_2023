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

size_t hash_func(const string& s) {
    size_t h = 0;

    for (auto c : s) {
        h += c;
        h *= 17;
        h %= 256;
    }

    return h;
}

int main() {
    ifstream fin("inputs/15.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    map<size_t, vector<pair<string, int>>> m;

    while (getline(fin, line, ',')) {
        size_t h = hash_func(line);
        ans_1 += h;

        size_t minus_pos = line.find('-');
        size_t equal_pos = line.find('=');
        size_t pos = minus_pos != string::npos ? minus_pos : equal_pos;

        string label = line.substr(0, pos);
        h = hash_func(label);

        auto mirror = find_if(m[h].begin(), m[h].end(), [&](const auto& v) { return v.first == label; });
        if (line[pos] == '-') {
            if (mirror != m[h].end()) {
                m[h].erase(mirror);
            }
        } else {
            int focal = stoi(line.substr(pos + 1));

            if (mirror != m[h].end()) {
                (*mirror).second = focal;
            } else {
                m[h].push_back(make_pair(label, focal));
            }
        }
    }

    for (const auto& [k, v] : m) {
        for (int i = 0; i < v.size(); i++) {
            size_t power = (k + 1) * (i + 1) * v[i].second;

            ans_2 += power;
        }
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
