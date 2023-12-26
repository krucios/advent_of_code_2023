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

int main() {
    ifstream fin("inputs/9.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;


    while (getline(fin, line)) {
        stringstream ss(line);

        vector<vector<long>> seqs;
        vector<long> seq;

        while (!ss.eof()) {
            long val;
            ss >> val;
            seq.push_back(val);
        }
        seqs.push_back(seq);

        while (!all_of(seq.begin(), seq.end(), [](size_t v) { return v == 0; })) {
            seq.clear();

            const auto& prev = seqs.back();
            for (int i = 1; i < prev.size(); i++) {
                seq.push_back(prev[i] - prev[i - 1]);
            }

            seqs.push_back(seq);
        }

        seqs.back().push_back(0);
        for (int i = seqs.size() - 2; i >= 0; i--) {
            long new_val = seqs[i].back() + seqs[i + 1].back();
            seqs[i].push_back(new_val);
        }

        seqs.back().insert(seqs.back().begin(), 0);
        for (int i = seqs.size() - 2; i >= 0; i--) {
            long new_val = seqs[i][0] - seqs[i + 1][0];
            seqs[i].insert(seqs[i].begin(), new_val);
        }

        // for (int i = 0; i < seqs.size(); i++) {
        //     for (int j = 0; j < seqs[i].size(); j++) {
        //         cout << seqs[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        ans_1 += seqs[0].back();
        ans_2 += seqs[0][0];
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}