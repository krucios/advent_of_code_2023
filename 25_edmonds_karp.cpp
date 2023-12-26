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

pair<size_t, size_t> components(const vector<vector<size_t>>& E) {
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

size_t bfs(size_t src, size_t dst, vector<size_t>& path, const vector<vector<size_t>>& C) {
    size_t big_val = C.size() + 2;
    fill(path.begin(), path.end(), big_val);
    path[src] = big_val - 1;

    queue<pair<size_t, size_t>> q;
    q.push({src, big_val});

    while (!q.empty()) {
        size_t cur = q.front().first;
        size_t flow = q.front().second;
        q.pop();

        for (int i = 0; i < C[cur].size(); i++) {
            if (path[i] == big_val && C[cur][i] > 0) {
                path[i] = cur;
                flow = min(flow, C[cur][i]);

                if (i == dst) {
                    return flow;
                }

                q.push({i, flow});
            }
        }
    }

    return 0;
}

size_t max_flow(size_t src, size_t dst, vector<vector<size_t>> C) {
    size_t flow = 0;
    vector<size_t> path(C.size());
    size_t additional_flow;

    while ((additional_flow = bfs(src, dst, path, C))) {
        flow += additional_flow;

        // Update
        size_t cur = dst;
        while (cur != src) {
            size_t prev = path[cur];
            C[prev][cur] -= additional_flow;
            C[cur][prev] += additional_flow;

            cur = prev;
        }
    }

    return flow;
}

size_t min_max_flow(const vector<vector<size_t>>& C) {
    size_t ans = INT32_MAX;

    for (int i = 0; i < C.size() - 1; i++) {
        for (int j = i + 1; j < C.size(); j++) {
            ans = min(ans, max_flow(i, j, C));
        }
    }

    return ans;
}

int main() {
    ifstream fin("inputs/25.in");

    string line;

    size_t ans_1 = 0;
    size_t ans_2 = 0;

    map<size_t, vector<size_t>> G;
    map<string, size_t> ids;

    vector<vector<size_t>> E;
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
    for (auto& e : E) {
        e.resize(N, 0);
    }

    for (const auto& [src, dsts] : G) {
        for (const auto& dst : dsts) {
            E[src][dst] = 1;
            E[dst][src] = 1;
        }
    }

    size_t base_connectivity = min_max_flow(E);

    size_t s = 0;
    size_t t = 1;
    for (; t < N; t++) {
        if (max_flow(s, t, E) == base_connectivity) {
            break;
        }
    }

    vector<pair<size_t, size_t>> edges;
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (E[i][j]) {
                E[i][j] = 0;
                E[j][i] = 0;

                size_t connectivity_lvl = max_flow(s, t, E);
                if (connectivity_lvl < base_connectivity) {
                    edges.push_back({i, j});
                }

                E[i][j] = 1;
                E[j][i] = 1;
            }
        }
    }

    assert(edges.size() == 3);
    for (auto e : edges) {
        E[e.first][e.second] = 0;
        E[e.second][e.first] = 0;
    }

    auto cc = components(E);
    cout << cc.first << " " << cc.second << endl;

    return 0;
}
