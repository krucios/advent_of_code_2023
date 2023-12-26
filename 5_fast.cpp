#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <regex>
#include <queue>
#include <map>
#include <set>

//
// WARNING!
// This program doesn't work correctly. I wasn't able to merge ranges properly.
//

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

    vector<MapRange> merged(maps[0].ranges);

    for (int i = 1; i < maps.size(); i++) {
        queue<MapRange> to_add;

        for (const auto& r : maps[i].ranges) {
            to_add.push(r);
        }

        while (!to_add.empty()) {
            MapRange nr = to_add.front();
            to_add.pop();

            bool overlap = false;

            for (auto& r : merged) {
                if (r.start <= nr.start && nr.limit <= r.limit) {
                    if (r.start < nr.start) {
                        MapRange left;
                        left.start = r.start;
                        left.limit = nr.start - 1;
                        left.offset = r.offset;

                        to_add.push(left);
                    }

                    if (nr.limit < r.limit) {
                        MapRange right;
                        right.start = nr.limit + 1;
                        right.limit = r.limit;
                        right.offset = r.offset;

                        to_add.push(right);
                    }

                    r.start = nr.start;
                    r.limit = nr.limit;
                    r.offset += nr.offset;

                    overlap = true;
                    break;
                } else if (nr.start < r.start && r.limit < nr.limit) {
                    r.offset += nr.offset;

                    MapRange left;
                    left.start = nr.start;
                    left.limit = r.start - 1;
                    left.offset = nr.offset;

                    to_add.push(left);

                    MapRange right;
                    right.start = r.limit + 1;
                    right.limit = nr.limit;
                    right.offset = nr.offset;

                    to_add.push(right);

                    overlap = true;
                    break;
                } else if (r.start <= nr.start && nr.start <= r.limit) {
                    if (r.start < nr.start) {
                        MapRange left;
                        left.start = r.start;
                        left.limit = nr.start - 1;
                        left.offset = r.offset;

                        to_add.push(left);
                    }

                    if (r.limit < nr.limit) {
                        MapRange right;
                        right.start = r.limit + 1;
                        right.limit = nr.limit;
                        right.offset = nr.offset;

                        to_add.push(right);
                    }

                    r.start = nr.start;
                    r.offset += nr.offset;

                    overlap = true;
                    break;
                } else if (r.start <= nr.limit && nr.limit <= r.limit) {
                    if (nr.start < r.start) {
                        MapRange left;
                        left.start = nr.start;
                        left.limit = r.start - 1;
                        left.offset = nr.offset;

                        to_add.push(left);
                    }

                    if (nr.limit < r.limit) {
                        MapRange right;
                        right.start = nr.limit + 1;
                        right.limit = r.limit;
                        right.offset = r.offset;

                        to_add.push(right);
                    }

                    r.limit = nr.limit;
                    r.offset += nr.offset;

                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                merged.push_back(nr);
            }
        }
    }

    sort(merged.begin(), merged.end(), [](const MapRange& a, const MapRange& b) { return a.start < b.start; });

    long ans = INT64_MAX;
    for (auto seed : seeds) {
        for (const auto& r : merged) {
            if (r.start <= seed[0] && seed[0] <= r.limit) {
                ans = min(ans, seed[0] + r.offset);
                break;
            }
        }

        for (const auto& r : merged) {
            if (seed[0] < r.start && r.start <= seed[0] + seed[1]) {
                ans = min(ans, r.start + r.offset);
                break;
            }
        }

        cout << seed[0] << " - " << seed[1] << endl;
    }

    cout << ans << endl;

    return 0;
}