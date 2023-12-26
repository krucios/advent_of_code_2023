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

class Node;

struct Action {
    string sender;
    Node* receiver;
    bool polarity;
};

class ActionQueue {
public:
    queue<Action> q;

    size_t count_low;
    size_t count_high;

    void register_pulse(const string& sender, Node* receiver, bool polarity) {
        Action a;
        a.sender = sender;
        a.receiver = receiver;
        a.polarity = polarity;

        this->q.push(a);

        this->count_low  += polarity == 0;
        this->count_high += polarity == 1;
    }

    bool is_empty() const {
        return this->q.empty();
    }
};

class Node {
public:
    string name;
    vector<Node*> output;

    Node(const string& name) {
        this->name = name;
    }

    void add_output(Node* d) {
        this->output.push_back(d);
    }

    virtual bool pulse(const string& input, bool polarity, ActionQueue* q) { return false; }
    virtual void add_input(const string& inputs) {}
};

class Broadcaster : public Node {
public:
    Broadcaster(const string& name) : Node(name) {}

    virtual bool pulse(const string& input, bool polarity, ActionQueue* q) {
        for (Node* d : this->output) {
            q->register_pulse(this->name, d, polarity);
        }
        return false;
    }
};

class FlipFlop : public Node {
    bool state;

public:
    FlipFlop(const string& name) : Node(name) {
        this->state = false;
    }

    virtual bool pulse(const string& input, bool polarity, ActionQueue* q) {
        if (!polarity) {
            this->state = !this->state;

            for (Node* d : this->output) {
                q->register_pulse(this->name, d, this->state);
            }
        }
        return false;
    }
};

class Conjunction : public Node {
    map<string, bool> inputs;

public:
    Conjunction(const string& name) : Node(name) {
        this->inputs.clear();
    }

    void add_input(const string& i) {
        this->inputs[i] = 0;
    }

    virtual bool pulse(const string& input, bool polarity, ActionQueue* q) {
        bool result;

        this->inputs[input] = polarity;

        result = all_of(this->inputs.begin(), this->inputs.end(), [](const auto& e) { return e.second; });
        for (Node* d : this->output) {
            q->register_pulse(this->name, d, !result);
        }

        return false;
    }
};

class RX : public Node {
public:
    RX(const string& name) : Node(name) {}

    virtual bool pulse(const string& input, bool polarity, ActionQueue* q) {
        return polarity == 0;
    }
};

int main() {
    ifstream fin("inputs/20.in");

    string line;

    uint64_t ans_1 = 0;
    uint64_t ans_2 = 0;

    map<string, vector<string>> input_map;
    map<string, vector<string>> output_map;
    map<string, Node*> nodes_map;

    while (getline(fin, line)) {
        stringstream ss(line);

        Node* n;
        string name;
        string token;
        ss >> token;

        if (token[0] == '%') {
            token.erase(0, 1);
            FlipFlop* f = new FlipFlop(token);
            n = f;
        } else if (token[0] == '&') {
            token.erase(0, 1);
            Conjunction* c = new Conjunction(token);
            n = c;
        } else {
            Broadcaster* b = new Broadcaster(token);
            n = b;
        }

        name = token;
        nodes_map[name] = n;

        ss >> token;
        assert(token == "->");
        ss.ignore(1);

        while (getline(ss, token, ',')) {
            ss.ignore(1);
            input_map[token].push_back(name);
            output_map[name].push_back(token);
        }
    }

    for (const auto& [name, signals] : input_map) {
        if (nodes_map.find(name) == nodes_map.end()) {
            Node* n = name == "rx" ? new RX(name) : new Node(name);
            nodes_map[name] = n;
        }
    }

    for (const auto& [name, signals] : output_map) {
        if (nodes_map.find(name) == nodes_map.end()) {
            Node* n = name == "rx" ? new RX(name) :
            nodes_map[name] = n;
        }
    }

    for (const auto& [name, node] : nodes_map) {
        for (const auto& i : input_map[name]) {
            node->add_input(i);
        }
        for (const auto& o : output_map[name]) {
            node->add_output(nodes_map[o]);
        }
    }

    ActionQueue* actions = new ActionQueue();
    bool stop = false;
    for (int step = 0; step < 1000; step++) {
        actions->register_pulse("Push button", nodes_map["broadcaster"], false);

        while (!actions->q.empty()) {
            Action a = actions->q.front();
            actions->q.pop();

            stop = a.receiver->pulse(a.sender, a.polarity, actions);
        }
    }

    ans_1 = actions->count_high * actions->count_low;

    //
    // Part 2 should be solved via graph representation
    // there are 4 counters and schematic will trigger RX once LCM of them appeared
    // My counter periods are: 3917, 3919, 4007, 4027
    //
    ans_2 = lcm(3917ULL, 3919ULL);
    ans_2 = lcm(ans_2, 4007ULL);
    ans_2 = lcm(ans_2, 4027ULL);


    cout << ans_1 << endl;
    cout << ans_2 << endl;

    return 0;
}
