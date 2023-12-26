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

int solve_part_1(const vector<string>& m) {
    int answer = 0;

    vector<int> rows;
    vector<int> cols;

    for (int i = 0; i < m.size(); i++) {
        int val = 0;
        for (int j = 0; j < m[0].length(); j++) {
            val |= m[i][j] == '#';
            val <<= 1;
        }
        rows.push_back(val);
    }

    for (int j = 0; j < m[0].length(); j++) {
        int val = 0;
        for (int i = 0; i < m.size(); i++) {
            val |= m[i][j] == '#';
            val <<= 1;
        }
        cols.push_back(val);
    }

    for (int h = 0; h < m.size() - 1; h++) {
        bool is_perfect = true;
        for (int i = h, j = h + 1; (i >= 0) && (j < m.size()); i--, j++) {
            if (rows[i] != rows[j]) {
                is_perfect = false;
                break;
            }
        }

        if (is_perfect) {
            return 100 * (h + 1);
        }
    }

    for (int v = 0; v < m[0].length() - 1; v++) {
        bool is_perfect = true;
        for (int i = v, j = v + 1; (i >= 0) && (j < m[0].length()); i--, j++) {
            if (cols[i] != cols[j]) {
                is_perfect = false;
                break;
            }
        }

        if (is_perfect) {
            return v + 1;
        }
    }

    return answer;
}

int solve_part_2(const vector<string>& m) {
    int answer = 0;

    vector<uint64_t> rows;
    vector<uint64_t> cols;

    for (int i = 0; i < m.size(); i++) {
        int val = 0;
        for (int j = 0; j < m[0].length(); j++) {
            val |= static_cast<uint64_t>(m[i][j] == '#');
            val <<= 1;
        }
        rows.push_back(val);
    }

    for (int j = 0; j < m[0].length(); j++) {
        int val = 0;
        for (int i = 0; i < m.size(); i++) {
            val |= static_cast<uint64_t>(m[i][j] == '#');
            val <<= 1;
        }
        cols.push_back(val);
    }

    for (int h = 0; h < m.size() - 1; h++) {
        size_t mistakes = 0;
        for (int i = h, j = h + 1; (i >= 0) && (j < m.size()); i--, j++) {
            mistakes += popcount(rows[i] ^ rows[j]);
            if (mistakes > 1) {
                break;
            }
        }

        if (mistakes == 1) {
            return 100 * (h + 1);
        }
    }

    for (int v = 0; v < m[0].length() - 1; v++) {
        size_t mistakes = 0;
        for (int i = v, j = v + 1; (i >= 0) && (j < m[0].length()); i--, j++) {
            mistakes += popcount(cols[i] ^ cols[j]);
            if (mistakes > 1) {
                break;
            }
        }

        if (mistakes == 1) {
            return v + 1;
        }
    }

    return answer;
}

int main() {
    ifstream fin("inputs/13.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<string> task;

    while (getline(fin, line)) {
        if (line == "") {
            ans_1 += solve_part_1(task);
            ans_2 += solve_part_2(task);
            task.clear();
        } else {
            task.push_back(line);
        }
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}