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

struct Hand {
    array<int, 5> cards;
    int bid;
    int type;
};

int card_to_int(char card) {
    //
    // Part 1
    //
    // switch (card) {
    //     case 'A': return 14;
    //     case 'K': return 13;
    //     case 'Q': return 12;
    //     case 'J': return 11;
    //     case 'T': return 10;
    //     case '9': return 9;
    //     case '8': return 8;
    //     case '7': return 7;
    //     case '6': return 6;
    //     case '5': return 5;
    //     case '4': return 4;
    //     case '3': return 3;
    //     case '2': return 2;
    //     default: assert(false);
    // }
    switch (card) {
        case 'A': return 13;
        case 'K': return 12;
        case 'Q': return 11;
        case 'T': return 10;
        case '9': return 9;
        case '8': return 8;
        case '7': return 7;
        case '6': return 6;
        case '5': return 5;
        case '4': return 4;
        case '3': return 3;
        case '2': return 2;
        case 'J': return 1;
        default: assert(false);
    }
    return 0;
}

int type_of_hand(const Hand& h) {
    int type;

    array<int, 15> amount_of_cards = {};

    //
    // Part 1
    //
    // for (auto c : h.cards) {
    //     amount_of_cards[c]++;
    // }
    int amount_of_jokers = 0;
    for (auto c : h.cards) {
        if (c == 1) {
            amount_of_jokers++;
            continue;
        }
        amount_of_cards[c]++;
    }

    int max_amount = -1;
    int max_idx = -1;
    for (int i = 0; i < amount_of_cards.size(); i++) {
        if (amount_of_cards[i] > max_amount) {
            max_amount = amount_of_cards[i];
            max_idx = i;
        }
    }
    amount_of_cards[max_idx] += amount_of_jokers;

    array<int, 6> amount_of_same = {};

    for (auto a : amount_of_cards) {
        amount_of_same[a]++;
    }

    if (amount_of_same[5]) {
        type = 7;
    } else if (amount_of_same[4]) {
        type = 6;
    } else if (amount_of_same[3] && amount_of_same[2]) {
        type = 5;
    } else if (amount_of_same[3]) {
        type = 4;
    } else if (amount_of_same[2] == 2) {
        type = 3;
    } else if (amount_of_same[2]) {
        type = 2;
    } else {
        type = 1;
    }

    return type;
}

int main() {
    ifstream fin("inputs/7.in");

    string line;
    string token;
    int number;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    vector<Hand> hands;

    while (getline(fin, line)) {
        stringstream ss(line);

        ss >> token;
        ss >> number;

        Hand h;

        for (int i = 0; i < token.length(); i++) {
            h.cards[i] = card_to_int(token[i]);
        }
        h.bid = number;
        h.type = type_of_hand(h);

        hands.push_back(h);
    }

    sort(hands.begin(), hands.end(), [](const Hand& a, const Hand& b) {
        if (a.type == b.type) {
            for (int i = 0; i < a.cards.size(); i++) {
                if (a.cards[i] != b.cards[i]) {
                    return a.cards[i] < b.cards[i];
                }
            }
            return false;
        } else {
            return a.type < b.type;
        }
    });

    for (int i = 0; i < hands.size(); i++) {
        cout << "( ";
        for (auto c : hands[i].cards) {
            cout << c << " ";
        }
        cout << "): " << hands[i].type << " $" << hands[i].bid << endl;

        ans_1 += (i + 1) * hands[i].bid;
    }

    cout << ans_1 << endl;

    return 0;
}