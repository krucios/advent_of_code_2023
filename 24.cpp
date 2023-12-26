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
#include <regex>
#include <map>
#include <set>
#include <bit>

#include <Eigen/Dense>

using namespace std;

struct Point {
    long double x;
    long double y;
    long double z;
};

struct Hail {
    Point p;

    long double dx;
    long double dy;
    long double dz;

    // For XY plane: y = k * x + b
    long double k;
    long double b;

    bool is_in_future(const Point& p) const {
        long double tx = (p.x - this->p.x) / this->dx;
        long double ty = (p.y - this->p.y) / this->dy;

        return tx > 0 && ty > 0;
    }

    long double time_to_reach(const Point& p) const {
        long double tx = (p.x - this->p.x) / this->dx;
        long double ty = (p.y - this->p.y) / this->dy;

        return (tx != ty) ? -1 : tx;
    }
};

Hail string_to_hail(const string& line) {
    stringstream ss(line);

    Hail h;

    ss >> h.p.x;
    ss.ignore(2);

    ss >> h.p.y;
    ss.ignore(2);

    ss >> h.p.z;
    ss.ignore(2);

    ss >> h.dx;
    ss.ignore(2);

    ss >> h.dy;
    ss.ignore(2);

    ss >> h.dz;
    ss.ignore(2);

    h.k = h.dy / h.dx;
    h.b = h.p.y - h.k * h.p.x;

    return h;
}

optional<Point> path_intersection_xy(const Hail& a, const Hail& b) {
    if ((a.k - b.k) == 0) {
        // Parallel lines
        return nullopt;
    }

    Point p;
    p.x = (b.b - a.b) / (a.k - b.k);
    p.y = a.k * p.x + a.b;

    return p;
}

optional<Point> get_intersection(const Hail& a, const Hail& b) {
    Eigen::Vector3d p(a.p.x, a.p.y, a.p.z);
    Eigen::Vector3d v(a.dx, a.dy, a.dz);
    Eigen::Vector3d q(b.p.x, b.p.y, b.p.z);
    Eigen::Vector3d u(b.dx, b.dy, b.dz);

    Eigen::Vector3d c = v.cross(u);

    long double dot = c.dot(c);

    if (dot == 0) {
        return nullopt;
    }

    Eigen::Vector3d d = (q - p).cross(u);
    long double t = d.dot(c) / dot;

    Eigen::Vector3d point = p + (t * v);

    Point intersection;
    intersection.x = point(0);
    intersection.y = point(1);
    intersection.z = point(2);

    return intersection;
}

int main() {
    ifstream fin("inputs/24.in");

    string line;

    size_t ans_1 = 0;
    long double ans_2 = 0;

    vector<Hail> hails;

    while (getline(fin, line)) {
        hails.push_back(string_to_hail(line));
    }

    for (int i = 0; i < hails.size() - 1; i++) {
        for (int j = i + 1; j < hails.size(); j++) {
            auto p = path_intersection_xy(hails[i], hails[j]);

            if (p) {
                bool ti = hails[i].is_in_future(*p);
                bool tj = hails[j].is_in_future(*p);

                if (ti && tj) {
                    ans_1 += (200000000000000.0L <= p->x && p->x <= 400000000000000.0L)
                          && (200000000000000.0L <= p->y && p->y <= 400000000000000.0L);
                }
            }
        }
    }

    int search_size = 300;
    for (int x = -search_size; x < search_size; x++) {
        for (int y = -search_size; y < search_size; y++) {
            for (int z = -search_size; z < search_size; z++) {
                Point prev_intersection;
                bool first_intersection = true;
                bool intersection_found = true;

                for (int i = 0; i < (hails.size() - 1) && intersection_found; i++) {
                    for (int j = i + 1; j < hails.size() && intersection_found; j++) {
                        Hail a = hails[i];
                        Hail b = hails[j];

                        a.dx -= x;
                        a.dy -= y;
                        a.dz -= z;

                        b.dx -= x;
                        b.dy -= y;
                        b.dz -= z;

                        auto p = get_intersection(a, b);

                        if (p) {
                            if (!first_intersection) {
                                intersection_found &= fabsl(prev_intersection.x - p->x) < 1.0L;
                                intersection_found &= fabsl(prev_intersection.y - p->y) < 1.0L;
                                intersection_found &= fabsl(prev_intersection.z - p->z) < 1.0L;
                            }
                            prev_intersection = *p;
                        } else {
                            intersection_found = false;
                        }

                        first_intersection = false;
                    }
                }

                if (intersection_found) {
                    ans_2 = prev_intersection.x + prev_intersection.y + prev_intersection.z;
                }
            }
        }
    }

    cout << ans_1 << endl;
    cout << (int64_t)ans_2 << endl;

    return 0;
}
