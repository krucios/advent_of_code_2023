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

void print_map(const vector<string>& m) {
    for (const auto& r : m) {
        cout << r << endl;
    }
    cout << endl;
}

size_t count_weight(const vector<string>& m) {
    size_t ans = 0;

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].length(); j++) {
            if (m[i][j] == 'O') {
                ans += m.size() - i;
            }
        }
    }

    return ans;
}

void tilt_north(vector<string>& m) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].length(); j++) {
            if (m[i][j] == 'O') {
                m[i][j] = '.';

                int new_i = i;
                while (new_i >= 0 && m[new_i][j] == '.') {
                    new_i--;
                }

                m[new_i + 1][j] = 'O';
            }
        }
    }
}

void tilt_west(vector<string>& m) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].length(); j++) {
            if (m[i][j] == 'O') {
                m[i][j] = '.';

                int new_j = j;
                while (new_j >= 0 && m[i][new_j] == '.') {
                    new_j--;
                }

                m[i][new_j + 1] = 'O';
            }
        }
    }
}

void tilt_south(vector<string>& m) {
    for (int i = m.size() - 1; i >= 0; i--) {
        for (int j = 0; j < m[0].length(); j++) {
            if (m[i][j] == 'O') {
                m[i][j] = '.';

                int new_i = i;
                while (new_i < m.size() && m[new_i][j] == '.') {
                    new_i++;
                }

                m[new_i - 1][j] = 'O';
            }
        }
    }
}

void tilt_east(vector<string>& m) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = m[0].length() - 1; j >= 0; j--) {
            if (m[i][j] == 'O') {
                m[i][j] = '.';

                int new_j = j;
                while (new_j < m[0].length() && m[i][new_j] == '.') {
                    new_j++;
                }

                m[i][new_j - 1] = 'O';
            }
        }
    }
}

void tilt_cycle(vector<string>& m) {
    tilt_north(m);
    tilt_west(m);
    tilt_south(m);
    tilt_east(m);
}

int main() {
    ifstream fin("inputs/14.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<string> m;

    while (getline(fin, line)) {
        m.push_back(line);
    }

    vector<string> m_copy(m);
    tilt_north(m_copy);
    ans_1 = count_weight(m_copy);

    cout << ans_1 << endl;

    for (int i = 0; i < 1000; i++) {
        tilt_cycle(m);
        cout << count_weight(m) << endl;
    }

    // Pattern is repeating, so do rest manually
    // Find period and then get a reminder of
    // 1000000000 / period
    // offset - 86
    // period - 27

    cout << ans_2 << endl;

    return 0;
}
