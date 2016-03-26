//
// Created by kamenev on 26.03.16.
//
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>

#define x first
#define y second
typedef std::pair<int64_t , int64_t> Point;

int64_t oriented_sq(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

bool cw(Point a, Point b, Point c) {
    return oriented_sq(a, b, c) < 0;
}

bool ccw(Point a, Point b, Point c) {
    return oriented_sq(a, b, c) > 0;
}

int main() {
    auto fin = new std::fstream("hull.in", std::fstream::in);
    auto fout = new std::fstream("hull.out", std::fstream::out | std::fstream::trunc);
    int n;
    fin->operator>>(n);
    std::vector<Point> points;
    for (int i = 0; i < n; i++) {
        double x, y;
        fin->operator>>(x);
        fin->operator>>(y);
        points.push_back({x, y});
    }
    std::sort(points.begin(), points.end());
    std::vector<Point> up, down;
    Point p1 = points.front(); // левая нижняя
    Point p2 = points.back(); // правая верхняя
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i = 1; i < points.size(); i++) {
        if (i == points.size() - 1 || cw(p1, points[i], p2)) {
            while (up.size() >= 2 && !cw(up[up.size() - 2], up[up.size() - 1], points[i]))
                up.pop_back();
            up.push_back(points[i]);
        }
        if (i == points.size() - 1 || ccw(p1, points[i], p2)) {
            while (down.size() >= 2 && !ccw(down[down.size() - 2], down[down.size() - 1], points[i]))
                down.pop_back();
            down.push_back(points[i]);
        }
    }
    points = up;
    std::copy(down.rbegin() + 1, down.rend() - 1, std::back_inserter(points));
    *fout << points.size() << std::endl;
    std::transform(points.begin(), points.end(), std::ostream_iterator<std::string>(*fout, "\n"),
                   [](Point &a) {
                       return std::to_string(a.first) + " " + std::to_string(a.second);
                   });
    delete fin;
    delete fout;
}