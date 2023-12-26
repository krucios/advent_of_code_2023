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

typedef vector<string> matrix_t;

array<int, 8> dx = {-1,  0,  1,  1,  1,  0, -1, -1};
array<int, 8> dy = {-1, -1, -1,  0,  1,  1,  1,  0};

bool is_symbol(char c) {
    return c != '.' && (c < '0' || c > '9');
}

bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

size_t to_integer(const vector<size_t>& v) {
    size_t ret = 0;

    for (int i = 0; i < v.size(); i++) {
        ret *= 10;
        ret += v[i];
    }

    return ret;
}

int main() {
    ifstream fin("inputs/3.in");

    string line;
    matrix_t schematic;

    while (getline(fin, line)) {
        schematic.push_back(line);
    }

    size_t ans_1 = 0;
    vector<size_t> number;
    bool is_number_in_progress = false;
    bool is_number_a_part = false;

    map<size_t, vector<size_t>> gears;
    set<size_t> gear_ids;

    for (size_t i = 0, n = schematic.size(); i < n; i++) {
        for (size_t j = 0, m = schematic[i].length(); j < m; j++) {
            char c = schematic[i][j];

            if (is_number_in_progress) {
                if (!is_digit(c)) {
                    size_t part_num;

                    part_num = to_integer(number);

                    if (is_number_a_part) {
                        ans_1 += part_num;
                    }

                    for (const auto gid : gear_ids) {
                        gears[gid].push_back(part_num);
                    }

                    number.clear();
                    gear_ids.clear();
                    is_number_in_progress = false;
                    is_number_a_part = false;
                } else {
                    number.push_back(c - '0');
                }
            } else {
                if (is_digit(c)) {
                    number.push_back(c - '0');
                    is_number_in_progress = true;
                }
            }

            if (is_number_in_progress) {
                for (int k = 0; k < dx.size(); k++) {
                    int new_i = i + dy[k];
                    int new_j = j + dx[k];

                    if ((0 <= new_i && new_i < n) && (0 <= new_j && new_j < m)) {
                        is_number_a_part |= is_symbol(schematic[new_i][new_j]);
                        if (schematic[new_i][new_j] == '*') {
                            gear_ids.insert(new_i * max(n, m) + new_j);
                        }
                    }
                }
            }
        }
    }

    size_t ans_2 = 0;
    for (const auto& [id, parts] : gears) {
        if (parts.size() == 2) {
            ans_2 += parts[0] * parts[1];
        }
    }

    cout << "Part 1 answer: " << ans_1 << endl;
    cout << "Part 2 answer: " << ans_2 << endl;

    return 0;
}