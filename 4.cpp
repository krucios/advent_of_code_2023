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

int main() {
    ifstream fin("inputs/4.in");

    string line;
    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<size_t> cards_won;
    vector<size_t> cards_amount;

    while (getline(fin, line)) {
        stringstream ss(line);
        string token;
        size_t game_num;
        size_t num;
        bool is_winning_section = true;

        ss >> token;
        assert(token == "Card");

        ss >> num;

        ss >> token;
        assert(token == ":");

        set<size_t> winning;
        size_t numbers_won = 0;

        while (!ss.eof()) {
            ss.ignore(1); // Ignore whitespace

             if (ss.peek() == '|') {
                is_winning_section = false;
                ss >> token;
                assert(token == "|");

                continue;
            }

            ss >> num;
            if (is_winning_section) {
                winning.insert(num);
            } else {
                numbers_won += size_t(winning.find(num) != winning.end());
            }
        }

        ans_1 += (numbers_won != 0) << (numbers_won - 1);

        cards_won.push_back(numbers_won);
        cards_amount.push_back(1);
    }
    cout << ans_1 << endl;

    for (size_t i = 0; i < cards_won.size(); i++) {
        for (size_t j = i + 1; j < min(cards_won.size(), i + cards_won[i] + 1); j++) {
            cards_amount[j] += cards_amount[i];
        }
    }

    for (const auto c : cards_amount) {
        ans_2 += c;
    }
    cout << ans_2 << endl;

    return 0;
}