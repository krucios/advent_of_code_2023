#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <regex>

using namespace std;

struct RGB {
    size_t r;
    size_t g;
    size_t b;
};

bool is_possible_game(const vector<RGB>& game) {
    for (const auto& e: game) {
        if (e.r > 12 || e.g > 13 || e.b > 14) {
            return false;
        }
    }
    return true;
}

size_t game_power(const vector<RGB>& game) {
    return (*max_element(game.begin(), game.end(), [](const RGB& a, const RGB& b) { return a.r < b.r; })).r
         * (*max_element(game.begin(), game.end(), [](const RGB& a, const RGB& b) { return a.g < b.g; })).g
         * (*max_element(game.begin(), game.end(), [](const RGB& a, const RGB& b) { return a.b < b.b; })).b;
}

int main() {
    ifstream fin("inputs/2.in");

    string line;
    size_t sum = 0;
    size_t power_sum = 0;

    while (getline(fin, line)) {
        stringstream ss(line);

        string token;
        size_t number;
        size_t game_id;
        size_t power;

        vector<RGB> game;

        ss >> token;
        assert(token == "Game");

        ss >> game_id;

        ss >> token;
        assert(token == ":");

        game.push_back({0, 0, 0});
        while (!ss.eof()) {
            ss >> number;
            ss >> token;

            if (token.rfind("red", 0) == 0) {
                game.back().r = number;
            } else if (token.rfind("green", 0) == 0) {
                game.back().g = number;
            } else if (token.rfind("blue", 0) == 0) {
                game.back().b = number;
            } else {
                assert(false);
            }

            if (token.back() == ';') {
                game.push_back({0, 0, 0});
            }
        }

        power_sum += game_power(game);

        if (is_possible_game(game)) {
            sum += game_id;
        }

        cout << "Game " << game_id << ": ";
        for (const auto& e: game) {
            cout << e.r << ", " << e.g << ", " << e.b << "; ";
        }
        cout << endl;
    }

    cout << "Answer part 1: " << sum << endl;
    cout << "Answer part 2: " << power_sum << endl;

    return 0;
}