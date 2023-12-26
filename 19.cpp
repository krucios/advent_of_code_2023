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

struct Rule {
    bool is_cond;
    bool is_greater;
    int border;
    string var;
    string dest;
};

struct Gift {
    int x;
    int m;
    int a;
    int s;
};

struct Range {
    int x[2];
    int m[2];
    int a[2];
    int s[2];

    uint64_t cases() const {
        int64_t c =  static_cast<int64_t>(x[1] - x[0] + 1)
                   * static_cast<int64_t>(m[1] - m[0] + 1)
                   * static_cast<int64_t>(a[1] - a[0] + 1)
                   * static_cast<int64_t>(s[1] - s[0] + 1);

        return c > 0 ? c : 0;
    }
};

bool is_applicable(const Gift& g, const Rule& r) {
    if (!r.is_cond) {
        return true;
    }

    if (r.var == "x") {
        return r.is_greater ? g.x > r.border : g.x < r.border;
    } else if (r.var == "m") {
        return r.is_greater ? g.m > r.border : g.m < r.border;
    } else if (r.var == "a") {
        return r.is_greater ? g.a > r.border : g.a < r.border;
    } else if (r.var == "s") {
        return r.is_greater ? g.s > r.border : g.s < r.border;
    } else {
        assert(false);
    }
}

bool is_accepted(const Gift& g, map<string, vector<Rule>>& rules) {
    string cur_rule = "in";

    while (true) {
        for (auto& r : rules[cur_rule]) {
            if (r.is_cond) {
                if (is_applicable(g, r)) {
                    if (r.dest == "A" || r.dest == "R") {
                        return r.dest == "A";
                    }
                    cur_rule = r.dest;
                    break;
                }
            } else {
                if (r.dest == "A" || r.dest == "R") {
                    return r.dest == "A";
                }
                cur_rule = r.dest;
                break;
            }
        }
    }
}

Range get_rule_range(Range& range, Rule rule) {
    Range new_range = range;

    if (!rule.is_cond) {
        range.x[0] = 0; range.x[1] = 0;
        range.m[0] = 0; range.m[1] = 0;
        range.a[0] = 0; range.a[1] = 0;
        range.s[0] = 0; range.s[1] = 0;
        return new_range;
    }

    if (rule.var == "x") {
        if (rule.is_greater) {
            new_range.x[0] = max(rule.border + 1, range.x[0]);
            range.x[1] = min(rule.border, range.x[1]);
        } else {
            new_range.x[1] = min(rule.border - 1, range.x[1]);
            range.x[0] = max(rule.border, range.x[0]);
        }
    } else if (rule.var == "m") {
        if (rule.is_greater) {
            new_range.m[0] = max(rule.border + 1, range.m[0]);
            range.m[1] = min(rule.border, range.m[1]);
        } else {
            new_range.m[1] = min(rule.border - 1, range.m[1]);
            range.m[0] = max(rule.border, range.m[0]);
        }
    } else if (rule.var == "a") {
        if (rule.is_greater) {
            new_range.a[0] = max(rule.border + 1, range.a[0]);
            range.a[1] = min(rule.border, range.a[1]);
        } else {
            new_range.a[1] = min(rule.border - 1, range.a[1]);
            range.a[0] = max(rule.border, range.a[0]);
        }
    } else if (rule.var == "s") {
        if (rule.is_greater) {
            new_range.s[0] = max(rule.border + 1, range.s[0]);
            range.s[1] = min(rule.border, range.s[1]);
        } else {
            new_range.s[1] = min(rule.border - 1, range.s[1]);
            range.s[0] = max(rule.border, range.s[0]);
        }
    } else {
        assert(false);
    }

    return new_range;
}

uint64_t count_accepted(const string& rule, Range range, map<string, vector<Rule>>& rules) {
    uint64_t ans = 0;

    if (rule == "A") {
        return range.cases();
    }

    if (rule == "R") {
        return 0;
    }

    for (auto& r : rules[rule]) {
        Range rule_range = get_rule_range(range, r);
        ans += count_accepted(r.dest, rule_range, rules);
    }

    return ans;
}

int main() {
    ifstream fin("inputs/19.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    map<string, vector<Rule>> rules;
    vector<Gift> gifts;

    bool is_rules_section = true;

    while (getline(fin, line)) {
        stringstream ss(line);
        string name;
        string token;

        if (line == "") {
            is_rules_section = false;
            continue;
        }

        if (is_rules_section) {
            getline(ss, name, '{');

            while (getline(ss, token, ',')) {
                Rule r;

                if (token.find('}') != string::npos) { // Last rule
                    token.pop_back();
                    r.is_cond = false;
                    r.dest = token;
                } else {
                    r.is_cond = true;
                    r.var = token[0];
                    token.erase(0, 1);
                    r.is_greater = token[0] == '>';
                    token.erase(0, 1);

                    stringstream token_ss(token);
                    token_ss >> r.border;
                    token_ss.ignore(1);

                    token_ss >> r.dest;
                }

                rules[name].push_back(r);
            }
        } else {
            Gift g;

            ss.ignore(3);
            ss >> g.x;
            ss.ignore(3);
            ss >> g.m;
            ss.ignore(3);
            ss >> g.a;
            ss.ignore(3);
            ss >> g.s;

            gifts.push_back(g);
        }
    }

    for (const auto& g : gifts) {
        if (is_accepted(g, rules)) {
            ans_1 += g.x + g.m + g.a + g.s;
        }
    }

    Range r;
    r.x[0] = 1; r.x[1] = 4000;
    r.m[0] = 1; r.m[1] = 4000;
    r.a[0] = 1; r.a[1] = 4000;
    r.s[0] = 1; r.s[1] = 4000;
    ans_2 = count_accepted("in", r, rules);

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
