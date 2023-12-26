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

size_t count_winning_strategies(size_t time, size_t distance) {
    //
    // It's a quadratic equation!
    // hold * (time - hold) > distance
    // -1 * hold ^ 2 + time * hold - distance > 0
    //
    double d = time * time - 4 * distance;
    double min_hold = (-1 * (double)time + sqrt(d)) / -2;
    double mid = floor(time / 2);

    modf(min_hold, &min_hold);
    min_hold += 1;

    size_t amount = (mid - min_hold + 1) * 2 + (time % 2 ? 0 : -1);

    return amount;
}

int main() {
    ifstream fin("inputs/6.in");

    string line;
    string token;
    uint64_t number;

    uint64_t ans_1 = 1;
    uint64_t ans_2 = 0;

    vector<size_t> times;
    vector<size_t> distances;

    getline(fin, line);

    stringstream time_ss(line);
    time_ss >> token;

    while (!time_ss.eof()) {
        time_ss >> number;

        times.push_back(number);
    }


    getline(fin, line);

    stringstream distance_ss(line);
    distance_ss >> token;

    while (!distance_ss.eof()) {
        distance_ss >> number;

        distances.push_back(number);
    }

    for (size_t i = 0; i < times.size(); i++) {
        ans_2 = count_winning_strategies(times[i], distances[i]);
        ans_1 *= ans_2;

        cout << ans_2 << endl;
    }

    cout << ans_1 << endl;

    return 0;
}