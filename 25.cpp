#include <iostream>
#include <optional>
#include <cassert>
#include <sstream>
#include <fstream>
#include <utility>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <regex>
#include <map>
#include <set>
#include <bit>

using namespace std;

void visit_edges(const size_t v, const vector<vector<bool>>& E, vector<vector<size_t>>& C) {
    vector<bool> visited;
    visited.resize(E.size(), false);

    queue<size_t> bfs;
    bfs.push(v);

    while (!bfs.empty()) {
        size_t src = bfs.front();
        bfs.pop();

        for (size_t i = 0; i < E.size(); i++) {
            if (!visited[i] && E[src][i]) {
                visited[i] = true;
                C[src][i]++;
                C[i][src]++;
                bfs.push(i);
            }
        }
    }
}

pair<size_t, size_t> conencted_components(const vector<vector<bool>>& E) {
    vector<bool> visited;
    visited.resize(E.size(), false);

    stack<size_t> dfs;
    dfs.push(0);
    visited[0] = true;

    while (!dfs.empty()) {
        size_t src = dfs.top();
        dfs.pop();

        for (size_t i = 0; i < E.size(); i++) {
            if (!visited[i] && E[src][i]) {
                visited[i] = true;
                dfs.push(i);
            }
        }
    }

    size_t connected = count(visited.begin(), visited.end(), true);

    return make_pair(connected, visited.size() - connected);
}

int main() {
    ifstream fin("inputs/25.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    map<size_t, vector<size_t>> G;
    vector<vector<bool>> E;
    vector<vector<size_t>> C;
    map<string, size_t> ids;
    size_t id = 0;

    while (getline(fin, line)) {
        auto pos = line.find(':');
        string token = line.substr(0, pos);

        line.erase(0, pos + 1);

        string source = token;
        size_t src_id;
        if (ids.find(source) == ids.end()) {
            src_id = id;
            ids[source] = id++;
        } else {
            src_id = ids[source];
        }

        stringstream ss(line);
        while (!ss.eof()) {
            ss >> token;

            size_t dst_id;
            string destination = token;

            if (ids.find(destination) == ids.end()) {
                dst_id = id;
                ids[destination] = id++;
            } else {
                dst_id = ids[destination];
            }

            G[src_id].push_back(dst_id);
            G[dst_id].push_back(src_id);
        }
    }

    size_t N = G.size();

    E.resize(N);
    C.resize(N);
    for (auto& e : E) {
        e.resize(N, false);
    }
    for (auto& c : C) {
        c.resize(N, 0);
    }

    for (const auto& [src, dsts] : G) {
        for (const auto& dst : dsts) {
            E[src][dst] = true;
        }
    }

    //
    // 1) Find most visited edges. Answer is likely to be between them.
    //    This step helps to reduce problem size significantly.
    //
    for (int i = 0; i < N; i++) {
        visit_edges(i, E, C);
    }

    size_t max_visits = 0;
    for (const auto& c : C) {
        auto max_c = max_element(c.begin(), c.end());
        max_visits = max(max_visits, *max_c);
    }

    size_t max_count = 0;
    vector<pair<size_t, size_t>> edges;

    while (edges.size() < 10 && max_visits > 0) {
        for (size_t i = 0; i < N - 1; i++) {
            for (size_t j = i + 1; j < N; j++) {
                if (C[i][j] == max_visits) {
                    edges.push_back(make_pair(i, j));
                    max_count++;
                }
            }
        }
        max_visits--;
    }

    size_t M = edges.size();

    cout << "Found " << max_count << " most visited edges" << endl;

    //
    // 2) For all possible combinations of 3 edges from step 1)
    //    try to remove them and check if graph is still connected.
    //

    for (size_t i = 0; i < M - 2; i++) {
        auto e1_src = edges[i].first;
        auto e1_dst = edges[i].second;

        E[e1_src][e1_dst] = false;
        E[e1_dst][e1_src] = false;

        for (size_t j = i + 1; j < M - 1; j++) {
            auto e2_src = edges[j].first;
            auto e2_dst = edges[j].second;

            E[e2_src][e2_dst] = false;
            E[e2_dst][e2_src] = false;

            for (size_t k = j + 1; k < M; k++) {
                auto e3_src = edges[k].first;
                auto e3_dst = edges[k].second;

                // 1) Remove selected edges
                E[e3_src][e3_dst] = false;
                E[e3_dst][e3_src] = false;

                // 2) Check graph connectivity
                auto cc = conencted_components(E);
                if (cc.second != 0) {
                    cout << "Got it! i = " << i << ", j = " << j << ", k = " << k << endl;
                    ans_1 = cc.first * cc.second;
                }

                // 3) Bring edges back
                E[e3_src][e3_dst] = true;
                E[e3_dst][e3_src] = true;
            }

            E[e2_src][e2_dst] = true;
            E[e2_dst][e2_src] = true;
        }

        E[e1_src][e1_dst] = true;
        E[e1_dst][e1_src] = true;
    }

    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
