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

int main() {
    ifstream fin("inputs/11.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<string> map;
    vector<bool> is_row_empty;
    vector<bool> is_col_empty;

    vector<pair<int, int>> galaxies;

    while (getline(fin, line)) {
        map.push_back(line);
        is_row_empty.push_back(all_of(line.begin(), line.end(), [](char c) { return c == '.'; }));
    }

    for (int j = 0; j < map[0].length(); j++) {
        bool is_clear = true;

        for (int i = 0; i < map.size(); i++) {
            is_clear &= map[i][j] == '.';

            if (map[i][j] == '#') {
                galaxies.push_back(make_pair(i, j));
            }
        }

        is_col_empty.push_back(is_clear);
    }

    for (int i = 0; i < galaxies.size() - 1; i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {

            int min_x = min(galaxies[i].first, galaxies[j].first);
            int max_x = max(galaxies[i].first, galaxies[j].first);
            int min_y = min(galaxies[i].second, galaxies[j].second);
            int max_y = max(galaxies[i].second, galaxies[j].second);

            for (int ii = min_x; ii < max_x; ii++) {
                ans_1 += 1ULL + is_row_empty[ii];
                ans_2 += 1ULL + (is_row_empty[ii] * 999999);
            }
            for (int jj = min_y; jj < max_y; jj++) {
                ans_1 += 1ULL + is_col_empty[jj];
                ans_2 += 1ULL + (is_col_empty[jj] * 999999);
            }
        }
    }


    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}