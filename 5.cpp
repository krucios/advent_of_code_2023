#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <regex>
#include <map>
#include <set>

using namespace std;

struct MapRange {
    long start;
    long limit;
    long offset;
};

struct GardenMap {
    string src;
    string dst;
    vector<MapRange> ranges;
};

int main() {
    ifstream fin("inputs/5.in");

    string line;
    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<array<long, 2>> seeds;

    getline(fin, line);
    stringstream ss(line);
    getline(fin, line);

    string token;
    long number;

    ss >> token;
    assert(token == "seeds:");

    while (!ss.eof()) {
        array<long, 2> s;
        ss >> s[0] >> s[1];
        seeds.push_back(s);
    }

    vector<GardenMap> maps;
    GardenMap current_map;

    bool is_header = true;

    while (getline(fin, line)) {
        if (line == "") {
            maps.push_back(current_map);
            current_map.ranges.clear();
            is_header = true;
            continue;
        }

        stringstream ss(line);

        if (is_header) {
            ss >> token;

            size_t split_pos = token.find('-');
            current_map.src = token.substr(0, split_pos);

            split_pos = token.find('-', split_pos + 1);
            current_map.dst = token.substr(split_pos + 1);

            is_header = false;
        } else {
            MapRange range;

            ss >> number;
            range.offset = number;

            ss >> number;
            range.start = number;
            range.offset -= number;

            ss >> number;
            range.limit = range.start + number - 1;

            current_map.ranges.push_back(range);
        }
    }
    maps.push_back(current_map);

    long ans = INT64_MAX;
    for (auto seed : seeds) {
        for (long s = seed[0], l = seed[0] + seed[1]; s < l; s++) {
            long loc = s;
            for (const auto& m : maps) {
                for (const auto& r : m.ranges) {
                    if (r.start <= loc && loc <= r.limit) {
                        loc += r.offset;
                        break;
                    }
                }
            }

            ans = min(ans, loc);
        }
        cout << seed[0] << " - " << seed[1] << endl;
    }

    cout << ans << endl;

    return 0;
}