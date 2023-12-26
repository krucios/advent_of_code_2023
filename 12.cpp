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

uint64_t count_possible_answers(const string& p, const vector<int>& segments) {
    uint64_t count = 0;

    map<uint64_t, map<uint64_t, uint64_t>> memoization;

    std::function<uint64_t(int, int)> r;
    r = [&](int i, int j) {
        uint64_t count = 0;
        int start;

        if (memoization.find(i) != memoization.end()) {
            if (memoization[i].find(j) != memoization[i].end()) {
                return memoization[i][j];
            }
        }

        // Can't finish sequence in definitely empty cell
        if (p[i] == '.') {
            memoization[i][j] = 0ULL;
            return memoization[i][j];
        }

        // Sequence doesn't fit
        start = i - segments[j] + 1;
        if (start < 0) {
            memoization[i][j] = 0ULL;
            return memoization[i][j];
        }

        // To be possible, all characters for the last sequence item have to be '?' or '#'
        for (int ii = start; ii <= i; ii++) {
            if (p[ii] == '.') {
                memoization[i][j] = 0ULL;
                return memoization[i][j];
            }
        }

        // At least one space between sequences
        if (start > 0 && p[start - 1] == '#') {
            memoization[i][j] = 0ULL;
            return memoization[i][j];
        }

        // Can't place sequence further than the first '#'. Otherwise it should be possible given
        // previous conditions.
        if (j == 0) {
            memoization[i][j] = static_cast<int>(start <= p.find('#'));
            return memoization[i][j]; // npos is fine here as it's very big
        }

        // Main DP loop: count amount of cases which can lead to current situation (i, j)
        for (int jj = start - 2; jj >= 0; jj--) {
            count += r(jj, j - 1);

            // The previous sequence can't finish earlier than the '#' given that this '#' is
            // outside of allowed current sequence (j) placement.
            if (p[jj] == '#') {
                break;
            }
        }

        memoization[i][j] = count;
        return memoization[i][j];
    };

    int start = accumulate(segments.begin(), segments.end(), 0);
    start += segments.size() - 2;

    int last_hash_pos = p.rfind('#');
    start = max(start, last_hash_pos);

    for (int i = start; i < p.length(); i++) {
        count += r(i, segments.size() - 1);
    }

    return count;
}

int main() {
    ifstream fin("inputs/12.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    while (getline(fin, line)) {
        stringstream ss(line);

        string pattern;
        vector<int> segments;

        ss >> pattern;

        while (!ss.eof()) {
            int val;

            ss.ignore(1);
            ss >> val;
            segments.push_back(val);
        }

        uint64_t ans = count_possible_answers(pattern, segments);
        ans_1 += ans;

        string new_pattern = pattern;
        vector<int> new_segments(segments);
        for (int i = 0; i < 4; i++) {
            new_pattern += "?" + pattern;
            new_segments.insert(new_segments.end(), segments.begin(), segments.end());
        }

        ans = count_possible_answers(new_pattern, new_segments);
        ans_2 += ans;
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}